/**
 * @file main.c
 * @author Bernhard Kraemer
 * @brief Application startup
 * @version 0.1
 * @date 2020-11-15
 * 
 * @copyright Copyright (c) 2020
 * 
 */

/*** Imported Objects ***************************************************************/
#include <zephyr.h>
#include <logging/log.h>

#include <usb/usb_device.h>

/*** Constants **********************************************************************/
#define LOG_MODULE_NAME sys

#define START_DELAY 400

/*** Types **************************************************************************/

/*** Variables **********************************************************************/

/*** Function Prototypes ************************************************************/
void main(void);

/*** Macros *************************************************************************/
LOG_MODULE_REGISTER(LOG_MODULE_NAME, LOG_LEVEL_DBG);

BUILD_ASSERT(DT_NODE_HAS_COMPAT(DT_CHOSEN(zephyr_console), zephyr_cdc_acm_uart),
	     "Console device is not ACM CDC UART device");

/*** Function implementation ********************************************************/

void main(void)
{
	uint32_t version = sys_kernel_version_get();

	if (usb_enable(NULL)) {
		return;
	}

	/* Sleep some time after boot to give USB-Client a chance to connect 
	 * before first log output is produced. 
	 */
	k_msleep(START_DELAY);

	LOG_INF("Started zephyr %u.%u.%u on board %s/%s.", SYS_KERNEL_VER_MAJOR(version), SYS_KERNEL_VER_MINOR(version), SYS_KERNEL_VER_PATCHLEVEL(version), CONFIG_ARCH, CONFIG_BOARD);
}
