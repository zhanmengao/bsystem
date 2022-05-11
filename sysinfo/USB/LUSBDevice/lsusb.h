#pragma once
#include"USBUtils.h"

/*
return 0 means don't equal;return 1 means equal.
*/
int chk_serial_number(uint16_t pid, uint16_t vid, char *sn)
{
	//usb information index
	struct libusb_device_descriptor usb_info;
	//usb device
	struct libusb_device *usb_d = NULL;
	//the handle of the opened usb device
	struct libusb_device_handle *usb_p = NULL;
	//buffer
	char buf[CHAR_MAX_LEN] = { 0 };

	usb_p = libusb_open_device_with_vid_pid(NULL, pid, vid);
	if (usb_p != 0)
	{
		//find information index
		usb_d = libusb_get_device(usb_p);
		if (libusb_get_device_descriptor(usb_d, &usb_info) != 0)
		{
			perror("can't find usb device's information");
			libusb_close(usb_p);
			return 0;
		}
		//find SerialNumber
		libusb_get_string_descriptor_ascii(usb_p, usb_info.iSerialNumber, (unsigned char*)buf, CHAR_MAX_LEN);
		return (strcmp(buf, sn) == 0);
	}
	else
	{
		perror("can't find usb device");
		libusb_close(usb_p);
		return 0;
	}
	libusb_close(usb_p);
	return 1;
}


/*
initialization
*/
void t_init()
{
	libusb_init(NULL);
}


/*
when app distory
*/
void t_exit()
{
	libusb_exit(NULL);
}

ssize_t lsUsbDevice(struct usbDevice *st);

void mainlsusb()
{
	struct usbDevice st[CHAR_MAX_LEN] = { 0 };
	ssize_t usb_list_len = 0;
	int i = 0;
	t_init();
	printf("%d", chk_serial_number(0x8829, 0x0010, "9053053023054027"));
	usb_list_len = lsUsbDevice(st);
	for (i = 0; i < usb_list_len; ++i)
	{
		printf("product:%s\nsn:%s\npid:%x\nvid:%x\n\n", st[i].product, st[i].sn, st[i].pid, st[i].vid);
	}
	t_exit();
}