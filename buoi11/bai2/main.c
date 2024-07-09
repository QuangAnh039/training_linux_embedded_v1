#include <linux/module.h>
#include <linux/init.h>
#include <linux/mod_devicetable.h>
#include <linux/property.h>
#include <linux/platform_device.h>
#include <linux/of_device.h>
#include <linux/gpio/consumer.h>
#include <linux/of.h> 
#include <linux/proc_fs.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>
#include <linux/interrupt.h>
#include <linux/delay.h>

#define DRIVER_AUTHOR       "Nguyen Quang Anh"
#define DRIVER_DESC         "using interrupt for button"

#define LOW     0
#define HIGH    1

static struct gpio_desc *gpio0_30 = NULL;
static struct gpio_desc *gpio0_31 = NULL;
int irq = 0;

static irqreturn_t gpio_intterupt_handler(int irq_num, void *dev_id) 
{
    printk("gpio_irq: Interrupt was triggered and ISR was called!\n");
    gpiod_set_value(gpio0_30, !(gpiod_get_value(gpio0_30)));
    return IRQ_HANDLED;
}

static int my_pdrv_probe(struct platform_device *pdev)
{
    int ret = 0;

    //Init LED
    struct device *dev = &pdev->dev;
    gpio0_30 = gpiod_get(dev, "led", GPIOD_OUT_LOW);
    gpiod_set_value(gpio0_30, HIGH);

    //Init Button
    gpio0_31 = gpiod_get(dev, "button", GPIOD_IN);
    irq = gpiod_to_irq(gpio0_31);
    if (irq < 0) 
    {
        printk(KERN_ERR "Failed to get IRQ for platform device\n");
    }
    else
    {
        printk(KERN_INFO "IRQ for platform device: %d\n", irq);
    }

    if(request_irq(irq, (void *)gpio_intterupt_handler, IRQF_TRIGGER_FALLING, "button", NULL))
    {
        pr_err("Cannot register interrupt number: %d\n", irq);
        return -1;
    }
    pr_info("Device Driver Insert...Done!!!\n");
    return 0;
}

static int my_pdrv_remove(struct platform_device *pdev)
{
    gpiod_set_value(gpio0_30, LOW);
    gpiod_put(gpio0_30);
    free_irq(irq, NULL);
    gpiod_put(gpio0_31);
    pr_info("Device Driver Remove...Done!!!\n");
    return 0;
}

static const struct of_device_id gpio_dt_ids[] = {
    { .compatible = "gpio-on-of-led", },
    { /* sentinel */ },
};

static struct platform_driver mypdrv = {
    .probe = my_pdrv_probe,
    .remove = my_pdrv_remove,
    .driver = {
        .name = "on-off-led",
        .of_match_table = gpio_dt_ids,
    }
};

module_platform_driver(mypdrv);
MODULE_DEVICE_TABLE(of, gpio_dt_ids);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);