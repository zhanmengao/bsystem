#include <stdio.h>
#include <libusb-1.0/libusb.h>
#ifdef __linux
#include <unistd.h>
#else
#include<Windows.h>
#pragma comment(lib,"libusb-1.0.lib")
#pragma comment(lib,"legacy_stdio_definitions.lib")

_CRTIMP FILE * __cdecl __iob_func(void);
#ifdef __cplusplus
extern "C"
#endif
FILE _iob[3] = { *stdin, *stdout,*stderr};

#endif
// First, use "lsusb" see vid and pid.
// there is my printer(hp deskjet 1010) vid and pid.
#define VID 0x03f0
#define PID 0xb511
static int device_satus(libusb_device_handle *hd)
{
    int interface = 0;
    unsigned char byte;
    libusb_control_transfer(hd, LIBUSB_ENDPOINT_IN | LIBUSB_REQUEST_TYPE_CLASS | LIBUSB_RECIPIENT_INTERFACE,
                            LIBUSB_REQUEST_CLEAR_FEATURE,   0,   interface,   &byte, 1, 5000);
    printf("status:0x%x\n", byte);/** * byte * normal:0x18 * other :0x10 */
    return 0;
}
int mainStatus()
{
    libusb_device **devs;                               //pointer to pointer of device, used to retrieve a list of devices
    libusb_device_handle *dev_handle;                       //a device handle
    libusb_context *ctx = NULL;                         //a libusb session
    int r;                                              //for return values
    ssize_t cnt;                                        //holding number of devices in list
    r = libusb_init(&ctx);                              //initialize the library for the session we just declared
    if(r < 0)
    {
        perror("Init Error\n");                         //there was an error
        return 1;
    }
    libusb_set_debug(ctx, LIBUSB_LOG_LEVEL_INFO); //set verbosity level to 3, as suggested in the documentation
    cnt = libusb_get_device_list(ctx, &devs); //get the list of devices
    if(cnt < 0)
    {
        perror("Get Device Error\n"); //there was an error
        return 1;
    }
    printf("%u Devices in list.\n", cnt);
    dev_handle = libusb_open_device_with_vid_pid(ctx, VID, PID); //these are vendorID and productID I found for my usb device
    if(dev_handle == NULL)
        perror("Cannot open device\n");
    else  printf("Device Opened\n");
    libusb_free_device_list(devs, 1); //free the list, unref the devices in it
    if(libusb_kernel_driver_active(dev_handle, 0) == 1)
    {
        //find out if kernel driver is attached
        printf("Kernel Driver Active\n");
        if(libusb_detach_kernel_driver(dev_handle, 0) == 0) //detach it
            printf("Kernel Driver Detached!\n"); }
    r = libusb_claim_interface(dev_handle, 0); //claim interface 0 (the first) of device (mine had jsut 1)
    if(r < 0)
    {
        perror("Cannot Claim Interface\n");
        return 1;
    }
    printf("Claimed Interface\n");
    device_satus(dev_handle);
    libusb_close(dev_handle); //close the device we opened
    libusb_exit(ctx); //needs to be called to end the
    return 0;
}