/***************************************************************************************
* Name:      void UPDATE_IO(int8 IO_STATE)MAIN.h
*
* Revision: draft
*
* Author:     Phil Hockley
*
* Description:
*          ECO-6 main header file.
*
* History:    30/03/15   Initial code started
*
****************************************************************************************/

//Defines
#BYTE   OPTION_REG = 0x0095
#BYTE    OSCCON = 0x0099
#BYTE   TRISA = 0x008C
#BYTE   TRISB = 0x008D
#BYTE   TRISC = 0x008E
#BYTE   TRISD = 0x008F
#BYTE   TRISE = 0x0090
#BYTE   TRISF = 0x030C
#BYTE   TRISG = 0x030D
#BYTE   PORTA = 0x000C
#BYTE   PORTB = 0x000D
#BYTE   PORTC = 0x000E
#BYTE   PORTD = 0x000F
#BYTE   PORTE = 0x0010
#BYTE   PORTF = 0x028C
#BYTE   PORTG = 0x028D
#BYTE    ADCON0 = 0x009D
#BYTE   ADCON1 = 0x009E
#BYTE   ANSELA = 0x018C
#BYTE   ANSELE = 0x0190
#BYTE    ANSELF = 0x040C
#BYTE   ANSELG = 0x040D
#BYTE   ADRESL = 0x009B
#BYTE   ADRESH = 0x009C
#BYTE   PIR1 = 0x0011
#BYTE    PIR2 = 0x0012
#BYTE   PIR3 = 0x0013
#BYTE   PIR4 = 0x0014
#BYTE   PIE1 = 0x0091
#BYTE   PIE2 = 0x0092
#BYTE   PIE3 = 0x0093
#BYTE   PIE4 = 0x0094
#BYTE   WDTCON = 0x0097
#BYTE   TMR0 = 0x0015
#BYTE   TMR2 = 0x001A
#BYTE   T1CON = 0x0018
#BYTE   TMR1L = 0x0016
#BYTE   TMR1H = 0x0017
#BYTE   PR2 = 0x001B
#BYTE   T2CON = 0x001C
#BYTE   PR4 = 0x0416
#BYTE   T4CON = 0x0417
#BYTE   PR6 = 0x041D
#BYTE   T6CON = 0x041E
#BYTE   INTCON = 0x000B
#BYTE    EEADRL = 0x0191
#BYTE    EEADRH = 0x0192
#BYTE    EEDATL = 0x0193
#BYTE    EEDATH = 0x0194
#BYTE    EECON1 = 0x0195
#BYTE    EECON2 = 0x0196

#BIT   GO_DONE = ADCON0.1
#BIT   TMR2_IF = PIR1.1
#BIT   TMR1ON = T1CON.0
#BIT   TMR4_IE = PIE3.1
#BIT   TMR6_IE = PIE3.3
#BIT   TMR4_IF = PIR3.1
#BIT   TMR6_IF = PIR3.3
#BIT   GIE = INTCON.7
#BIT   PEIE = INTCON.6
#BIT   TMR0IF = INTCON.2
#BIT   TMR0IE = INTCON.5
#BIT   WREN = EECON1.2
#BIT   E2WR = EECON1.1
#BIT   E2RD = EECON1.0
#BIT   CFGS = EECON1.6
#BIT   EEPGD = EECON1.7

//COMMS Bytes/def's and Macros
#BYTE   RC1REG = 0x0199
#BYTE   TX1REG = 0x019A
#BYTE   SP1BRGL = 0x019B
#BYTE   SP1BRGH = 0x019C
#BYTE   RC1STA = 0x019D
#BYTE   TX1STA = 0x019E
#BYTE   BAUD1CON = 0x019F
#BIT   TX1_DONE = TX1STA.1
#BIT   CREN1 = RC1STA.4
#BIT   TX1_INT_EN = PIE1.4
#BIT   TX1_EN = TX1STA.5
#BIT   RX1_EN = RC1STA.4
#BIT   RX1_IF = PIR1.5
#BIT   TXRX = PORTF.1
#define   COMM_IN RC1REG
#define   COMM_OUT TX1REG
#define EN_TX1_INT TX1_INT_EN = 1
#define DIS_TX1_INT TX1_INT_EN = 0
#define EN_RX1_INT TX1_INT_EN = 1
#define DIS_RX1_INT TX1_INT_EN = 0
#define DIR_RX 0
#define DIR_TX 1
#define MESS_IN_LEN 5
#define MESS_OUT_LEN 24
#define incoming_len 1

#BYTE   RC2REG = 0x0491
#BYTE   TX2REG = 0x0492
#BYTE   SP2BRGL = 0x0493
#BYTE   SP2BRGH = 0x0494
#BYTE   RC2STA = 0x0495
#BYTE   TX2STA = 0x0496
#BYTE   BAUD2CON = 0x0497
#BIT   TX2_DONE = TX2STA.1
#BIT   CREN2 = RC2STA.4
#BIT   TX2_INT_EN = PIE4.4
#BIT   TX2_EN = TX1STA.5
#BIT   RX2_EN = RC1STA.4
#BIT   RX2_IF = PIR4.5
#define   BOOT_IN RC1REG
#define   BOOT_OUT TX1REG
#define EN_TX2_INT TX2_INT_EN = 1
#define DIS_TX2_INT TX2_INT_EN = 0
#define EN_RX2_INT TX2_INT_EN = 1
#define DIS_RX2_INT TX2_INT_EN = 0

//Comparators
#BYTE CM3CON0 = 0x011E
#BYTE CM3CON1 = 0x011F
#BYTE FVRCON = 0x0117
#BYTE CMOUT = 0X0115
#BIT OVER_I = CMOUT.2
#define MOTOR_OI 0


//IO Defines
#BIT   MOTOR_OUT0 = PORTF.3
#BIT   MOTOR_OUT1 = PORTF.4
#BIT   MOTOR_OUT2 = PORTF.6
#BIT   MOTOR_OUT3 = PORTF.5
#BIT   FLUSH_SW = PORTC.1
#BIT   ENC_A = PORTC.0
#BIT   RST_IN = PORTB.0
#BIT   SET_MODE = PORTC.2
#BIT   FWD = PORTC.3
#BIT   BCK = PORTB.1
#BIT   ALARM_ON = PORTB.3
#BIT   LED_FLUSH = PORTF.7
#BIT   LED_SER = PORTE.1
#BIT   LED_RUN   = PORTE.0
#BIT   LED_OFF = PORTG.0
#BIT   BUZZER_ON = PORTB.2


//Macro's
#define DO_NOP #asm NOP #endasm
#define DELAY1US DO_NOP;DO_NOP;   //at 8MHz
#define ENCODER_START TMR1ON = 1
#define ENCODER_STOP TMR1ON = 0
#define ENCODER_ZERO set_timer1(0)
#define READ_ENCODER get_timer1()
#define EN_BEEP_INT TMR4_IE = 1
#define DIS_BEEP_INT TMR4_IE = 0
#define EN_FLASH_INT TMR6_IE = 1
#define DIS_FLASH_INT TMR6_IE = 0
#define RESET_BEEP_IF TMR4_IF = 0
#define RESET_FLASH_IF TMR6_IF = 0
#define EN_TMR0_INT TMR0IE = 1
#define DIS_TMR0_INT TMR0IE = 0


#define tick_time 10   //mS
#define bag_used_constant 1300   //(6.53): 90mm = 735 pulses.
#define BATT_OK_LIM   832  //10V -> 1023 * Vbatt/(3 * 4.096)MK3
#define BATT_NOK_LIM 749 //9V -> 1023 * Vbatt/(3 * 4.096)MK3
#define debounce_time_pressed 30/tick_time
#define debounce_time_released 100/tick_time
#define motor_start_delay 200/(10*tick_time)
#define motor_run_delay 300/(10*tick_time)
#define min_startup_pulses  75        //75 new encoder
#define min_pulse_count_10ticks 80  //40 new encoder
#define max_block_counts 2
#define max_count16 0xFFFF
//#define fail_alarm_time 2000/tick_time   //uses int8 so no bigger than 256 allowed (after division)
#define short_beep 1000/tick_time
#define block_beep_rate 5000/tick_time
#define Seconds_10 10000/tick_time
#define mS_200 2   //in 100mS steps
#define mS_500 5
#define mS_1000 10
#define mS_2000 20
#define mS_4500 45
#define mS_5000 50
#define mS_10000 100
#define PR_100mS 195
#define once 1
#define continuous 0


#define BIT0 0b00000001
#define BIT1 0b00000010
#define BIT2 0b00000100
#define BIT3 0b00001000
#define BIT4 0b00010000
#define BIT5 0b00100000
#define BIT6 0b01000000
#define BIT7 0b10000000

#define RESET_IO  0b00000000
#define MODE_OFF  BIT0
#define MODE_RUN  BIT1
#define MODE_SER  BIT2
#define BUZZER     BIT3
#define FLUSH     BIT4
#define DNU_LED   BIT5
#define block_flag 0x08

//EEPROM Addressses.
#define unit_addr_addr 0
#define Bag_len_addr 3      //allow 2 bytes
#define status_addr 5


//STATUS BYTE is as follows.
//bit15-8 used for the number of resets taken place since unit started
//bit7-0 (in order)
#define SERSTATE  0b00000001
#define RUNSTATE  0b00000010
#define OOSERVICE 0b00000100
#define LOW_FILM  0b00001000
#define blockage  0b00010000
#define LS_RESET  0b00000111
#define RST_STATE 0b11111000
#define OVERFLOW_LIM 40000      //allows up to 40M of film, blocked can be any other number
#define SERVICE 1
#define RUNNING 2
#define OO_SERVICE 4
#define BLOCKED 5      //not a true state, but defined as part of the actual states.

//define buffer for transmitting data to attendant
#define TX_MESS_WORDS 10
int8 LooSTATUS;
int16 TL_DATA[TX_MESS_WORDS];
//Note there is no buffer for the status byte, as buffering this is unnecessary.
#define STATUS_16   TL_DATA[0]      //16 bit loo status
#define MFB_USED   TL_DATA[1]      //Motor Forward Bag Used
#define MBB_USED    TL_DATA[2]      //Motor Back Bag Used
#define NOI_FLUSH   TL_DATA[3]      //No of Initial flushes
#define NOIB_USED   TL_DATA[4]      //Inital flush bag used
#define NOS_FLUSH   TL_DATA[5]      //No of secondary flushes
#define NOSB_USED   TL_DATA[6]      //secondary flush bag used
#define BAG_USED   TL_DATA[7]      //Total bag used
#define BAG_LEFT   TL_DATA[8]      //bag left
#define NO_BLOCKS    TL_DATA[9]      //number of blockages

//RS485 Defines
#define NO   0
#define YES 1
#define ON 1
#define OFF 0

//General Globals
int16   stop_flag = 0;
int8    IO_STATE;
int8    flash_what;
int16   flash_rate,flash_time,beep_rate,beep_time,beep_count=0,flash_count=0;
int8   beep_mode = 0;
int1    batt_low_flag = 0;
int8    NO_RESETS = 0;
int8    UNIT_ADR;
char message_out[MESS_OUT_LEN];
char message_in[MESS_IN_LEN];


