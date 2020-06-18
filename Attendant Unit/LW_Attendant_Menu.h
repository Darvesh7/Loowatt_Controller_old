/****************************************************************************
* File Name: LooWatt Attendant_Menu.h
* Purpose:
*		Menu header file.         
* Parameters Passed: 
* Parameters Returned: 
* Globals:
* Date Started 	28/02/2016	
* Version		Not released
****************************************************************************/  
void display_USB_warning (int8 action);
int1 set_init_mess (void);
void set_top_menu (void);
void reset_disp_ram (void);
void set_reset_addr_menu (void);
void reset_all_addr_menu (void);
void menu_sys (void);
void set_trailer_id (void);
void set_rst_units (void);
void set_unit_addrs (void);
int8 rst_unit_addrs (void);		//return to break from function.
void set_time_date (void);
int8 convert_BCD_to_No (int8 BCDin,int8 Act_Bits);
int8 convert_No_to_BCD (int8 no_in);
int8 enter_number (int8 hi_lim, int8 lo_lim, int8 number, int8 line, int8 col);

#define ID_POS 9
#define disp_warning 1
#define rst_warning 0