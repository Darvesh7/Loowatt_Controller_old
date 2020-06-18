/****************************************************************************
* File Name: LooWatt Attendant_Menu
* Purpose:
*		MAIN application program.         
* Parameters Passed: 
* Parameters Returned: 
* Globals:
* Date Started 	28/02/2016	
* Version		Not released
****************************************************************************/  
void display_USB_warning (int8 action)
{
	extern int8 FLASH_REG[2];
	extern char display_ram[6][21];
	static int8 FLSH_STR;
	static char disp_ram_str[11];
	char dtext[11];
	int8 i;

	if(action == disp_warning)
	{	//back up existing message, and flash regs
		FLSH_STR = FLASH_REG[0];
		FLASH_REG[0] &= ~BIT0;
		dtext = "USB ACTIVE";
		for(i=0;i<11;i++)
		{
			disp_ram_str[i] = display_ram[0][(i+line1_flash_A-1)];
			display_ram[0][(i+line1_flash_A-1)] = dtext[i];
		}	
	}
	else
	{
		FLASH_REG[0] = FLSH_STR;
		for(i=0;i<11;i++)
		{
			display_ram[0][i+line1_flash_A-1] = disp_ram_str[i];
		}	
	}
}


int1 set_init_mess (void)
{
	extern int8 FLASH_REG[2];
	extern int8 ALARM_FLAG;
	extern int8 FLASH_FLAG;
	extern int8 target_USB;
	char text[21];

	FLASH_REG[0] = 0;
	FLASH_REG[1] = 0;
	ALARM_FLAG = 0;
	FLASH_FLAG = 0;
	target_USB = 0xFF;

	while(NO_USB)
	{
		text = "  USB FLASH DRIVE   ";
		strcpy(&display_ram[0], &text);
		text = "MISSING - INSERT USB";
		strcpy(&display_ram[1] ,&text);
		text = "DEVICE AND PRESS ENT";
		strcpy(&display_ram[2], &text);
		text = "PRESS UP FOR NO USB ";
		strcpy(&display_ram[3], &text);
		while(!chk_ent())
		{
			while(tick == 0);
			tick = 0;
			if(check_menu_TO()) return 0;
			if(chk_up()) return 0;
		}
		target_MENU = RESET_TO;
		reset_disp_ram();
		delay_ms(1000);
	}
	text = "                    ";
	strcpy(&display_ram[0], &text);
	text = "    INITIALISING    ";
	strcpy(&display_ram[1] ,&text);
	text = "     USB DEVICE     ";
	strcpy(&display_ram[2], &text);
	text = "                    ";
	strcpy(&display_ram[3], &text);
}


void set_top_menu (void)
{
	char text[21];

	text = "Set Trailer ID      ";
	strcpy(&display_ram[0], &text);
	text = "S/R Unit Addresses  ";
	strcpy(&display_ram[1] ,&text);
	text = "Set Time and Date   ";
	strcpy(&display_ram[2], &text);
	text = "Exit                ";
	strcpy(&display_ram[3], &text);
}

void reset_disp_ram (void)
{
	char text[21];

	text = "                    ";
	strcpy(&display_ram[0], &text);
	strcpy(&display_ram[1], &text);
	strcpy(&display_ram[2], &text);
	strcpy(&display_ram[3], &text);
	strcpy(&display_ram[4], &text);
	strcpy(&display_ram[5], &text);
}

void set_reset_addr_menu (void)
{
	char text[21];

	text = "Set Unit Addresses  ";
	strcpy(&display_ram[0], &text);
	text = "Reset All addresses ";
	strcpy(&display_ram[1], &text);
	text = "                    ";
	strcpy(&display_ram[2], &text);
	text = "Exit                ";
	strcpy(&display_ram[3], &text);
}

void reset_all_addr_menu (void)
{
	char text[21];

	reset_disp_ram(); 
	text = "!ABOUT TO RESET ALL!";
	strcpy(&display_ram[0], &text);
	text = "   ARE YOU SURE??   ";
	strcpy(&display_ram[1], &text);
	text = " Press ENT to abort ";
	strcpy(&display_ram[2], &text);
	text = " Press UP reset ALL ";
	strcpy(&display_ram[3], &text);
}

void menu_sys (void)
{
	extern int8 display_type;
	extern int8 FLASH_REG[2];
	extern int8 scroll_offset;
	extern int8 tick;

	FLASH_REG[0] = BIT0;

	TRAILER_ID = read_eeprom(trailer_addr);
	display_type = MENU;
	scroll_offset = 0;
	set_top_menu();
	target_MENU = RESET_TO;
	for( ; ; )
	{
		while(tick == 0);
		tick = 0;
		if(check_menu_TO()) break;
		if(chk_up()) 
		{
			target_MENU = RESET_TO;
			FLASH_REG[0] = FLASH_REG[0] >>1;
			if(FLASH_REG[0] == 0 ) FLASH_REG[0] = BIT3;
		}
		if(chk_dn()) 
		{
			target_MENU = RESET_TO;
			FLASH_REG[0] = FLASH_REG[0] << 1;
			if(FLASH_REG[0] & BIT4 ) FLASH_REG[0] = BIT0;
		}
		if(chk_ent())
		{	//if exit selected then done.
			target_MENU = RESET_TO;
			if(FLASH_REG[0] & BIT3) break;
			if(FLASH_REG[0] & BIT0) set_trailer_id();
			if(FLASH_REG[0] & BIT1) set_rst_units();
			if(FLASH_REG[0] & BIT2) set_time_date();
			set_top_menu();
			target_MENU = RESET_TO;
		}
	}
	reset_disp_ram();
}

void set_trailer_id (void)
{
	extern int8 FLASH_REG[2];
	extern int8 scroll_offset;
	extern char display_ram[6][21];
	extern int8 tick;
	extern char TRAILER_ID;
	char text[21];

	FLASH_REG[0] = 0;
	scroll_offset = 0;

	TRAILER_ID = read_eeprom(trailer_addr);
	display_ram[2][0] = "                   ";
	if(TRAILER_ID < 'A') TRAILER_ID = 'Z';
	if(TRAILER_ID > 'Z') TRAILER_ID = 'A';
	display_ram[2][ID_POS] = TRAILER_ID;       
	
	for( ; ; )
	{
		while(tick == 0);
		tick = 0;
		text = "Set Trailer ID      ";
		strcpy(&display_ram[0], &text);
		text = "Use Up/Down to sel  ";
		strcpy(&display_ram[1], &text);
		text = "                    ";
		strcpy(&display_ram[2], &text);
		text = "Press Ent when done ";
		strcpy(&display_ram[3], &text);

		if(chk_up()) 
		{
			TRAILER_ID++;
			if(TRAILER_ID > 'Z') TRAILER_ID = 'A';
		}
		if(chk_dn()) 
		{
			TRAILER_ID--;
			if(TRAILER_ID < 'A') TRAILER_ID = 'Z';
		}
		display_ram[2][ID_POS] = TRAILER_ID;
		if(chk_ent())
		{	//if exit selected then done.
			
			set_top_menu();
			write_eeprom (trailer_addr,TRAILER_ID);
			break;
		}
	}
	FLASH_REG[0] = BIT0;
}

void set_rst_units (void)
{
	extern int8 FLASH_REG[2];
	extern int8 scroll_offset;
	extern int8 tick;

	FLASH_REG[0] = BIT0;
	scroll_offset = 0;

	//Now we need to find out how many units have been addressed already
	set_reset_addr_menu();	
	for( ; ; )
	{
		while(tick == 0);
		tick = 0;
		if(chk_up()) 
		{
			FLASH_REG[0] = FLASH_REG[0] >>1;
			if(FLASH_REG[0] == 0 ) FLASH_REG[0] = BIT3;
			if(FLASH_REG[0] & BIT2) FLASH_REG[0] = BIT1;
		}
		if(chk_dn()) 
		{
			FLASH_REG[0] = FLASH_REG[0] << 1;
			if(FLASH_REG[0] & BIT4 ) FLASH_REG[0] = BIT0;
			if(FLASH_REG[0] & BIT2) FLASH_REG[0] = BIT3;
		}
		if(chk_ent())
		{	//if exit selected then done.
			if(FLASH_REG[0] & BIT3) break;
			if(FLASH_REG[0] & BIT0) set_unit_addrs();
			if(FLASH_REG[0] & BIT1) rst_unit_addrs();
			set_reset_addr_menu();
			FLASH_REG[0] = BIT0;
		}
	}	
	set_top_menu();
}

void set_unit_addrs (void)
{
	extern int8 FLASH_REG[2];
	extern int8 scroll_offset;
	extern char display_ram[6][21];
	extern int8 tick;
	int8 next_address, number1,number2;
	char text[21];
	int1 OK_flag;

	FLASH_REG[0] = 0;
	scroll_offset = 0;
	reset_disp_ram();

	//Now we need to find out how many units have been addressed already
	next_address = read_eeprom(no_of_units)+1;
	if(next_address > 6 )
	{						 
		text = "MAX ADDRESS REACHED ";
		strcpy(&display_ram[1], &text);
		delay_ms(3000);
	}
	else
	{
		for( ; ; )
		{

			while(tick == 0);
			tick = 0;
			if(next_address < 10) 
			{
				number2 = next_address + '0';
				number1 = ' ';
			}
			else
			{
				number1 = '1';
				number2 = '0';
			}
			text = "Set Unit Address    ";
			strcpy(&display_ram[0], &text);
			display_ram[0][18] = number1;
			display_ram[0][19] = number2;
			text = "Press Enter to Exit ";
			strcpy(&display_ram[2], &text);
			text = "Press UP to set addr";
			strcpy(&display_ram[3], &text);
			if(chk_ent())
			{	//if exit selected then done.
				write_eeprom(no_of_units,(next_address-1));
				break;		
			}
			if(chk_up()) 
			{
				FLASHING_OFF
				OK_flag = send_RS485 (SET_ADR,next_address);
				if(!OK_flag)
				{     
					FLASHING_ON      
					text = "                    ";
					strcpy(&display_ram[2], &text);
					text = " !OPERATION FAILED! ";
					strcpy(&display_ram[3], &text);
					write_eeprom(no_of_units,(next_address-1));
					delay_ms(3000);
					break;
				}
				else
				{	 
					FLASHING_ON       
					text = "                   ";
					strcpy(&display_ram[2], &text);
					text = "   ADDRESS SET OK   ";
					strcpy(&display_ram[3], &text);
					delay_ms(3000);
					next_address++;
					if(next_address >10 )
					{						 
						text = "MAX ADDRESS REACHED!";
						strcpy(&display_ram[1], &text);
						delay_ms(3000);
					}
				}
			}
		}
	}
}		

int8 rst_unit_addrs (void)
{
	extern int8 FLASH_REG[2];
	extern int8 scroll_offset;
	extern char display_ram[6][21];
	int8 number_of_set_units;
	char text[21];

	FLASH_REG[0] = 0;
	scroll_offset = 0;

	//Now we need to find out how many units have been addressed already
	number_of_set_units = read_eeprom(no_of_units);
	if(number_of_set_units == 0) number_of_set_units = 1;	//alway assume there is one unit.
	reset_all_addr_menu();

	while(!chk_ent())
	{
		if(chk_up()) 
		{	//reset all set up unit addresses.
			reset_disp_ram();       
			text = "     Please Wait    ";
			strcpy(&display_ram[0], &text);
			FLASHING_OFF
			send_RS485(RST_ADR,0);
			FLASHING_ON
			delay_ms(3000);
			text = "  RESET SUCCESSFUL  ";
			strcpy(&display_ram[0], &text);
			write_eeprom(no_of_units,0);
			delay_ms(3000);
			set_reset_addr_menu();
			break;
		}
	}
	return 1;
}


void set_time_date (void)
{
	extern struct TIME TIME_STAMP;
   	extern struct TIME *pTIME_STAMP;
	extern int8 scroll_offset;
	extern int8 FLASH_REG[2];
	extern char display_ram[6][21];
	char text[21];
	int8 number;

	pTIME_STAMP = &TIME_STAMP;	
	scroll_offset = 0;
	reset_disp_ram();
	read_RTC();
	text = "ENTER DATE  YY MM DD";
	strcpy(&display_ram[0], &text);
	text = 	"ENTER TIME  HH MM SS";
	strcpy(&display_ram[2], &text);
	pTIME_STAMP = &TIME_STAMP;
	FLASH_REG[0] = 0;
	number = convert_BCD_to_No (pTIME_STAMP->year,7);
	number = enter_number (65,16,number,1,12);
	pTIME_STAMP->year = convert_No_to_BCD (number);

	number = convert_BCD_to_No (pTIME_STAMP->month,5);
	number = enter_number (12,1,number,1,15);
	pTIME_STAMP->month = convert_No_to_BCD (number);


	number = convert_BCD_to_No (pTIME_STAMP->day,6);
	number = enter_number (31,1,number,1,18);
	pTIME_STAMP->day = convert_No_to_BCD (number);


	number = convert_BCD_to_No (pTIME_STAMP->hour,5);
	number = enter_number (23,0,number,3,12);
	pTIME_STAMP->hour = convert_No_to_BCD (number);


	number = convert_BCD_to_No (pTIME_STAMP->min,7);
	number = enter_number (59,0,number,3,15);
	pTIME_STAMP->min = convert_No_to_BCD (number);

	number = convert_BCD_to_No (pTIME_STAMP->sec,7);
	number = enter_number (59,0,number,3,18);
	pTIME_STAMP->sec = convert_No_to_BCD (number);
	write_RTC();
	FLASH_REG[0] = BIT0;
	set_top_menu();
}

int8 convert_BCD_to_No (int8 BCDin,int8 Act_Bits)
{
	int8 output = 0, BIT_MASK = 0b00000001, i;
	int8 TAB_BDC_VAL[7] = {1,2,4,8,10,20,40};

	for(i=0;i<Act_Bits;i++)
	{
		if(BCDin & BIT_MASK)
		{
			output += TAB_BDC_VAL[i];
		}
		BIT_MASK = BIT_MASK << 1;
	}
	return output;
}

int8 convert_No_to_BCD (int8 no_in)
{	//Note the 40,20,10 columns override the 8,4,2,1 i.e. 10 = 10, not 8+2.
	int8 BCDout;
	int8 val;

	val = no_in/10;
	BCDout = val << 4;
	val = no_in - (10*val);
	BCDout |= val;
	return BCDout;
}


int8 enter_number (int8 hi_lim, int8 lo_lim, int8 number, int8 line, int8 col)
{
	int8 digit1,digit2,flash_timer = 25;

	//first ensure numbers are in range
	if(number > hi_lim) number = hi_lim;
	if(lo_lim != 0) {if(number < lo_lim) number = lo_lim;}

	for( ; ; )
	{
		if(chk_up()) 
		{
			number++;
			if(number > hi_lim) number = lo_lim;
		}	
		if(chk_dn()) 
		{
			if(number <= lo_lim) {number = hi_lim;} else {number--;}
		}
		if(chk_ent())
		{	//done
			display_ram[line][col] = digit1;
			display_ram[line][col+1] = digit2;
			return number;
		}	
		flash_timer++;
		if(flash_timer > 50) flash_timer = 0;
		if(flash_timer < 25)
		{
			display_ram[line][col] = ' ';
			display_ram[line][col+1] = ' ';
		}
		else
		{
			//convert to display digits and set in display ram.
			digit1 = (number/10) + '0';
			display_ram[line][col] = digit1;
			digit2 = number;
			while (digit2 > 9)
			{
				digit2 = digit2 -10;
			}
			digit2 = digit2 + '0';
			display_ram[line][col+1] = digit2;
		}			
		delay_ms(20);
	}
}






