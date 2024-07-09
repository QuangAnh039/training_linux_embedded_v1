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
	{ 0xb1ad28e0, "__gnu_mcount_nc" },
	{ 0x561f734a, "serdev_device_write_buf" },
	{ 0xae94f8a5, "misc_register" },
	{ 0x22733741, "__serdev_device_driver_register" },
	{ 0xc5850110, "printk" },
	{ 0x56940520, "driver_unregister" },
	{ 0x961d553f, "serdev_device_set_parity" },
	{ 0x3362f8f3, "serdev_device_open" },
	{ 0x6d711361, "serdev_device_set_flow_control" },
	{ 0xcb723f92, "serdev_device_set_baudrate" },
	{ 0x62c169bd, "misc_deregister" },
};

MODULE_INFO(depends, "");

