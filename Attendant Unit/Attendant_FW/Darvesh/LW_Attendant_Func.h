/***************************************************************************************
* Name:   	LW_Controller_Func.H
* 
* Revision: draft
*
* Author:  	Phil Hockley
*
* Description:
*		 	prototype header file.
*
* History: 	30/03/15	Initial code started
*			
****************************************************************************************/
void INIT_USART1 (void);
void INIT_USART2 (void);
void INIT_ALL (void);
int16 CHK_BATT (void);
int1 chk_up (void);
int1 chk_dn (void);
int1 chk_ent (void);
void read_RTC (void);
void write_RTC (void);
int8 SEND_SPI (int8 spid_out);
int8 write_to_usb (int8 USB_CMD, char *pUSB_DATA);
int8 USB_ready (void);
void convert_to_3digits (int16 INPUT, char *pOUTPUT);
long long Convert_To_UNIX_TIME (void);

#define debounce_time 2
#define USB_I2C_ADDR 82	//actuallly 41, but I2C addr is top 7 bits
#define I2C_FILENAME 0
#define IC2_WRITE 1
#define I2C_APPEND 2
#define I2C_READB 3
#define I2C_READ_SETLINE 4
#define I2C_FLASH_READY 99

//RTC defines
#define read_time_date 0b10001000
#define write_time_date 0b00001000

//Misc
#define RESET_TO 0xFF