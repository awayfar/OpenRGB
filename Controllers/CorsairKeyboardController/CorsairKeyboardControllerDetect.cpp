#include "CorsairKeyboardController.h"
#include "RGBController.h"
#include "RGBController_CorsairKeyboard.h"
#include <vector>
#include <hidapi/hidapi.h>

/*-----------------------------------------------------*\
| Corsair vendor ID                                     |
\*-----------------------------------------------------*/
#define CORSAIR_VID                     0x1B1C

/*-----------------------------------------------------*\
| Keyboard product IDs                                  |
|   List taken from ckb-next                            |
|   Non-RGB keyboards were omitted from this list       |
\*-----------------------------------------------------*/
#define CORSAIR_K55_RGB_PID             0x1B3D

#define CORSAIR_K65_RGB_PID             0x1B17
#define CORSAIR_K65_LUX_RGB_PID         0x1B37
#define CORSAIR_K65_RGB_RAPIDFIRE_PID   0x1B39

#define CORSAIR_K68_RGB                 0x1B4F

#define CORSAIR_K70_RGB_PID             0x1B13
#define CORSAIR_K70_LUX_RGB_PID         0x1B33
#define CORSAIR_K70_RGB_RAPIDFIRE_PID   0x1B38
#define CORSAIR_K70_RGB_MK2_PID         0x1B38
#define CORSAIR_K70_RGB_MK2_SE_PID      0x1B6B
#define CORSAIR_K70_RGB_MK2_LP_PID      0x1B55

#define CORSAIR_K95_RGB_PID             0x1B11
#define CORSAIR_K95_PLATINUM_PID        0x1B2D

#define CORSAIR_STRAFE_PID              0x1B20
#define CORSAIR_STRAFE_MK2_PID          0x1B48

/*-----------------------------------------------------*\
| Mouse product IDs                                     |
|   List taken from ckb-next                            |
\*-----------------------------------------------------*/

/*-----------------------------------------------------*\
| Mousepad product IDs                                  |
|   List taken from ckb-next                            |
\*-----------------------------------------------------*/
#define CORSAIR_MM800_RGB_POLARIS_PID   0x1B3B

typedef struct
{
    unsigned short  usb_vid;
    unsigned short  usb_pid;
    unsigned char   usb_interface;
    const char *    name;
} corsair_node_device;

static const corsair_node_device device_list[3] =
{
    { CORSAIR_VID,          CORSAIR_K70_RGB_PID,                1,      "Corsair K70 RGB"           },
    { CORSAIR_VID,          CORSAIR_K95_RGB_PID,                1,      "Corsair K95 RGB"           },
    { CORSAIR_VID,          CORSAIR_MM800_RGB_POLARIS_PID,      0,      "Corsair MM800 RGB Polaris" }
};

/******************************************************************************************\
*                                                                                          *
*   DetectCorsairKeyboardControllers                                                       *
*                                                                                          *
*       Tests the USB address to see if a Corsair RGB Keyboard controller exists there.    *
*                                                                                          *
\******************************************************************************************/

void DetectCorsairKeyboardControllers(std::vector<RGBController*>& rgb_controllers)
{
    hid_device_info* info;
    hid_device* dev;

    hid_init();

    for(int device_idx = 0; device_idx < 3; device_idx++)
    {
        dev = NULL;

        info = hid_enumerate(device_list[device_idx].usb_vid, device_list[device_idx].usb_pid);

        //Look for Corsair RGB Keyboard, interface 1
        while(info)
        {
            if((info->vendor_id == device_list[device_idx].usb_vid)
            &&(info->product_id == device_list[device_idx].usb_pid)
            &&(info->interface_number == device_list[device_idx].usb_interface))
            {
                dev = hid_open_path(info->path);
                break;
            }
            else
            {
                info = info->next;
            }
        }

        if( dev )
        {
            CorsairKeyboardController* controller = new CorsairKeyboardController(dev);

            if(controller->GetDeviceType() != DEVICE_TYPE_UNKNOWN)
            {
                RGBController_CorsairKeyboard* rgb_controller = new RGBController_CorsairKeyboard(controller);

                rgb_controller->name = device_list[device_idx].name;
                
                rgb_controllers.push_back(rgb_controller);
            }
        }
    }
}
