/***************************************************************************************
* Name:   	MAIN.h
* 
* Revision: draft
*
* Author:  	Phil Hockley
*
* Description:
*		 	Loo Watt Attendant main header file.
*
* History: 	30/03/15	Initial code started
*			
****************************************************************************************/

//Defines
#BYTE 	OSCCON = 0x0FD3
#BYTE	TRISA = 0x0F92
#BYTE	TRISB = 0x0F93
#BYTE	TRISC = 0x0F94
#BYTE	TRISD = 0x0F95
#BYTE	TRISE = 0x0F96
#BYTE	TRISF = 0x0F97
#BYTE	TRISG = 0x0F98
#BYTE	PORTA = 0x0F80		
#BYTE	PORTB = 0x0F81		
#BYTE	PORTC = 0x0F82		
#BYTE	PORTD = 0x0F83		
#BYTE	PORTE = 0x0F84		
#BYTE	PORTF = 0x0F85		
#BYTE	PORTG = 0x0F86		
#BYTE 	ADCON0 = 0x0FC2
#BYTE	ADCON1 = 0x0FC1
#BYTE	ADCON2 = 0x0FC0
#BYTE	ANCON0 = 0x0F25
#BYTE	ANCON1 = 0x0F24
#BYTE 	ANCON2 = 0x0F23
#BYTE  	ADRESL = 0x0FC3
#BYTE 	ADRESH = 0x0FC4
#BYTE	PIR1 = 0x0F9E
#BYTE 	PIR2 = 0x0FA1
#BYTE	PIR3 = 0x0FA4
#BYTE	PIR4 = 0x0FB7
#BYTE	PIE1 = 0x0F9D
#BYTE	PIE2 = 0x0FA0
#BYTE	PIE3 = 0x0FA3
#BYTE	PIE4 = 0x0FB6
#BYTE	WDTCON = 0x0FD1
#BYTE	T1CON = 0x0FCD
#BYTE	TMR1L = 0x0FCE
#BYTE	TMR1H = 0x0FCF
#BYTE	T2CON = 0x0FCA
#BYTE	PR2 = 0x0FCB
#BYTE 	TMR2 = 0x0FCC
#BYTE	INTCON = 0x0FF2
#BYTE	COMMS_IN = 0x0FAE		//RCREG1
#BYTE	COMMS_OUT = 0x0FAD		//TXREG1
#BYTE	SPBRG1 = 0x0FAF
#BYTE	RCSTA1 = 0x0FAB
#BYTE	TXSTA1 = 0x0FAC
#BYTE	BAUD1CON = 0x0F65
#BYTE	BAUD2CON = 0x0F20
#BYTE	RCREG2 = 0x0F1D
#BYTE	TXREG2 = 0x0F1C
#BYTE	SPBRG2 = 0x0F1E
#BYTE	RCSTA2= 0x0F22
#BYTE	TXSTA2 = 0x0F21
#BYTE	BAUD2CON = 0x0F20
#BYTE 	CCP4CON = 0x0F0F
#BYTE 	CCPTMRS0 = 0x0F2D
#BYTE 	PR4 = 0x0F04
#BYTE 	CCPR4L = 0x0F10
#BYTE 	T4CON = 0x0F03

#BYTE	T6CON = 0x0F39
#BYTE	PR6 = 0x0F3A
#BYTE	TMR6 = 0x0F3B

#BIT	GO_DONE = ADCON0.1
#BIT	TMR1ON = T1CON.0
#BIT	TMR1IF = PIR1.0
#BIT	TRMT1 = TXSTA1.1
#BIT	TX_EN = TXSTA1.5
#BIT	RX_EN = RCSTA1.4
#BIT	RC1IF = PIR1.5
#BIT	CREN = RCSTA1.4
#BIT	TMR2_IF = PIR1.1
#BIT 	TMR2IE = PIE1.1
#BIT	TMR6IE = PIE3.3
#BIT	TMR6IF = PIR3.1

#define MK3
//IO Defines
#BIT	LCD_RS = PORTG.3
#BIT	LCD_RW = PORTG.0
#ifdef 	MK3
#BIT	LCD_E = PORTD.0
#else
#BIT	LCD_E = PORTD.0
#endif
#BIT	BUZZER_ON = PORTG.4
#BIT	EXT_LAMP = PORTD.7
#BIT	DIR1 = PORTC.0
#BIT	DIR2 = PORTC.1
#BIT	RTC_CE = PORTC.3
#BIT	NO_USB = PORTF.1
#BIT	PB_UP = PORTF.3
#BIT	PB_DN = PORTF.4
#BIT	PB_ENT = PORTF.5
//SPI pins
#BIT 	SPI_SCK = PORTC.2
#BIT	SPI_SDO = PORTC.4
#BIT	SPI_SDI = PORTC.5



//Macro's
#define DO_NOP #asm NOP #endasm
#define wait1us DO_NOP;DO_NOP;
#define START_GET_DATA_TO TMR1ON = 1;
#define STOP_GET_DATA_TO TMR1ON = 0;
#define RST_GET_DATA_TO set_timer1(0); TMR1IF = 0;
#define RX1_BYTE_RDY RC1IF == 1			//used in statements, no ; required.
#define RX1_BYTE_NRDY RC1IF == 0		//used in statements, no ; required.
#define FLASHING_ON TMR2_IF = 0; TMR2 = 0; TMR2IE = 1;
#define FLASHING_OFF TMR2IE = 0;
#define INT_EN INTCON |= 0b11000000;
#define INT_DIS INTCON &= 0b00111111;
#define TIME_ON TMR6IE = 1;
#define TIME_OFF TMR6IE = 0;
#define TOP_LINE_FLASH_ON FLASH_REG[0] |= BIT0;
#define TOP_LINE_FLASH_OFF FLASH_REG[0] &= ~BIT0;
#define RTC_EN 	SPI_SCK = 0; DO_NOP; RTC_CE = 1;
#define RTC_DIS RTC_CE = 0;SPI_SCK = 0;

#define DIR_RX 0
#define DIR_TX 1
#define ON 1
#define OFF 0

//Bit defines
#define BIT0 0b00000001
#define BIT1 0b00000010
#define BIT2 0b00000100
#define BIT3 0b00001000
#define BIT4 0b00010000
#define BIT5 0b00100000
#define BIT6 0b01000000
#define BIT7 0b10000000

#define BATT_LOW_LIM 716


//display defines
#define DAT 0
#define CMD 1
#define max_retries 5

//Display flashing defines.
#define half_way 25
#define flash_rate 50

//time structure
typedef struct TIME {
	int8 sec;
	int8 min;
	int8 hour;
	int8 day;
	int8 month;
	int8 year;
};

//STATES
#define SERVICE 0
#define RUNNING 1
#define OO_SERVICE 2

//STATUS BYTE is as follows.
//bit15-8 used for the number of resets taken place since unit started
//bit0-7 (in order)
#define RUNSTATE  0b00000010
#define SERSTATE  0b00000001
#define OOSERVICE 0b00000100
#define LOW_FILM  0b00001000
#define blockage  0b00010000
//#define LS_RESET  0b00000111
//#define RST_STATE 0b11111000
//#define OVERFLOW_LIM 40000		//allows up to 40M of film.


//Data unions
#define UNION_LEN 20
typedef union LOG_UNION {
	int16	LOG_DATA[UNION_LEN/2];
	int8	LOG_RXD[UNION_LEN];	
};

//For single bits logic  1 is active state. For len, it is 0-2 (11 is empty loo).
//Note the first flush after the bag_reset will reset the bag_reset bit.
#define LooSTATUS	LOG_DATA[0]
#define MFB_USED	LOG_DATA[1]
#define MBB_USED 	LOG_DATA[2]
#define NOI_FLUSH	LOG_DATA[3]
#define NOIB_USED	LOG_DATA[4]
#define NOS_FLUSH	LOG_DATA[5]
#define NOSB_USED	LOG_DATA[6]
#define BAG_USED	LOG_DATA[7]
#define BAG_LEFT	LOG_DATA[8]
#define NO_BLOCKS	LOG_DATA[9]
	
//RS485 Defines
#define NO	0
#define YES 1

//loo error defines
#define RUN_BIT BIT5
#define LOW_BATT BIT4
#define ERR_BIT BIT3
#define BLK_BIT BIT2
#define SER_BIT BIT1
#define LOW_BIT BIT0

#define Max_no_LOOS 6		//max units in a trailer, Changed PH 121217 renamed to Max_no_LOOS, and 6 max

//EEPROM Defines
#define trailer_addr 0		//EEPROM address of trailer ident.
#define no_of_units 1		//actual number of loos fitted


struct TIME TIME_STAMP;
struct TIME *pTIME_STAMP;
union LOG_UNION DATA_IN[Max_no_LOOS];
union LOG_UNION *pDATA_IN;

//Timings
#define W_HOUR 120		//based on a USB update rate of 30 seconds = 1 hour.

int8 FLASH_REG[2];
int8 scroll_offset;
int8 ALARM_FLAG;
int8 FLASH_FLAG;
int8 tick;				//gets set every LCD update used for general timing/de-bounce
int8 target_USB = 0xFF;
int8 target_MENU = 0xFF;
char TRAILER_ID;
int8 no_of_loos;
int8 max_scroll_offset;
int8 *Ploo[11];
int8 Loo_FREG[11];
int8 Loo_FL[11];
int8 SEND_FLT_FLAG = 0;	//tells attendant new fault so send to webserver.
