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
#include <linux/uaccess.h>         //copy_to/from_user()
#include <linux/ioctl.h>
#include <linux/err.h>
#include <linux/timer.h>
#include <linux/jiffies.h>

#include "blink_ioctl.h"


#define DRIVER_AUTHOR       "Nguyen Quang Anh"
#define DRIVER_DESC         "module gpio blink led "

#define LOW     0
#define HIGH    1
/*****************************************************************************************************
************************************ _PROTOTYPE_FUNCTION_ ********************************************
*****************************************************************************************************/
static int device_open(struct inode *device_file, struct file *instance);
static int device_close(struct inode *device_file, struct file *instance);
static long blink_ioctl(struct file *instance, unsigned int cmd, unsigned long arg);
static int my_pdrv_probe(struct platform_device *pdev);
static int my_pdrv_remove(struct platform_device *pdev);

static struct gpio_desc *gpio0_30 = NULL;
static struct timer_list etx_timer;

void timer_callback(struct timer_list * data)
{
    int gpio_value;
    gpio_value = gpiod_get_value(gpio0_30);
    if(gpio_value == 0)
    {
        gpiod_set_value(gpio0_30, HIGH);
    }
    else if(gpio_value == 1)
    {
        gpiod_set_value(gpio0_30, LOW);
    }
    /*
       Re-enable timer. Because this function will be called only first time. 
       If we re-enable this will work like periodic timer. 
    */
    mod_timer(&etx_timer, jiffies + HZ);
}

static struct file_operations fops = {
	.owner      = THIS_MODULE,
	.open       = device_open,
    .unlocked_ioctl = blink_ioctl,
	.release    = device_close
};

// Struct use to register device file after match with DT
struct miscdevice led = {
    .minor  = MISC_DYNAMIC_MINOR,
    .name   = "blinkled",
    .fops   = &fops,
};

//This function will be called when we open the Device file
static int device_open(struct inode *device_file, struct file *instance) 
{
	printk("open was called!\n");
	return 0;
}
//This function will be called when we close the Device file
static int device_close(struct inode *device_file, struct file *instance) 
{
	printk("close was called!\n");
	return 0;
}
//This function will be called when we write IOCTL on the Device file
static long blink_ioctl(struct file *instance, unsigned int cmd, unsigned long arg)
{
    switch(cmd)
    {
        case IOCTL_BLINK:
            //setup timer
            timer_setup(&etx_timer, timer_callback, 0);
            mod_timer(&etx_timer, jiffies + HZ);
            pr_info("Blinking\n");
            break;
        default:
            pr_info("Default\n");
            break;
    }
    return 0;
}
// function is called when match!
static int my_pdrv_probe(struct platform_device *pdev)
{
    int ret = 0;
    struct device *dev = &pdev->dev;
    // Config LED ON after math with DT
    gpio0_30 = gpiod_get(dev, "led", GPIOD_OUT_LOW);
    gpiod_set_value(gpio0_30, HIGH);

    // create character device file 
    ret = misc_register(&led);
    if(ret)
    {
        pr_err("can't misc_register\n");
        return -1;
    }
    // log announce probe successfully!!
    pr_info("%s - %d", __func__, __LINE__);
    return 0;
}

// function is called when rmmod
static int my_pdrv_remove(struct platform_device *pdev)
{
    // Config LED OFF
    gpiod_set_value(gpio0_30, LOW);
    gpiod_put(gpio0_30);

    // free character device file
    misc_deregister(&led);

    // log announce remove successfully!!
    pr_info("%s - %d", __func__, __LINE__);
    return 0;
}

static const struct of_device_id gpio_dt_ids[] = {
    { .compatible = "blink-led", },
    { /* sentinel */ },
};  

static struct platform_driver mypdrv = {
    .probe = my_pdrv_probe,
    .remove = my_pdrv_remove,
    .driver = {
        .name = "blink-led",
        .of_match_table = gpio_dt_ids,
    }
};

// register platform_driver
module_platform_driver(mypdrv);
// register match with Device tree
MODULE_DEVICE_TABLE(of, gpio_dt_ids);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);