/***************************************************************************************
* Name:   	LW_Controller_RS485.H
* 
* Revision: draft
*
* Author:  	Phil Hockley
*
* Description:
*		 	prototype header file.
*
* History: 	14/04/15	Initial code started
*			
****************************************************************************************/
void RX_ATTENDANT (void);
void TX_LOG_ATTENDANT (void);
void TX_BYTE_ATTENDANT (void);

//Command Defines for comms
#define STX 0x02
#define STX_POS 0
#define LEN_POS 1
#define ADR_POS 2
#define DATA_START 3
#define BCC_POS 4		//only set/reset address, not for send data
#define OVERHEAD 3		//The STX and BCC don't count
#define MAX_ADR 10
#define SET_ADR 0x1E
#define RST_ADR 0x4A
#define SND_DATA 0x2D
