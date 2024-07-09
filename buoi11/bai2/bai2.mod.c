#include <linux/build-salt.h>
#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(.gnu.linkonce.this_module) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section(__versions) = {
	{ 0x441199c1, "module_layout" },
	{ 0xf9a482f9, "msleep" },
	{ 0xb1ad28e0, "__gnu_mcount_nc" },
	{ 0x1e6d42b5, "__platform_driver_register" },
	{ 0xc5850110, "printk" },
	{ 0x92d5838e, "request_threaded_irq" },
	{ 0x966efd2b, "gpiod_get_value" },
	{ 0x33c18bef, "gpiod_to_irq" },
	{ 0x4c5731ee, "platform_driver_unregister" },
	{ 0x17c39e9a, "gpiod_set_value" },
	{ 0xeaa68e42, "gpiod_put" },
	{ 0xc1514a3b, "free_irq" },
	{ 0x8fdbf9ab, "gpiod_get" },
};

MODULE_INFO(depends, "");

MODULE_ALIAS("of:N*T*Cgpio-on-of-led");
MODULE_ALIAS("of:N*T*Cgpio-on-of-ledC*");
