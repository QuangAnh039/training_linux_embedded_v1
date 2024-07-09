#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/timer.h>
#include <linux/jiffies.h>
#include <linux/err.h>
#include <linux/gpio/consumer.h>
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

#define DRIVER_AUTHOR       "Nguyen Quang Anh"
#define DRIVER_DESC         "using timer polling button"

#define LOW     0
#define HIGH    1

static struct timer_list etx_timer;
static struct gpio_desc *gpio0_30 = NULL;
static struct gpio_desc *gpio0_31 = NULL;

void timer_callback(struct timer_list * data)
{
    int gpio_value;
    gpio_value = gpiod_get_value(gpio0_31);
    if(gpio_value == 0)
    {
        pr_info("button is pressed\n");
        gpiod_set_value(gpio0_30, !(gpiod_get_value(gpio0_30)));
    }
    else if(gpio_value == 1)
    {
        //do nothing
    }
    mod_timer(&etx_timer, jiffies + HZ/10);
}

static int my_pdrv_probe(struct platform_device *pdev)
{
    //Init LED
    struct device *dev = &pdev->dev;
    gpio0_30 = gpiod_get(dev, "led", GPIOD_OUT_LOW);
    gpiod_set_value(gpio0_30, LOW);

    //Init Button
    gpio0_31 = gpiod_get(dev, "button", GPIOD_IN);
    gpiod_set_value(gpio0_31, HIGH);

    //setup timer
    timer_setup(&etx_timer, timer_callback, 0);
    mod_timer(&etx_timer, jiffies + HZ/10);
    pr_info("Device Driver Insert...Done!!!\n");
    return 0;
}

static int my_pdrv_remove(struct platform_device *pdev)
{
    del_timer(&etx_timer);
    gpiod_put(gpio0_30);
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

//register platform_driver
module_platform_driver(mypdrv);
//register match with Device tree
MODULE_DEVICE_TABLE(of, gpio_dt_ids);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);