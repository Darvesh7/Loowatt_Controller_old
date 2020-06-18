/***************************************************************************************
* Name:   	LW_Attendant_Display.C
* 
* Revision: draft
*
* Author:  	Phil Hockley
*
* Description:
*		 	Display functions
*
* History: 	28/02/16	Initial code started
*			
****************************************************************************************/
#INT_TIMER2 
void FLASH_TIMER (void)
{
	extern int8 FLASH_REG[2];
	extern int8 scroll_offset;
	extern char display_ram[6][21];
	extern int8 ALARM_FLAG;
	extern int8 FLASH_FLAG;
	extern int8 display_type;
	extern int8 tick;
	static int8 flash_timer = 0;
	char text[21];
	int8 FLSH_REG1,FLSH_REG2;

	flash_timer++;
	tick = 1;
	
	//align flash registers with scrolled screen
	FLSH_REG1  = FLASH_REG[0] >> scroll_offset;
	FLSH_REG2  = FLASH_REG[1] >> scroll_offset;

	if(flash_timer <= half_way)
	{	//all things that flash are off
		if(display_type == NORMAL)
		{           
			text = "          ";
			if(FLASH_REG[0] & BIT0) write_text_LCD (LINE_1, line1_flash_A, &text);
			text = "   ";
			if(FLSH_REG1 & BIT1) write_text_LCD (LINE_2, line_flash_A1, &text);
			if(FLSH_REG1 & BIT2) write_text_LCD (LINE_3, line_flash_A1, &text);
			if(FLSH_REG1 & BIT3) write_text_LCD (LINE_4, line_flash_A1, &text);
			if(FLSH_REG2 & BIT1) write_text_LCD (LINE_2, line_flash_A2, &text);
			if(FLSH_REG2 & BIT2) write_text_LCD (LINE_3, line_flash_A2, &text);
			if(FLSH_REG2 & BIT3) write_text_LCD (LINE_4, line_flash_A2, &text);
		}
		else
		{	//display is menu, so flash entire line
			text = "                    ";
			if(FLSH_REG1 & BIT0) write_text_LCD (LINE_1, 0, &text);
			if(FLSH_REG1 & BIT1) write_text_LCD (LINE_2, 0, &text);
			if(FLSH_REG1 & BIT2) write_text_LCD (LINE_3, 0, &text);
			if(FLSH_REG1 & BIT3) write_text_LCD (LINE_4, 0, &text);
			
		}
		EXT_LAMP = OFF;
		BUZZER_ON = OFF;
		
	}
	else
	{	//all things are on, though alarms may need to be off.
		if(display_type == NORMAL)
		{
			write_text_LCD (LINE_1, 0, &display_ram[0][0]);
			write_text_LCD (LINE_2, 0, &display_ram[1+scroll_offset][0]);
			write_text_LCD (LINE_3, 0, &display_ram[2+scroll_offset][0]);
			write_text_LCD (LINE_4, 0, &display_ram[3+scroll_offset][0]);
		}
		else
		{
			write_text_LCD (LINE_1, 0, &display_ram[0][0]);
			write_text_LCD (LINE_2, 0, &display_ram[1][0]);
			write_text_LCD (LINE_3, 0, &display_ram[2][0]);
			write_text_LCD (LINE_4, 0, &display_ram[3][0]);
		}
		if(ALARM_FLAG) BUZZER_ON = ON;
		if(FLASH_FLAG) EXT_LAMP = ON;
	}
	if(flash_timer >= flash_rate) flash_timer = 0;
}




void init_display (void)
{
	delay_ms(100);
	WRITE_IR
	LCD_IO = FUNC_SET;
	CYCLE_E
	delay_ms(100);
	LCD_IO = DISP_MODE;	
	CYCLE_E
	delay_ms(100);
	LCD_IO = CLEAR_DISP;	
	CYCLE_E
	delay_ms(5);
	LCD_IO = ENTRY_MODE;	
	CYCLE_E
	delay_ms(100);
}


void write_text_LCD (int8 line, int8 col, char *pLCD_TEXT)
{	//line is a defined hex number, col 0-19)
	int8 address;
	int1 F_OK;

	address =  line + col;
	LCD_IO_DIR_OP;
	WRITE_IR
	LCD_IO = (SET_DDRAM | address);
	CYCLE_E
	delay_us(50);

	while(*pLCD_TEXT != 0)
	{
		F_OK = 0;
//		LCD_IO_DIR_IP
/*		do
		{
			READ_BUSY
			LCD_E = 1;
			DO_NOP;
			if(LCD_BF & BIT7) F_OK = 1;
			LCD_E = 0;
			delay_us(10);
		}while (F_OK == 0);
		LCD_E = 0;
*/
//		LCD_IO_DIR_OP;
		LCD_IO = *pLCD_TEXT;
		WRITE_DATA
		CYCLE_E
		pLCD_TEXT++;
		delay_us(50);
	}
DO_NOP
}


void set_run_display_ram(void)
{	//were going to do this the easy, but long winded way

	extern int8 display_type;
	extern int8 no_of_loos;
	extern int8 *Ploo[11];
	extern int8 Loo_FREG[11];
	extern int8 max_scroll_offset;
	extern char display_ram[6][21];
	extern int8 max_scroll_offset;
	extern int8 TRAILER_ID;
	extern int8 Loo_FL[11];
	char text[21];
		   				 
	reset_disp_ram();
	display_type = NORMAL;
	text = "UNIT   :            ";
	strcpy(&display_ram[0], &text);
	display_ram[0][6] = TRAILER_ID;
								 
	pLoo[1] = &display_ram[1][0];		//loo1 is always here.
	Loo_FREG[1] = 0;
	Loo_FL[1] = BIT1;
	if(no_of_loos <= 2)
	{
		pLoo[2] = &display_ram[1][10];
		Loo_FREG[2] = 1;
		Loo_FL[2] = BIT1;
		max_scroll_offset = 0;
	}
	else if (no_of_loos <= 4)
	{
		pLoo[2] = &display_ram[2][0];
		Loo_FREG[2] = 0;
		Loo_FL[2] = BIT2;
		pLoo[3] = &display_ram[1][10];
		Loo_FREG[3] = 1;
		Loo_FL[3] = BIT1;
		pLoo[4] = &display_ram[2][10];		//doesn't matter if loo4 does not exist.
		Loo_FREG[4] = 1;
		Loo_FL[4] = BIT2;
		max_scroll_offset = 0;
	}
	else if (no_of_loos <= 6)
	{
		pLoo[2] = &display_ram[2][0];
		Loo_FREG[2] = 0;
		Loo_FL[2] = BIT2;
		pLoo[3] = &display_ram[3][0];
		Loo_FREG[3] = 0;
		Loo_FL[3] = BIT3;
		pLoo[4] = &display_ram[1][10];		
		Loo_FREG[4] = 1;
		Loo_FL[4] = BIT1;
		pLoo[5] = &display_ram[2][10];
		Loo_FREG[5] = 1;
		Loo_FL[5] = BIT2;
		pLoo[6] = &display_ram[3][10];
		Loo_FREG[6] = 1;
		Loo_FL[6] = BIT3;
		max_scroll_offset = 0;
	}
	else if (no_of_loos <= 8)
	{
		pLoo[2] = &display_ram[2][0];
		Loo_FREG[2] = 0;
		Loo_FL[2] = BIT2;
		pLoo[3] = &display_ram[3][0];
		Loo_FREG[3] = 0;
		Loo_FL[3] = BIT3;
		pLoo[4] = &display_ram[4][0];		
		Loo_FREG[4] = 0;
		Loo_FL[4] = BIT4;
		pLoo[5] = &display_ram[1][10];
		Loo_FREG[5] = 1;
		Loo_FL[5] = BIT1;
		pLoo[6] = &display_ram[2][10];
		Loo_FREG[6] = 1;
		Loo_FL[6] = BIT2;
		pLoo[7] = &display_ram[3][10];
		Loo_FREG[7] = 1;
		Loo_FL[7] = BIT3;
		pLoo[8] = &display_ram[4][10];
		Loo_FREG[8] = 1;
		Loo_FL[8] = BIT4;
		max_scroll_offset = 1;
	}
	else 
	{
		pLoo[2] = &display_ram[2][0];
		Loo_FREG[2] = 0;
		Loo_FL[2] = BIT2;
		pLoo[3] = &display_ram[3][0];
		Loo_FREG[3] = 0;
		Loo_FL[3] = BIT3;
		pLoo[4] = &display_ram[4][0];		
		Loo_FREG[4] = 0;
		Loo_FL[4] = BIT4;
		pLoo[5] = &display_ram[5][0];
		Loo_FREG[5] = 0;
		Loo_FL[5] = BIT5;
		pLoo[6] = &display_ram[1][10];
		Loo_FREG[6] = 1;
		Loo_FL[6] = BIT1;
		pLoo[7] = &display_ram[2][10];
		Loo_FREG[7] = 1;
		Loo_FL[7] = BIT2;
		pLoo[8] = &display_ram[3][10];
		Loo_FREG[8] = 1;
		Loo_FL[8] = BIT3;
		pLoo[9] = &display_ram[4][10];
		Loo_FREG[9] = 1;
		Loo_FL[9] = BIT4;
		pLoo[10] = &display_ram[5][10];
		Loo_FREG[10] = 1;
		Loo_FL[10] = BIT5;
		max_scroll_offset = 2;
	}
	(*pLoo[1]) = '1';		
	if(no_of_loos >= 2)(*pLoo[2]) = '2';		
	if(no_of_loos >= 3)(*pLoo[3]) = '3';		
	if(no_of_loos >= 4)(*pLoo[4]) = '4';		
	if(no_of_loos >= 5)(*pLoo[5]) = '5';		
	if(no_of_loos >= 6)(*pLoo[6]) = '6';		
	if(no_of_loos >= 7)(*pLoo[7]) = '7';		
	if(no_of_loos >= 8)(*pLoo[8]) = '8';		
	if(no_of_loos >= 9)(*pLoo[9]) = '9';		
	if(no_of_loos == 10) (*pLoo[10]) = 'A';		
}
