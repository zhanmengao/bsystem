#include"USBUtils.h"
#include"xusb.h"
#include"lsusb.h"

static int printdevice(uint16_t vid, uint16_t pid)
{
	libusb_device_handle *handle;
	libusb_device *dev;
	uint8_t bus, port_path[8];
	struct libusb_bos_descriptor *bos_desc;
	struct libusb_config_descriptor *conf_desc;
	const struct libusb_endpoint_descriptor *endpoint;
	int i, j, k, r;
	int iface, nb_ifaces, first_iface = -1;
	struct libusb_device_descriptor dev_desc;
	const char* const speed_name[6] = { "Unknown", "1.5 Mbit/s (USB LowSpeed)", "12 Mbit/s (USB FullSpeed)",
		"480 Mbit/s (USB HighSpeed)", "5000 Mbit/s (USB SuperSpeed)", "10000 Mbit/s (USB SuperSpeedPlus)" };
	char string[128];
	uint8_t string_index[3];	// indexes of the string descriptors
	uint8_t endpoint_in = 0, endpoint_out = 0;	// default IN and OUT endpoints

	printf("Opening device %04X:%04X...\n", vid, pid);
	handle = libusb_open_device_with_vid_pid(NULL, vid, pid);

	if (handle == NULL)
	{
		perr("  Failed.\n");
		return -1;
	}

	dev = libusb_get_device(handle);
	bus = libusb_get_bus_number(dev);
	if (extra_info)
	{
		r = libusb_get_port_numbers(dev, port_path, sizeof(port_path));
		if (r > 0)
		{
			printf("\nDevice properties:\n");
			printf("        bus number: %d\n", bus);
			printf("         port path: %d", port_path[0]);
			for (i = 1; i<r; i++)
			{
				printf("->%d", port_path[i]);
			}
			printf(" (from root hub)\n");
		}
		r = libusb_get_device_speed(dev);
		if ((r<0) || (r>5)) r = 0;
		printf("             speed: %s\n", speed_name[r]);
	}

	printf("\nReading device descriptor:\n");
	CALL_CHECK_CLOSE(libusb_get_device_descriptor(dev, &dev_desc), handle);
	printf("            length: %d\n", dev_desc.bLength);
	printf("      device class: %d\n", dev_desc.bDeviceClass);
	printf("               S/N: %d\n", dev_desc.iSerialNumber);
	printf("           VID:PID: %04X:%04X\n", dev_desc.idVendor, dev_desc.idProduct);
	printf("         bcdDevice: %04X\n", dev_desc.bcdDevice);
	printf("   iMan:iProd:iSer: %d:%d:%d\n", dev_desc.iManufacturer, dev_desc.iProduct, dev_desc.iSerialNumber);
	printf("          nb confs: %d\n", dev_desc.bNumConfigurations);
	// Copy the string descriptors for easier parsing
	string_index[0] = dev_desc.iManufacturer;
	string_index[1] = dev_desc.iProduct;
	string_index[2] = dev_desc.iSerialNumber;

	printf("\nReading BOS descriptor: ");
	if (libusb_get_bos_descriptor(handle, &bos_desc) == LIBUSB_SUCCESS)
	{
		printf("%d caps\n", bos_desc->bNumDeviceCaps);
		for (i = 0; i < bos_desc->bNumDeviceCaps; i++)
			print_device_cap(bos_desc->dev_capability[i]);
		libusb_free_bos_descriptor(bos_desc);
	}
	else
	{
		printf("no descriptor\n");
	}

	printf("\nReading first configuration descriptor:\n");
	CALL_CHECK_CLOSE(libusb_get_config_descriptor(dev, 0, &conf_desc), handle);
	nb_ifaces = conf_desc->bNumInterfaces;
	printf("             nb interfaces: %d\n", nb_ifaces);
	if (nb_ifaces > 0)
		first_iface = conf_desc->usb_interface[0].altsetting[0].bInterfaceNumber;
	for (i = 0; i<nb_ifaces; i++)
	{
		printf("              interface[%d]: id = %d\n", i,
			conf_desc->usb_interface[i].altsetting[0].bInterfaceNumber);
		for (j = 0; j<conf_desc->usb_interface[i].num_altsetting; j++)
		{
			printf("interface[%d].altsetting[%d]: num endpoints = %d\n",
				i, j, conf_desc->usb_interface[i].altsetting[j].bNumEndpoints);
			printf("   Class.SubClass.Protocol: %02X.%02X.%02X\n",
				conf_desc->usb_interface[i].altsetting[j].bInterfaceClass,
				conf_desc->usb_interface[i].altsetting[j].bInterfaceSubClass,
				conf_desc->usb_interface[i].altsetting[j].bInterfaceProtocol);
			if ((conf_desc->usb_interface[i].altsetting[j].bInterfaceClass == LIBUSB_CLASS_MASS_STORAGE)
				&& ((conf_desc->usb_interface[i].altsetting[j].bInterfaceSubClass == 0x01)
					|| (conf_desc->usb_interface[i].altsetting[j].bInterfaceSubClass == 0x06))
				&& (conf_desc->usb_interface[i].altsetting[j].bInterfaceProtocol == 0x50))
			{
				// Mass storage devices that can use basic SCSI commands
				test_mode = USE_SCSI;
			}
			for (k = 0; k<conf_desc->usb_interface[i].altsetting[j].bNumEndpoints; k++)
			{
				struct libusb_ss_endpoint_companion_descriptor *ep_comp = NULL;
				endpoint = &conf_desc->usb_interface[i].altsetting[j].endpoint[k];
				printf("       endpoint[%d].address: %02X\n", k, endpoint->bEndpointAddress);
				// Use the first interrupt or bulk IN/OUT endpoints as default for testing
				if ((endpoint->bmAttributes & LIBUSB_TRANSFER_TYPE_MASK) & (LIBUSB_TRANSFER_TYPE_BULK | LIBUSB_TRANSFER_TYPE_INTERRUPT))
				{
					if (endpoint->bEndpointAddress & LIBUSB_ENDPOINT_IN)
					{
						if (!endpoint_in)
							endpoint_in = endpoint->bEndpointAddress;
					}
					else
					{
						if (!endpoint_out)
							endpoint_out = endpoint->bEndpointAddress;
					}
				}
				printf("           max packet size: %04X\n", endpoint->wMaxPacketSize);
				printf("          polling interval: %02X\n", endpoint->bInterval);
				libusb_get_ss_endpoint_companion_descriptor(NULL, endpoint, &ep_comp);
				if (ep_comp)
				{
					printf("                 max burst: %02X   (USB 3.0)\n", ep_comp->bMaxBurst);
					printf("        bytes per interval: %04X (USB 3.0)\n", ep_comp->wBytesPerInterval);
					libusb_free_ss_endpoint_companion_descriptor(ep_comp);
				}
			}
		}
	}
	libusb_free_config_descriptor(conf_desc);

	libusb_set_auto_detach_kernel_driver(handle, 1);
	for (iface = 0; iface < nb_ifaces; iface++)
	{
		printf("\nClaiming interface %d...\n", iface);
		r = libusb_claim_interface(handle, iface);
		if (r != LIBUSB_SUCCESS)
		{
			perr("   Failed.\n");
		}
	}

	printf("\nReading string descriptors:\n");
	for (i = 0; i<3; i++)
	{
		if (string_index[i] == 0)
		{
			continue;
		}
		if (libusb_get_string_descriptor_ascii(handle, string_index[i], (unsigned char*)string, sizeof(string)) > 0)
		{
			printf("   String (0x%02X): \"%s\"\n", string_index[i], string);
		}
	}
	// Read the OS String Descriptor
	r = libusb_get_string_descriptor(handle, MS_OS_DESC_STRING_INDEX, 0, (unsigned char*)string, MS_OS_DESC_STRING_LENGTH);
	if (r == MS_OS_DESC_STRING_LENGTH && memcmp(ms_os_desc_string, string, sizeof(ms_os_desc_string)) == 0)
	{
		// If this is a Microsoft OS String Descriptor,
		// attempt to read the WinUSB extended Feature Descriptors
		read_ms_winsub_feature_descriptors(handle, string[MS_OS_DESC_VENDOR_CODE_OFFSET], first_iface);
	}

	switch (test_mode)
	{
	case USE_PS3:
		CALL_CHECK_CLOSE(display_ps3_status(handle), handle);
		break;
	case USE_XBOX:
		CALL_CHECK_CLOSE(display_xbox_status(handle), handle);
		CALL_CHECK_CLOSE(set_xbox_actuators(handle, 128, 222), handle);
		msleep(2000);
		CALL_CHECK_CLOSE(set_xbox_actuators(handle, 0, 0), handle);
		break;
	case USE_HID:
		test_hid(handle, endpoint_in);
		break;
	case USE_SCSI:
		CALL_CHECK_CLOSE(test_mass_storage(handle, endpoint_in, endpoint_out), handle);
	case USE_GENERIC:
		break;
	}

	printf("\n");
	for (iface = 0; iface<nb_ifaces; iface++)
	{
		printf("Releasing interface %d...\n", iface);
		libusb_release_interface(handle, iface);
	}

	printf("Closing device...\n");
	libusb_close(handle);

	return 0;
}
static void print_devs(libusb_device **devs)
{
    libusb_device *dev;
    int i = 0;
	uint8_t path[8];

    while ((dev = devs[i++]) != NULL)
    {
        struct libusb_device_descriptor desc;
        int r = libusb_get_device_descriptor(dev, &desc);
        if (r < 0)
        {
            fprintf(stderr, "failed to get device descriptor");
            return;
        }
		printf("%04x:%04x (bus %d, device %d)\n",
			desc.idVendor, desc.idProduct,
			libusb_get_bus_number(dev), libusb_get_device_address(dev));
		printdevice(desc.idVendor, desc.idProduct);
		printf("\n\n\n");
    }
}

int mainlist(void)
{
    libusb_device **devs;
    int r;
    ssize_t cnt;

    r = libusb_init(NULL);
    if (r < 0)
        return r;

    cnt = libusb_get_device_list(NULL, &devs);

    print_devs(devs);
    libusb_free_device_list(devs, 1);
    printf("-------------------mainusb-------------------------------\n");
    libusb_exit(NULL);
    mainlsusb();
    return 0;
}

//#ifdef _WIN32
//_CRTIMP FILE * __cdecl __iob_func(void);
//#ifdef __cplusplus
//extern "C"
//#endif
//FILE _iob[3] = { *stdin, *stdout,*stderr };
//#endif
