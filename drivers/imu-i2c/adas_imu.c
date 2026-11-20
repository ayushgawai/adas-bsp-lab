// SPDX-License-Identifier: GPL-2.0
/* Mock I2C IMU -> IIO. ponytail: register reads return a synthesized ramp; swap regmap reads for real silicon. */
#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/iio/iio.h>
#define IMU_WHOAMI 0x00
struct imu { struct i2c_client *c; };
static int imu_read(struct iio_dev *d, struct iio_chan_spec const *ch, int *v, int *v2, long m){
  if(m!=IIO_CHAN_INFO_RAW) return -EINVAL; *v = 100 + ch->channel; *v2=0; return IIO_VAL_INT; }
static const struct iio_chan_spec chans[] = {
  { .type=IIO_ACCEL, .channel=0, .indexed=1, .info_mask_separate=BIT(IIO_CHAN_INFO_RAW) },
  { .type=IIO_ACCEL, .channel=1, .indexed=1, .info_mask_separate=BIT(IIO_CHAN_INFO_RAW) },
  { .type=IIO_ANGL_VEL, .channel=0, .indexed=1, .info_mask_separate=BIT(IIO_CHAN_INFO_RAW) } };
static const struct iio_info info = { .read_raw = imu_read };
static int imu_probe(struct i2c_client *c){ struct iio_dev *d=iio_device_alloc(&c->dev, sizeof(struct imu));
  if(!d) return -ENOMEM; d->name="adas-imu"; d->modes=INDIO_DIRECT_MODE; d->info=&info; d->channels=chans; d->num_channels=ARRAY_SIZE(chans);
  i2c_set_clientdata(c,d); return iio_device_register(d); }
static void imu_remove(struct i2c_client *c){ iio_device_unregister(i2c_get_clientdata(c)); }
static const struct of_device_id ids[]={{.compatible="adas,mock-imu"}, {}};
MODULE_DEVICE_TABLE(of, ids);
static struct i2c_driver drv={.driver={.name="adas-imu",.of_match_table=ids},.probe=imu_probe,.remove=imu_remove};
module_i2c_driver(drv); MODULE_LICENSE("GPL"); MODULE_DESCRIPTION("ADAS mock IMU (IIO)");
