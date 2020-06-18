/***************************************************************************************
* Name:   	LW_Controller_RS485.c
* 
* Revision: draft
*
* Author:  	Phil Hockley
*
* Description:
*		 	RS485 Functions/Interrupts C file.
*		STX, LEN,ADR,Data_n,BCC
*		STX = 0x02
*		LEN = message length of data bytes only
*		ADR = Address of unit 0-255. Note 0 = general command (i.e. Broadcast mode)
*		data_n = data bytes of length LEN
*		BCC = Bit test of LEN + ADR + data
*
*		LEN here is always one byte in receive and variable in Transmitt
*		ADR is 0-10, with 0 as receive broadcast. Data_in does not show the BCC, but it is present
*
*		Data_in = 	STX,1,ADR,0x1E -> set addr, set the UNIT_ADR as ADR only works if adr is 0.
*		Data_out = 	STX,1, UNIT_ADR, 0x1E, BCC	
*
*		Data_in = 	ST,1,ADR,0x4A -> reset UNIT_ADR to 0. This has to be done if ADR is not 0, and you want to change it
*		Data_out = 	STX,1, 0, 0x4A, BCC
*
*		Data_in = 	0x2D ->	Send Data, send controller data over to the attendant
*		data_out = 	STX,LEN,UNIT_ADR,Unit_Data,BCC. LEN = 20 at the moment.
*
*
* History: 	14/04/15	Initial code started
*			20/02/16	Changed for new Protocol
*			
****************************************************************************************/
#INT_RDA
void RX_message (void)
{	//all extern to be defined in main.h as well, but without the extern part.
	extern int8 UNIT_ADR;
	extern char message_out[MESS_OUT_LEN];
	extern char message_in[MESS_IN_LEN];
	extern int8 LooSTATUS;
	extern int16 TL_DATA[TX_MESS_WORDS];
	static int8 index = 0, BCC = 0, length = 0;	
	static int8 restart_flag = 0;
	int8 junk;

	message_in[index] = COMM_IN;
	if((RC1STA & 0x02) == 2) {CREN1 = 0; CREN1 = 1;} //reset overflow errors
	if(message_in[STX_POS] != STX)
	{
		index = 0;	//restart
		restart_flag = 1;
	}
	if(index == LEN_POS) 
	{
		length = message_in[LEN_POS];
		//if(length != 1) index = 0;		//must be data send from another unit.
	}
	if(index > STX_POS && index < BCC_POS) BCC = BCC ^ message_in[index];
	if(index >= length+OVERHEAD)
	{	//message received now check message for errors
		if((BCC == message_in[BCC_POS]) && ((UNIT_ADR == message_in[ADR_POS]) || (UNIT_ADR > 10) || UNIT_ADR == 0 ||(message_in[ADR_POS] == 0)))
		{	//message received correctly, act on it here in this case there are only three commands
			BCC = 0;
			RX1_EN = 0;		//no more messages until this one evaluated.
			DIS_RX1_INT;
			TX1_EN = 1;		//allow transmission of return data.
			TXRX = DIR_TX;
			if(message_in[DATA_START] == RST_ADR)
			{	//no return data as all attached units will receive this on addr 0.
				UNIT_ADR = 0;
				write_eeprom(unit_addr_addr,0);
			}
			if(message_in[3] == SET_ADR) 
			{
				UNIT_ADR = message_in[ADR_POS];
				write_eeprom(unit_addr_addr,UNIT_ADR);
				message_out[1] = incoming_len;
				message_out[2] = UNIT_ADR;
				message_out[3] = SET_ADR;
				delay_us(500);
				COMM_OUT = STX;
				BCC = 0;
				EN_TX1_INT;
			}
			if(message_in[DATA_START] == SND_DATA)
			{
				message_out[1] = (TX_MESS_WORDS * 2) + OVERHEAD;
				message_out[2] = UNIT_ADR;
				message_out[3] = SND_DATA;
				message_out[DATA_START+1] = LooSTATUS;   	//load 8 bit loo state into 16 bit buffer.
				BCC = BCC ^ (message_out[DATA_START+1]);
				message_out[DATA_START+2] = NO_RESETS;
				BCC = BCC ^ (message_out[DATA_START+2]);
				//Message looks like this
				//STX - LEN - ADR - CMD - Byte - Byte. So index = 5, next is 6.
				//need to load the last 9 words into the next 18 bytes from TL_DATA.
				//Only 9 words as we have already done word 0;
				//we will do this as inline code, as we have plenty of memory
				message_out[6] = (int8)TL_DATA[1];
				message_out[7] = (int8)(TL_DATA[1]/256);

				message_out[8] = (int8)TL_DATA[2];
				message_out[9] = (int8)(TL_DATA[2]/256);

				message_out[10] = (int8)TL_DATA[3];
				message_out[11] = (int8)(TL_DATA[3]/256);

				message_out[12] = (int8)TL_DATA[4];
				message_out[13] = (int8)(TL_DATA[4]/256);

				message_out[14] = (int8)TL_DATA[5];
				message_out[15] = (int8)(TL_DATA[5]/256);

				message_out[16] = (int8)TL_DATA[6];
				message_out[17] = (int8)(TL_DATA[6]/256);

				message_out[18] = (int8)TL_DATA[7];
				message_out[19] = (int8)(TL_DATA[7]/256);

				message_out[20] = (int8)TL_DATA[8];
				message_out[21] = (int8)(TL_DATA[8]/256);

				message_out[22] = (int8)TL_DATA[9];
				message_out[23] = (int8)(TL_DATA[9]/256);
				delay_us(500);
				COMM_OUT = STX;
				BCC = 0;
				EN_TX1_INT;
			}
		}
		else
		{	//Task not for this unit turn off Receive for 25mS to prevent
			//data from other units screwing up the comms

			CREN1 = 0;
			delay_ms(25);
			RX1_IF = 0;
			junk = COMM_IN; 
			junk = COMM_IN; 
			CREN1 = 1;
		}
		index = 0;		//ready to start again	
		BCC = 0;
	}
	else 
	{
		if(restart_flag) {index = 0; restart_flag = 0; BCC = 0;} else {index++;}
	}		
}	

#INT_TBE                  
void TX_message (void)
{
	extern char message_out[MESS_OUT_LEN];
	static int8 index = 1, BCC = 0,mess_len;	
	static int8 BCC_FLAG = 0;
	int8 junk;

	mess_len = message_out[LEN_POS];
	if(index <= mess_len+2)	//only add 2 here as STX and BCC are sent separately.
	{
		delay_us(200);
		COMM_OUT = message_out[index];
		if(index > STX_POS) BCC = BCC ^ message_out[index];
		index++;
		if(index > mess_len+2) BCC_FLAG = 1;
	}
	else if(BCC_FLAG == 1)
	{
		delay_us(1000);	//wait for transmission to end, can't use the interrupt for this
		DIS_TX1_INT;
		BCC_FLAG = 0;
		COMM_OUT = BCC;
		delay_us(1000);	//wait for transmission to end, can't use the interrupt for this
		BCC = 0;		
		TX1_EN = 0;		
		index = 1;
		RX1_EN = 1;		//enable incoming messages.
		TXRX = DIR_RX;
		junk = COMM_IN;
		EN_RX1_INT;
	}
}