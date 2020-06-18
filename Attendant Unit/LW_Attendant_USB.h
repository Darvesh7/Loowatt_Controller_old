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
int1 initalise_USB_files(int8 trailer_ID);
int1 send_log_data_to_USB(void);
void number_to_string (int16 number, *string_out);
void date_to_string (struct TIME *pTIME, char *string);
void time_to_string (struct TIME *pTIME, char *string);

