/***************************************************************************************
* Name:   	LW_Attendant_RS485.h
* 
* Revision: draft
*
* Author:  	Phil Hockley
*
*
* Description:
*		 	RS485 Functions/Interrupts Header file.
*
* History: 	14/04/15	Initial code started
*			15/03/16	New Comms function
*			
****************************************************************************************/
int1 send_RS485 (int8 action, int8 L_ADR);
void send_data_to_WEB(int8 *pFAULTS);
void delim_send (int8 data_in);
//Command Defines for comms
#define STX 0x02
#define ETX 0x03
#define DLE 0x10
#define STX_POS 0
#define LEN_POS 1
#define ADR_POS 2
#define DATA_START 3
#define BCC_POS 4		//only set/reset address, not for send data
#define OVERHEAD 4		
#define MAX_ADR 10
#define SET_ADR 0x1E
#define RST_ADR 0x4A
#define SND_DATA 0x2D