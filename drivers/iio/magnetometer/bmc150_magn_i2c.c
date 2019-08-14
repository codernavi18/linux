/*
 * 3-axis magnetometer driver supporting following I2C Bosch-Sensortec chips:
 *  - BMC150
 *  - BMC156
 *  - BMM150
 *
 * Copyright (c) 2016, Intel Corporation.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 */
#include <linux/device.h>
#include <linux/mod_devicetable.h>
#include <linux/i2c.h>
#include <linux/module.h>
#include <linux/acpi.h>
#include <linux/regmap.h>

#include "bmc150_magn.h"

static int bmc150_magn_i2c_probe(struct i2c_client *client,
				 const struct i2c_device_id *id)
{
	pr_info("!!!!! probe called !!!!!\n");
	struct regmap *regmap;
	const char *name = NULL;

	/* create a regmap using the defined regmap_config*/
	regmap = devm_regmap_init_i2c(client, &bmc150_magn_regmap_config);
	if (IS_ERR(regmap)) {
		dev_err(&client->dev, "Failed to initialize i2c regmap\n");
		return PTR_ERR(regmap);
	}

	//id gets filled even if the match happens because of device tree
	//If we do not defined i2c_device_id (which is completely optional)
	//then we will get id as blank
	if (id)
	{
		name = id->name; //comes here
		pr_info("name received : %s",name); //prints bmc150_magn
	}
	else
	{
		pr_info("id is not defined, since probed by device tree\n");
	}

	pr_info("client->irq : %d\n",client->irq); //prints 0 since not initalized
	//call internal probe() method that is common to both I2C and SPI.
	return bmc150_magn_probe(&client->dev, regmap, client->irq, name);
}

//calls internal remove() method that is common to both I2C and SPI.
static int bmc150_magn_i2c_remove(struct i2c_client *client)
{
	return bmc150_magn_remove(&client->dev);
}

static const struct acpi_device_id bmc150_magn_acpi_match[] = {
	{"BMC150B", 0},
	{"BMC156B", 0},
	{"BMM150B", 0},
	{},
};
MODULE_DEVICE_TABLE(acpi, bmc150_magn_acpi_match);

/*
 * Till kernel version 4.10 or earlier you need to define both
 * .id_table and .of_match_table, else your driver will not get probed.
 * This however have been fixed in the later versions of the kernel.
 */
static const struct i2c_device_id bmc150_magn_i2c_id[] = {
	{"bmc150_magn",	0},
	{"bmc156_magn", 0},
	{"bmm150_magn", 0},
	{}
};
MODULE_DEVICE_TABLE(i2c, bmc150_magn_i2c_id);

static const struct of_device_id bmc150_magn_of_match[] = {
	{ .compatible = "bosch,bmc150_magn" },
	{ .compatible = "bosch,bmc156_magn" },
	{ .compatible = "bosch,bmm150_magn" },
	{ }
};
MODULE_DEVICE_TABLE(of, bmc150_magn_of_match);

//Front face of the driver
static struct i2c_driver bmc150_magn_driver = {
	.driver = {
		.name	= "bmc150_magn_i2c",  //last option to match the device-driver
		.of_match_table = bmc150_magn_of_match, //maily for ARM platform or other device-tree compatible platforms
		.acpi_match_table = ACPI_PTR(bmc150_magn_acpi_match), //for x86 platforms
		.pm	= &bmc150_magn_pm_ops, //these are power management callbacks - suspens,resume,poweroff etc
	},
	.probe		= bmc150_magn_i2c_probe,
	.remove		= bmc150_magn_i2c_remove,
	.id_table	= bmc150_magn_i2c_id,
};
module_i2c_driver(bmc150_magn_driver);

MODULE_AUTHOR("Daniel Baluta <daniel.baluta@intel.com");
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("BMC150 I2C magnetometer driver");
