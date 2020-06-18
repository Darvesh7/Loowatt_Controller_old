/***************************************************************************************
* Name:   	LW_Attendant_Func.C
* 
* Revision: draft
*
* Author:  	Phil Hockley
*
* Description:
*		 	Control PCB functions.
*
* History: 	30/03/15	Initial code started
*			
****************************************************************************************/
//Define use of I2C for USB interface.
#USE I2C (MASTER,SLOW,I2C2)

void INIT_USART1 (void)
{
	RCSTA1 = 0;		//ensure completely reset
	SPBRG1 = 25;
	BAUD1CON = 0;
	RCSTA1 = 0b10000000;
	TXSTA1 = 0b00000100;
}

void INIT_USART2 (void)
{
	RCSTA2 = 0;		//ensure completely reset
	SPBRG2 = 25;	
	BAUD2CON = 0;
	RCSTA2 = 0b10000000;
	TXSTA2 = 0b00100100;
	DIR2 = DIR_TX;
}

void INIT_ALL (void)
{
	OSCCON = 0;	//external crystal...
	PORTA = 0;
	PORTB = 0;
	PORTC = 0;	
	PORTD = 0;
	PORTE = 0;
	PORTF = 0;
	PORTG = 0;
	TRISA = 0;
	TRISB = 0;
	TRISC = 0b10100000;
	TRISD = 0b01100000;
	TRISE = 0;
	TRISF = 0b00111110;
	TRISG = 0b00100100;		//PWM output must be disabled when set up, changed later so beware!!!
	ADCON0 = 0b00011101; 	//using AN7 only
	ADCON1 = 0b00110000;	//internal 4.096V reference
	ADCON2 = 0b10110101;	//16TAD, and Fosc/16
	ANCON0 = 0b10000000;	//Using AN7 only
	ANCON2 = 0;
	//Timer 1 set up
	T1CON = 0b00000110;		//overflows every 32mS or so
	WDTCON = 0b00011000;	//under FW control, and off!
	//set up required interrupts
	INTCON = 0;	//Don't allow global interrupts yet
	PIE1 = 0;
	PIE2 = 0;
	PIE3 = 0;
	PIE4 = 0;
	//set up SPI/I2C  interfaces
//	i2c_speed (100000);
	//i2c_init(TRUE); 
	T2CON = 0b01111111;
	PR2 = 159;				//20mS display update at 8MHz
	TMR2 = 0;
	FLASHING_ON
	INT_EN
	if(read_eeprom(no_of_units) > 10) write_eeprom(no_of_units,0);
}



int16 CHK_BATT (void)
{	//reads battery Voltage, and averages over 4 samples
	//throwing out the first reading. If below setpoint 
	//stores data in EEPROM.

	int8 i;
	int16 result;

	result = 0;
	for(i=0;i<5;i++)
	{
		delay_us(20);		//aq time
		GO_DONE = 1;		//start ADC
		while(GO_DONE);
		if(i != 0) result += ((int16)(ADRESH*256) + (int16)(ADRESL));
	}
	result = result/4;
	return result;
}

int1 chk_up (void)
{
	static int8 count,PB_released = 0;

	if(PB_UP)
	{
		count = 0;
		PB_released = 0;
	}
	else
	{
		if(PB_released == 0) count++;
	}
	if(count > debounce_time)
	{
		count = 0;
		PB_released = 1;
		return 1;
	}
	return 0;
}

int1 chk_dn (void)
{
	static int8 count,PB_released = 0;

	if(PB_DN)
	{
		count = 0;
		PB_released = 0;
	}
	else
	{
		if(PB_released == 0) count++;
	}
	if(count > debounce_time)
	{
		count = 0;
		PB_released = 1;
		return 1;
	}
	return 0;
}

int1 chk_ent (void)
{
	static int8 count,PB_released = 0;

	if(PB_ENT)
	{
		count = 0;
		PB_released = 0;
	}
	else
	{
		if(PB_released == 0) count++;
	}
	if(count > debounce_time)
	{
		count = 0;
		PB_released = 1;
		return 1;
	}
	return 0;

}


void read_RTC (void)
{
	extern TIME_STAMP;
	extern *pTIME_STAMP;

	pTIME_STAMP = &TIME_STAMP;	
	RTC_EN
	SEND_SPI(read_time_date);
	pTIME_STAMP->sec = SEND_SPI(0);
	pTIME_STAMP->min = SEND_SPI(0);
	pTIME_STAMP->hour = SEND_SPI(0);
	pTIME_STAMP->day = SEND_SPI(0);
	SEND_SPI(0);					//ignore this it is actually weekends
	pTIME_STAMP->month = SEND_SPI(0);	
	pTIME_STAMP->year = SEND_SPI(0);
	RTC_DIS
}

void write_RTC (void)
{
	extern TIME_STAMP;
	extern *pTIME_STAMP;
	
	pTIME_STAMP = &TIME_STAMP;	
	RTC_EN
	SEND_SPI(write_time_date);
	SEND_SPI(pTIME_STAMP->sec); 
	SEND_SPI(pTIME_STAMP->min); 
	SEND_SPI(pTIME_STAMP->hour); 
	SEND_SPI(pTIME_STAMP->day); 
	SEND_SPI(0); 					//we don't care about weekends
	SEND_SPI(pTIME_STAMP->month); 
	SEND_SPI(pTIME_STAMP->year); 
	RTC_DIS
}


int8 SEND_SPI (int8 spid_out)
{
	int8 i,spid_in;

	for(i=0;i<8;i++)
	{
		if(spid_out & BIT7) {SPI_SDO = 1;} else {SPI_SDO = 0;}
		SPI_SCK = 0;
		spid_out = spid_out << 1;
		SPI_SCK = 1;
		if(SPI_SDI == 1) 
		{
			spid_in |= BIT0;
		} 
		else 
		{
			spid_in &= ~BIT0;
		}
		if(i < 7) spid_in = spid_in << 1;
	}
	return spid_in;
}	

int1 write_to_usb (int8 USB_CMD, char *pUSB_DATA)
{
	int8 ACK;

	i2c_start();
	ACK = i2c_write(USB_I2C_ADDR);     // Device address
	if(ACK != 0) {i2c_stop(); return 0;}
	if(USB_CMD !=0xFF) ACK = i2c_write(USB_CMD);
	if(ACK != 0) {i2c_stop(); return 0;}
	do
	{
		ACK = i2c_write(*pUSB_DATA);
		if(ACK != 0) {i2c_stop(); return 0;}
		pUSB_DATA++;
	}while (*pUSB_DATA != 0);
	i2c_stop();
	return 1;
}

int8 USB_ready (void)
{	//if after 1000mS it has not found the USB then it exits.
	//looks in 20mS steps with a simple delay, so count = 50 attempts.
	int8 i,ACK;
	int8 ready_F = 0;

	for(i=0;i<50;i++)
	{
		delay_ms(20);
		i2c_start();
		ACK = i2c_write(USB_I2C_ADDR);     // Device address
		if(ACK != 0) {i2c_stop(); return 0;}
		ACK = i2c_write(I2C_FLASH_READY);
		if(ACK != 0) {i2c_stop(); return 0;}
		i2c_start();
		ACK = i2c_write(USB_I2C_ADDR | BIT0);     // Device address
		if(ACK != 0) {i2c_stop(); return 0;}
		ready_F = i2c_read(0);
		if(ACK != 0) {i2c_stop(); return 0;}
		i2c_stop();
		if(ready_F == 1) break;
	}
	return ready_F;
}

//converts int16 number in 0 to 999. Also removes leading zeros,
//add text "0" to the numbers. There is no NULL char at the end.
void convert_to_3digits (int16 INPUT, char *pOUTPUT)
{
	int8 zero_flag = 0;
	
	if(INPUT > 999) INPUT = 999;
	*pOUTPUT = '0';
	if(INPUT > 99)
	{
		zero_flag = 1;	
		while(INPUT >= 100)
		{
			(*pOUTPUT)++;
			INPUT = INPUT - 100;
		}
	}
	else
	{
		*pOUTPUT = ' ';
	}

	pOUTPUT++;

	*pOUTPUT = '0';
	if(INPUT > 9)
	{
		zero_flag = 1;	
		while(INPUT >= 10)
		{
			(*pOUTPUT)++;
			INPUT = INPUT - 10;
		}
	}
	if(zero_flag != 1)
	{	
		*pOUTPUT = ' ';
	}

	pOUTPUT++;
	*pOUTPUT = INPUT + '0';
}

int1 check_USB_TO (void)
{	//uses RTC to determine when the USB data needs to be updated
	//This is a fixed time only adjustable in this routine.
	//the calling function sets target to RESET_TO to reset the count

	extern int8 target_USB;
	static int8 seconds;
	int1 RV = 0;
	
	RTC_CE = 1;
	SPI_SCK = 0;
	RTC_CE = 1;
	SEND_SPI(read_time_date);
	seconds = SEND_SPI(0);
	RTC_CE = 0;
	seconds = convert_BCD_to_No(seconds,7);
	if(target_USB  == RESET_TO)
	{	//first time in loop so set up Target time
		if(seconds >= 30) {target_USB = seconds-30;} else {target_USB = seconds + 30;}
		RV = 0;		//no update yet
	}
	if(seconds == target_USB)
	{	//target reached, set new target
		if(seconds >= 30) {target_USB = seconds-30;} else {target_USB = seconds + 30;}
		RV = 1;
	}
	return RV;
}

int1 check_menu_TO (void)
{	//uses RTC to determine when the menu has timed out to be updated
	//This is a fixed time only adjustable in this routine.
	//the calling function sets target to RESET_TO to reset the count

	extern int8 target_MENU;
	int8 seconds;
	int1 RV = 0;

	RTC_CE = 1;
	SPI_SCK = 0;
	RTC_CE = 1;
	SEND_SPI(read_time_date);
	seconds = SEND_SPI(0);
	RTC_CE = 0;
	seconds = convert_BCD_to_No(seconds,7);

	if(target_MENU  == RESET_TO)
	{
		if(seconds >= 45) {target_MENU = seconds-45;} else {target_MENU = seconds + 15;}
		RV =  0;		//no update yet
	}
	if(seconds == target_MENU)
	{	//target reached, set new target
		if(seconds >= 45) {target_MENU = seconds-45;} else {target_MENU = seconds + 15;}
		RV = 1;
	}
	return RV;
}


int32 Convert_To_UNIX_TIME (void)
{
	extern struct TIME TIME_STAMP;
	extern struct TIME *pTIME_STAMP;
	int32 UNIX_TIME,DELTAyears;
	long GP_long;
	const long TAB_days[] = {0,31,59,90,120,151,181,212,243,273,304,334};


	read_RTC ();
	pTIME_STAMP = &TIME_STAMP;
	//first convert the time from BCD, and then muliply by constant

	//seconds so far for seconds gone
	UNIX_TIME = (int32)(pTIME_STAMP->sec & 0x0F) + (int32)(10*((pTIME_STAMP->sec >> 4) & 0x0F));
	//seconds so far for minutes gone
	UNIX_TIME += 60*((int32)(pTIME_STAMP->min & 0x0F) + (int32)(10*((pTIME_STAMP->min >> 4) & 0x0F)));
	//seconds so far for hours gone
	UNIX_TIME += 3600*((int32)(pTIME_STAMP->hour & 0x0F) + (int32)(10*((pTIME_STAMP->hour >> 4) & 0x0F)));
	//seconds so far for days gone (in the month)
	UNIX_TIME += 86400*(((int32)(pTIME_STAMP->day & 0x0F) + (int32)(10*((pTIME_STAMP->day >> 4) & 0x0F))) - 1);
	//so far OK, but now months have different number of days.. as below
	// 31, 28*,31,30,31,30,31,31,30,31,30,31. * ignore leap years for now, we will correct this later.
	GP_long = (pTIME_STAMP->month & 0x0F) + (10*(pTIME_STAMP->month >> 4) & 0x0F) - 1;
	GP_long = TAB_days[GP_long]; 	
	UNIX_TIME += 86400*(int32)GP_long;
	DELTAyears = (int32)(pTIME_STAMP->year & 0x0F) + (int32)(10*((pTIME_STAMP->year >> 4) & 0x0F)) + 30; //(+30 = 2000-1970)
	UNIX_TIME += 31536000*DELTAyears;
	//now compensate for for the number of leap years since 2016, we know there has been 12 already.
	GP_long = (pTIME_STAMP->year & 0x0F) + (10*(pTIME_STAMP->year >> 4)) - 16;
	GP_long = GP_long/4;
	if((pTIME_STAMP->month <= 2) &&  (gp_long != 0)) gp_long = gp_long - 1;	//Feb not happened yet...) 
	GP_long += 12;	//number of leap years since 1970 and 2016 which had gone when code written)
	UNIX_TIME += (int32)GP_long * 86400;

	return UNIX_TIME;
}






