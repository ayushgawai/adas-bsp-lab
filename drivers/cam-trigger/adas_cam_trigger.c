// SPDX-License-Identifier: GPL-2.0
/* GPIO camera trigger: IRQ counts pulses; sysfs exposes count + fire. */
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/gpio/consumer.h>
#include <linux/interrupt.h>
struct trig { struct gpio_desc *g; unsigned long count; };
static struct trig T;
static irqreturn_t irq_h(int i, void *d) { (void)i; (void)d; T.count++; return IRQ_HANDLED; }
static ssize_t count_show(struct device *dev, struct device_attribute *a, char *b) { return sysfs_emit(b, "%lu\n", T.count); }
static ssize_t fire_store(struct device *dev, struct device_attribute *a, const char *b, size_t n) {
	if (T.g) { gpiod_set_value_cansleep(T.g, 1); gpiod_set_value_cansleep(T.g, 0); } return n; }
static DEVICE_ATTR_RO(count); static DEVICE_ATTR_WO(fire);
static struct attribute *attrs[] = { &dev_attr_count.attr, &dev_attr_fire.attr, NULL };
static const struct attribute_group group = { .attrs = attrs };
static int probe(struct platform_device *p) {
	int irq;
	T.g = devm_gpiod_get_optional(&p->dev, "trigger", GPIOD_OUT_LOW);
	if (IS_ERR(T.g)) return PTR_ERR(T.g);
	if (T.g) { irq = gpiod_to_irq(T.g); if (irq >= 0) devm_request_irq(&p->dev, irq, irq_h, NULL, IRQF_TRIGGER_RISING, "cam-trigger", &T); }
	return sysfs_create_group(&p->dev.kobj, &group);
}
static struct platform_driver drv = { .probe = probe, .driver = { .name = "adas-cam-trigger" } };
module_platform_driver(drv); MODULE_LICENSE("GPL"); MODULE_DESCRIPTION("ADAS camera trigger");
