/*
 * libusb example program to list devices on the bus
 * Copyright Ã‚Â© 2007 Daniel Drake <dsd@gentoo.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */
 
#include <stdio.h>
#include<stdint.h>
#include <libusb-1.0/libusb.h>
 
#define USB_VENDOR_ID 0x0483
#define USB_PRODUCT_ID 0x8007
 
#define BULK_ENDPOINT_OUT 1
#define BULK_ENDPOINT_IN  2
 
struct userDevice{
	/*Device descriptor*/
 
	/** USB-IF vendor ID */
	uint16_t idVendor;
 
	/** USB-IF product ID */
	uint16_t idProduct;
 
	/*Interface descriptor*/
 
	/** USB-IF class code for this interface. See \ref libusb_class_code. */
	uint8_t  bInterfaceClass;
 
	/** USB-IF subclass code for this interface, qualified by the
	 * bInterfaceClass value */
	uint8_t  bInterfaceSubClass;
 
	/*Endpoint descriptor*/
 
	/** Attributes which apply to the endpoint when it is configured using
	 * the bConfigurationValue. Bits 0:1 determine the transfer type and
	 * correspond to \ref libusb_transfer_type. Bits 2:3 are only used for
	 * isochronous endpoints and correspond to \ref libusb_iso_sync_type.
	 * Bits 4:5 are also only used for isochronous endpoints and correspond to
	 * \ref libusb_iso_usage_type. Bits 6:7 are reserved.
	 */
	
	uint8_t  bmAttributes;
 
	
	/*save parameter*/
 
	libusb_device *dev;
	libusb_device **devs;
 
 
	uint8_t bInEndpointAddress;
 
	uint8_t bOutEndpointAddress;
 
	/* Number of this interface */
	uint8_t  bInterfaceNumber;
};
 
 
int init_libusb(void)
{
	/*1. init libusb lib*/
	int rv = 0;
	
	rv = libusb_init(NULL);
	if(rv < 0)
	{
		printf("*** initial USB lib failed! \n");    
		return -1;
	}
	return rv;
}
 
 
int get_device_descriptor(struct libusb_device_descriptor *dev_desc,struct userDevice *user_device)
{
	/*2.get device descriptor*/
	int rv = -2;
	ssize_t cnt;
	int i = 0;
 
	libusb_device **devs;
	libusb_device *dev;
 
	cnt = libusb_get_device_list(NULL, &devs); //check the device number
	if (cnt < 0)
		return (int) cnt;
 
	while ((dev = devs[i++]) != NULL)
	{
		rv = libusb_get_device_descriptor(dev,dev_desc);
		if(rv < 0)
		{
			printf("*** libusb_get_device_descriptor failed! i:%d \n",i);
			return -1;
		}
		if(dev_desc->idProduct==user_device->idProduct &&dev_desc->idVendor==user_device->idVendor)
		{
			user_device->dev = dev;
			user_device->devs = devs;
			rv = 0;
			break;
		}
	}
	return rv;
}
 
 
int match_with_endpoint(const struct libusb_interface_descriptor * interface, struct userDevice *user_device)
{
	int i;
	int ret=0;
	for(i=0;i<interface->bNumEndpoints;i++)
	{
		if((interface->endpoint[i].bmAttributes&0x03)==user_device->bmAttributes)   //transfer type :bulk ,control, interrupt
		{
				if(interface->endpoint[i].bEndpointAddress&0x80)					//out endpoint & in endpoint
				{
					ret|=1;
					user_device->bInEndpointAddress = interface->endpoint[i].bEndpointAddress;
				}
				else
				{
					ret|=2;
					user_device->bOutEndpointAddress = interface->endpoint[i].bEndpointAddress;
				}
		}
										
	}
	if(ret==3)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
 
int get_device_endpoint(struct libusb_device_descriptor *dev_desc,struct userDevice *user_device)
{
	/*3.get device endpoint that you need */
	int rv = -2;
	int i,j,k;
	struct libusb_config_descriptor *conf_desc;
	uint8_t isFind = 0;
	for (i=0; i< dev_desc->bNumConfigurations; i++)
	{
		if(user_device->dev != NULL)
			rv = libusb_get_config_descriptor(user_device->dev,i,&conf_desc);
		if(rv < 0)
		{
			printf("*** libusb_get_config_descriptor failed! \n");    
			return -1;
		}
		for (j=0; j< conf_desc->bNumInterfaces; j++)
		{
			for (k=0; k < conf_desc->interface[j].num_altsetting; k++)
			{
				if(
					conf_desc->interface[j].altsetting[k].bInterfaceClass==user_device->bInterfaceClass
					)
				{
					if(match_with_endpoint(&(conf_desc->interface[j].altsetting[k] ), user_device ))
					{
						user_device->bInterfaceNumber = conf_desc->interface[j].altsetting[k].bInterfaceNumber;
						libusb_free_config_descriptor(conf_desc);
						rv = 0;
						return rv;
					}
				}
			}
		}
	}
	return -2;  //don't find user device
}
 
 
int mainTransfer(void)
{
	int rv;
 
	int length;
 
	unsigned char a[100] = {0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a,0x0a};
 
	libusb_device_handle* g_usb_handle;
 
	struct userDevice user_device;
	struct libusb_device_descriptor dev_desc;
 
	user_device.idProduct = USB_PRODUCT_ID;
	user_device.idVendor =  USB_VENDOR_ID ;
	user_device.bInterfaceClass = LIBUSB_CLASS_PRINTER ;
	user_device.bInterfaceSubClass = LIBUSB_CLASS_PRINTER ;
	user_device.bmAttributes = LIBUSB_TRANSFER_TYPE_BULK ;
	user_device.dev = NULL;
 
	init_libusb();
 
	rv = get_device_descriptor(&dev_desc,&user_device);
	if(rv < 0)
	{
			printf("*** get_device_descriptor failed! \n");    
			return -1;
	}
 
	rv = get_device_endpoint(&dev_desc,&user_device);
	if(rv < 0)
	{
			printf("*** get_device_endpoint failed! rv:%d \n",rv);    
			return -1;
	}
 
	
	/*4.open device and start communication by usb*/
	//open the usb device
	g_usb_handle = libusb_open_device_with_vid_pid(NULL, user_device.idVendor, user_device.idProduct);
	if(g_usb_handle == NULL)
	{
		printf("*** Permission denied or Can not find the USB board (Maybe the USB driver has not been installed correctly), quit!\n");
		return -1;
	}
 
	rv = libusb_claim_interface(g_usb_handle,user_device.bInterfaceNumber);
	if(rv < 0)
	{
		rv = libusb_detach_kernel_driver(g_usb_handle,user_device.bInterfaceNumber);
		if(rv < 0)
		{
			printf("*** libusb_detach_kernel_driver failed! rv%d\n",rv);    
			return -1;
		}
		rv = libusb_claim_interface(g_usb_handle,user_device.bInterfaceNumber);
		if(rv < 0)
		{
			printf("*** libusb_claim_interface failed! rv%d\n",rv);    
			return -1;
		}
 
	}
 
 
	rv = libusb_bulk_transfer(g_usb_handle,BULK_ENDPOINT_OUT,a,100,&length,1000);	
	if(rv < 0)
	{
		printf("*** bulk_transfer failed! \n");    
		return -1;
	}
 
	libusb_close(g_usb_handle);
 
	libusb_release_interface(g_usb_handle,user_device.bInterfaceNumber);
 
	libusb_free_device_list(user_device.devs, 1);
 
	libusb_exit(NULL);
}
