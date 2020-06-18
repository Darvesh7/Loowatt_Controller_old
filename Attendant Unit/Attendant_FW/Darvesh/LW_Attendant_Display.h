/***************************************************************************************
* Name:   	LW_Attendant_Display.H
* 
* Revision: draft
*
* Author:  	Phil Hockley
*
* Description:
*		 	prototype header file.
*
* History: 	28/02/16	Initial code started
*			
****************************************************************************************/
void FLASH_TIMER (void);
void init_display (void);
void write_text_LCD (int8 line, int8 col, char *pLCD_TEXT);
void set_run_display_ram();
#BIT LCD_BF = PORTE.7

//LCD defines
#define CLEAR_DISP 	0b00000001
#define RETURN_HOME	0b00000010
#define DISP_MODE 	0b00001100
#define FUNC_SET	0b00111100
#define ENTRY_MODE 	0b00000110
#define SET_DDRAM	0b10000000	//0-40H in bits 0-6.
#define LCD_IO PORTE
#define WRITE_IR	LCD_RS = 0;LCD_RW = 0;
//#define READ_BUSY	LCD_RS = 0;LCD_RW = 1;
#define WRITE_DATA  LCD_RS = 1;LCD_RW = 0;
#define CYCLE_E 	LCD_E = 1;DO_NOP;LCD_E = 0;
#define LCD_IO_DIR_IP TRISE = 0xFF;
#define LCD_IO_DIR_OP TRISE = 0;

//initialisation for programs

//Display line addresses in LCD DDRAM.
#define LINE_1 0x00
#define LINE_2 0x40
#define LINE_3 0x14
#define LINE_4 0x54


//LCD RAM AREA, though it only has 4 lines, there can be six total 
char display_ram[6][21];	//21 for the end of string terminator
#define line1_flash_A 10
#define No_Line1_chars 9
#define line_flash_A1 6
#define line_flash_A2 16
#define flash_col 6
#define flush_rem_col 2
#define No_Line_flash_chars 3
#define NORMAL 1
#define MENU 0

int8 display_type;


