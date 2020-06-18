/***************************************************************************************
* Name:   	LW_Attendant_USB.C
* 
* Revision: draft
*
* Author:  	Phil Hockley
*
* Description:
*		 	Control PCB functions.
*
* History: 	22/06/15	Initial code started
*			
****************************************************************************************/
int1 initalise_USB_files(int8 trailer_ID)
{	//creates file headers for all possible units
	extern int8 no_of_loos;
	int8 loo;
	char I2C_TEXT[40], CAT_TEXT[15];
 
	for(loo =1; loo <= no_of_loos; loo++)
	{	
		//create string to set the file we want to append to
		I2C_TEXT = "LooW";
		if(loo < 10) CAT_TEXT[0] = '0'+loo; CAT_TEXT[1] = 0;
		if(loo == 10) CAT_TEXT = "10";
		strcat(I2C_TEXT,CAT_TEXT);
		CAT_TEXT = ".TXT";
		strcat(I2C_TEXT,CAT_TEXT);
		write_to_usb(I2C_FILENAME,I2C_TEXT);
		if(!USB_ready()) return 0;		//cancel USB as it is not working.

		//now for the file header e.g. TRAILER A: UNIT 1:
		I2C_TEXT = "TRAILER:";
		CAT_TEXT[0] = trailer_ID;
		CAT_TEXT[1] = 0;
		strcat(I2C_TEXT,CAT_TEXT);
		CAT_TEXT = " UNIT: ";
		strcat(I2C_TEXT,CAT_TEXT);
		if(loo < 10) CAT_TEXT[0] = '0'+loo;CAT_TEXT[1] = 0;
		if(loo == 10) CAT_TEXT = "10";
		strcat(I2C_TEXT,CAT_TEXT);
		CAT_TEXT = "\r\n";
		strcat(I2C_TEXT,CAT_TEXT);
		write_to_usb(I2C_APPEND,I2C_TEXT);
		if(!USB_ready()) return 0;		//cancel USB as it is not working.

		//now the log data header = 
		I2C_TEXT = "DATE,TIME,MANUAL FWD,MANUAL BACK,";
		write_to_usb(I2C_APPEND,I2C_TEXT);
		if(!USB_ready()) return 0;		//cancel USB as it is not working.

		I2C_TEXT = "MAIN FLUSH,MAIN BAG USED,SEC FLUSH,";
		write_to_usb(I2C_APPEND,I2C_TEXT);
		if(!USB_ready()) return 0;		//cancel USB as it is not working.

		I2C_TEXT = "SEC BAG USED,BAG USED,BAG LEFT,";
		write_to_usb(I2C_APPEND,I2C_TEXT);
		if(!USB_ready()) return 0;		//cancel USB as it is not working.

		I2C_TEXT = "NO BLOCKS,NO RESETS,MODE\r\n";
		write_to_usb(I2C_APPEND,I2C_TEXT);
		if(!USB_ready()) return 0;		//cancel USB as it is not working.
	}
}

int1 send_log_data_to_USB (void)
{	//Formatt, and send all data to USB flash memory.

	extern union LOG_UNION DATA_IN[Max_no_LOOS];
	extern union LOG_UNION *pDATA_IN;
	extern struct TIME TIME_STAMP;
	extern struct TIME *pTIME_STAMP;
	extern int8 no_of_loos;

	int8 loo;
	int16 number;

	char I2C_TEXT[20], CAT_TEXT[10];

	pTIME_STAMP = &TIME_STAMP;
	read_RTC();
	for(loo = 1; loo < no_of_loos+1; loo++)		//This may look weird, but don't think about changing it!!
	{
		pDATA_IN = &DATA_IN[loo-1];		

		I2C_TEXT = "LooW";
		if(loo < 10) CAT_TEXT[0] = '0'+loo; CAT_TEXT[1] = 0;
		if(loo == 10) 
		{
			CAT_TEXT = "10";
		}
		strcat(I2C_TEXT,CAT_TEXT);
		CAT_TEXT = ".TXT";
		strcat(I2C_TEXT,CAT_TEXT);
		write_to_usb(I2C_FILENAME,I2C_TEXT);
		if(!USB_ready()) return 0;		//cancel USB as it is not working.

		//First Date
		I2C_TEXT = "";
		CAT_TEXT = "";
		date_to_string (pTIME_STAMP,I2C_TEXT);
		CAT_TEXT = ",";
		strcat(I2C_TEXT,CAT_TEXT);
		write_to_usb(I2C_APPEND,I2C_TEXT);
		if(!USB_ready()) return 0;		//cancel USB as it is not working.
		time_to_string (pTIME_STAMP,I2C_TEXT);
		CAT_TEXT = ",";
		strcat(I2C_TEXT,CAT_TEXT);
		write_to_usb(I2C_APPEND,I2C_TEXT);
		if(!USB_ready()) return 0;		//cancel USB as it is not working.
		I2C_TEXT = "";
		number = pDATA_IN->MFB_USED/10;
		number_to_string (number,I2C_TEXT);
		CAT_TEXT = ",";
		strcat(I2C_TEXT,CAT_TEXT);
		number = pDATA_IN->MBB_USED/10;
		number_to_string (number,CAT_TEXT);
		strcat(I2C_TEXT,CAT_TEXT);
		CAT_TEXT = ",";
		strcat(I2C_TEXT,CAT_TEXT);
		number = pDATA_IN->NOI_FLUSH;
		number_to_string (number,CAT_TEXT);
		strcat(I2C_TEXT,CAT_TEXT);
		CAT_TEXT = ",";
		strcat(I2C_TEXT,CAT_TEXT);
		write_to_usb(I2C_APPEND,I2C_TEXT);
		if(!USB_ready()) return 0;		//cancel USB as it is not working.

		I2C_TEXT = "";
		number = pDATA_IN->NOIB_USED/10;
		number_to_string (number,I2C_TEXT);
		CAT_TEXT = ",";
		strcat(I2C_TEXT,CAT_TEXT);
		number = pDATA_IN->NOS_FLUSH;
		number_to_string (number,CAT_TEXT);
		strcat(I2C_TEXT,CAT_TEXT);
		CAT_TEXT = ",";
		strcat(I2C_TEXT,CAT_TEXT);
		number = pDATA_IN->NOSB_USED/10;
		number_to_string (number,CAT_TEXT);
		strcat(I2C_TEXT,CAT_TEXT);
		CAT_TEXT = ",";
		strcat(I2C_TEXT,CAT_TEXT);
		write_to_usb(I2C_APPEND,I2C_TEXT);
		if(!USB_ready()) return 0;		//cancel USB as it is not working.

		I2C_TEXT = "";
		number = pDATA_IN->BAG_USED/10;
		number_to_string (number,I2C_TEXT);
		CAT_TEXT = ",";
		strcat(I2C_TEXT,CAT_TEXT);
		number = pDATA_IN->BAG_LEFT/10;
		number_to_string (number,CAT_TEXT);
		strcat(I2C_TEXT,CAT_TEXT);
		CAT_TEXT = ",";
		strcat(I2C_TEXT,CAT_TEXT);
		number = pDATA_IN->NO_BLOCKS;
		number_to_string (number,CAT_TEXT);
		strcat(I2C_TEXT,CAT_TEXT);
		CAT_TEXT = ",";
		strcat(I2C_TEXT,CAT_TEXT);
		write_to_usb(I2C_APPEND,I2C_TEXT);
		if(!USB_ready()) return 0;		//cancel USB as it is not working.

		I2C_TEXT = "";
		number = ((pDATA_IN->LooSTATUS/256));
		number_to_string (number,I2C_TEXT);
		CAT_TEXT = ",";
		strcat(I2C_TEXT,CAT_TEXT);
		if((pDATA_IN->LooSTATUS & RUNSTATE) == RUNSTATE)
		{
			CAT_TEXT = "RUN\r\n";
		}
		else if ((pDATA_IN->LooSTATUS & SERSTATE) == SERSTATE)
		{	
			CAT_TEXT = "SRV\r\n";
		}
		else
		{
			CAT_TEXT = "OFF\r\n";
		}
		strcat(I2C_TEXT,CAT_TEXT);
		write_to_usb(I2C_APPEND,I2C_TEXT);
		if(!USB_ready()) return 0;		//cancel USB as it is not working.
	}	
	return 1;
}	



void number_to_string (int16 number, *string_out)
{	//does not remove leading zeros,adds NULL to end.

	int8 start_flag = 0;

	if(number > 9999) number = 9999;

	*string_out = 48;	//character 0.
	if(number > 999 )
	{
		while(number >= 1000)
		{
			(*string_out)++;
			number = number - 1000;
		}
		start_flag = 1;
		string_out++;
	}

	*string_out = 48;	//character 0.
	if((number > 99) || start_flag)
	{
		while(number >= 100)
		{
			(*string_out)++;
			number = number - 100;
		}
		string_out++;
	}
	else if(start_flag)
	{
		string_out++;
	}

	*string_out = 48;	//character 0.
	if(number > 9 )
	{
		while(number >= 10)
		{
			(*string_out)++;
			number = number - 10;
		}
	}
	string_out++;
	*string_out = 48 + number;
	string_out++;
	*string_out = 0;
}

void date_to_string (struct TIME *pTIME, char *string)
{
	*string = '2';	//start off string as year (2016 to 2065)	
	string++;
	*string = '0';
	string++;
	*string = (pTIME->year >> 4)+'0';
	string++;
	*string = (pTIME->year & 0x0F)+'0';
	string++;
	*string = '-';
	string++;
	*string = (pTIME->month >> 4)+'0';
	string++;
	*string = (pTIME->month & 0x0F)+'0';
	string++;
	*string = '-';
	string++;
	*string = (pTIME->day >> 4)+'0';
	string++;
	*string = (pTIME->day & 0x0F)+'0';
	string++;
	*string = 0;
}

void time_to_string (struct TIME *pTIME, char *string)
{	//load Time header to string
	(*string) = (pTIME->hour >> 4)+'0';
	string++;
	(*string) = (pTIME->hour & 0x0F)+'0';
	string++;
	*string = '-';
	string++;
	(*string) = (pTIME->min >> 4)+'0';
	string++;
	(*string) = (pTIME->min & 0x0F)+'0';
	string++;
	*string = '-';
	string++;
	(*string) = (pTIME->sec >> 4)+'0';
	string++;
	(*string) = (pTIME->sec & 0x0F)+'0';
	string++;
	(*string) = 0;

}


