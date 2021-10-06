# Zephyr on Teensy4.x

This is a litte application using Zephyr RTOS on Teensy 4.x hardware. It aims to test various hardware peripherals and kernel functions. Maybe it can provide a starting point for real world applications.

Every thread has a start delay of 500ms. This ensures that the device is properly listed on the usb host and comport is open before the log output runs in. 

## Standard Output
Standard output is mapped to usb according to [usb/console](https://github.com/zephyrproject-rtos/zephyr/tree/master/samples/subsys/usb/console) example. This is done in ```main.c```. As described there the board will be detected as a CDC_ACM serial device. To see the console output, use a command similar to ```minicom -D /dev/ttyACM0```.

## LED + toggle pin
The led flashes continously to give an visual indication that application is running. Toggle pin is mapped to teensy pin 41 (Teensy 4.1) or pin 2 (Teensy 4.0).

## Button
On every edge the button pushes a string to a message queue which is then processed by the uart. Pushputton is mapped to teensy pin 23. An external pullup resistor must be used currently.

## UART
The uart always listens for incoming data and dumps it to the log. It also listens for data in the message queue, which are sent out immediately. Used uart is mapped to teens pins 7/8 (Rx2/TX2 in teensy numbering, lpuart4 in nxp numbering). Rx/Tx shorted externally.

## SD card
USDHC hardware on Teensy 4.1 is working but has some limitations shown below. Example prints simple file listing of the card according to [samples/subsys/fs/fat_fs](https://github.com/zephyrproject-rtos/zephyr/tree/master/samples/subsys/fs/fat_fs) example.

At the time of writing the SD card usage seems not really stable.

### Card voltage switch
Teensy 4.1 has no hardware to switch card voltage. As a workaround in the file ```zephyr/drivers/disk/usdhc.c``` the line 502 needs to be changed:

```#define SDMMCHOST_NOT_SUPPORT USDHC_HOST_CTRL_CAP_VS18(1)```

This prevents the driver from trying to switch over to 1,8V supply for the card. Also a dummy pin is defined in board defs. 