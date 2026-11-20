// SPDX-License-Identifier: GPL-2.0
/* SPI CAN-FD skeleton -> SocketCAN. ponytail: mock transport loops TX back as RX; add SPI reg I/O for a real controller. */
#include <linux/module.h>
#include <linux/spi/spi.h>
#include <linux/can/dev.h>
struct canfd_priv { struct can_priv can; struct spi_device *spi; };
static netdev_tx_t xmit(struct sk_buff *skb, struct net_device *n){
  struct canfd_frame *f=(void*)skb->data; struct sk_buff *rx=can_create_echo_skb(skb);
  if(rx){ rx->protocol=htons(ETH_P_CANFD); netif_rx(rx); } n->stats.tx_packets++; dev_kfree_skb(skb); (void)f; return NETDEV_TX_OK; }
static int ndo_open(struct net_device *n){ open_candev(n); netif_start_queue(n); return 0; }
static int ndo_stop(struct net_device *n){ netif_stop_queue(n); close_candev(n); return 0; }
static const struct net_device_ops ops={.ndo_open=ndo_open,.ndo_stop=ndo_stop,.ndo_start_xmit=xmit};
static int probe(struct spi_device *s){ struct net_device *n=alloc_candev(sizeof(struct canfd_priv),1);
  if(!n) return -ENOMEM; n->netdev_ops=&ops; n->flags|=IFF_ECHO; SET_NETDEV_DEV(n,&s->dev);
  ((struct canfd_priv*)netdev_priv(n))->spi=s; ((struct canfd_priv*)netdev_priv(n))->can.ctrlmode_supported=CAN_CTRLMODE_FD;
  spi_set_drvdata(s,n); return register_candev(n); }
static void remove(struct spi_device *s){ unregister_candev(spi_get_drvdata(s)); free_candev(spi_get_drvdata(s)); }
static const struct of_device_id ids[]={{.compatible="adas,mock-canfd"}, {}};
MODULE_DEVICE_TABLE(of, ids);
static struct spi_driver drv={.driver={.name="adas-canfd",.of_match_table=ids},.probe=probe,.remove=remove};
module_spi_driver(drv); MODULE_LICENSE("GPL"); MODULE_DESCRIPTION("ADAS mock SPI CAN-FD (SocketCAN)");
