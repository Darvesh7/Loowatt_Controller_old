/***************************************************************************************
* Name:      LW_Attendant_RS485.c
* 
* Revision: draft
*
* Author:     Phil Hockley
*
* Description:
*          RS485 Functions/Interrupts C file.
*         Sends a command, and replies with OK=1, bad mesaage = 0;
*
*      STX,LEN,ADR,Data_n,BCC
*      STX = 0x02
*      LEN = message length of data bytes only
*      ADR = Address of unit 0-255. Note 0 = general command (i.e. Broadcast mode)
*      data_n = data bytes of length LEN
*      BCC = exc or Bit test of LEN + ADR + data
*
*      LEN here is always 1 in transmitt (the command) and variable in receive
*      ADR is 0-10, with 0 as Transmitt broadcast. Data_in does not show the BCC, but it is present
*
*      Data_out =    STX,1,ADR,0x1E -> set addr, set the UNIT_ADR as ADR only works if unit adr is 0.
*      Data_in =    STX,1, UNIT_ADR, 0x1E, BCC   
*
*      Data_out =    STX,1,ADR,0x4A -> reset UNIT_ADR to 0. This has to be done if ADR is not 0, and you want to change it
*      Data_in =    STX,1, 0, 0x4A, BCC
*
*      Data_out =    0x2D ->   Send Data, send controller data over to the attendant
*      data_in =    STX,LEN,UNIT_ADR,Unit_Data,BCC. LEN = 20 at the moment.
*
* History:    14/04/15   Initial code started
*         15/03/16   New Comms function
*         
****************************************************************************************/

int1 send_RS485 (int8 action, int8 L_ADR)
{   
   extern union LOG_UNION DATA_IN[Max_no_LOOS];
   extern union LOG_UNION *pDATA_IN;
   char message_in[30];
   char message_out[5];
   int8 i,BCC = 0,junk;

   TX_EN = ON;
   DIR1 = DIR_TX;
   wait1uS
   TX_EN = ON;
   message_out[0] = STX;
   message_out[1] = 1;
   message_out[2] = L_ADR;
   message_out[3] = action;
   message_out[4] = 0x01 ^ L_ADR ^ action;
   for(i=0;i<5;i++)
   {
      COMMS_OUT = message_out[i];
      while(!TRMT1);

      delay_us(500);
   }
   //last byte sent, now wait for return data. Timeout if no data after 30mS.
   TX_EN = OFF;
   DIR1 = DIR_RX;
   wait1uS
   CREN = 0; CREN = 1;   junk = COMMS_IN;junk = COMMS_IN;
   RX_EN = ON;
   RST_GET_DATA_TO
   START_GET_DATA_TO
   if(message_out[3] == SET_ADR || message_out[3] == RST_ADR)
   {
      for(i=0;i<5;i++)
      {
         while(!RC1IF)
         {
            if(TMR1IF)
            {   //this is the only way out....
               STOP_GET_DATA_TO
               INIT_USART1();
               return 0;
            }
         }
         message_in[i] = COMMS_IN;
         if(message_in[0] != STX) 
         {
            STOP_GET_DATA_TO 
            INIT_USART1();
            return 0;
         }
      }
      BCC = message_in[1] ^ message_in[2] ^ message_in[3];
      if(BCC != message_in[4])
      {
         STOP_GET_DATA_TO
         INIT_USART1();
         return 0;
      }
   }
   else if(message_out[3] == SND_DATA)
   {
      i = 0;
      do 
      {
         while(!RC1IF)
         {
            if(TMR1IF)
            {   //this is the only way out....
               STOP_GET_DATA_TO
               return 0;
            }
         }
         message_in[i] = COMMS_IN;
         RST_GET_DATA_TO
         if(message_in[0] != STX) 
         {
            STOP_GET_DATA_TO return 0;
         }
         if((i > 0) && (i < ((message_in[LEN_POS]+OVERHEAD)-1))) BCC = BCC ^ message_in[i];
         i++;
      }while(i < (message_in[LEN_POS]+OVERHEAD));
         
      //delay_us(1000);
      i = COMMS_IN;
      if(BCC != i) {STOP_GET_DATA_TO INIT_USART1();return 0;}
      //everything OK, so load data to correct loo storage area.
      pDATA_IN = &DATA_IN[L_ADR-1];
      for(i=0;i<UNION_LEN;i++)
      {
         pDATA_IN->LOG_RXD[i] = message_in[i+4];
      }   
   }
   return 1;
}

void send_data_to_WEB(int8 *pFAULTS)
{
   extern union LOG_UNION DATA_IN[Max_no_LOOS];
   extern union LOG_UNION *pDATA_IN;
   extern int8 no_of_loos;
   extern char TRAILER_ID; 
   long long UNIX_TIME;
   static int8 loo = 0;
//   int16 bag_len_fitted;
   int8 i,x;

   UNIX_TIME = Convert_To_UNIX_TIME();

   //Start the transmission
   TXREG2 = STX;
   delay_ms(1);
   delim_send(TRAILER_ID);
   delim_send((int8)UNIX_TIME);
   delim_send((int8)(UNIX_TIME >> 8));
   delim_send((int8)(UNIX_TIME >> 16));
   delim_send((int8)(UNIX_TIME >> 24));
   delim_send(no_of_loos);
   //Now each controller information
   
   for(x = 0; x < 6; x++)
   {
      pDATA_IN = &DATA_IN[loo];
//      bag_len_Fitted = pDATA_IN -> LOG_DATA[7] + pDATA_IN -> LOG_DATA[8];
      delim_send(loo+1);         //loo number being sent 1-6
      delim_send(pFAULTS[loo+1]);
      for(i=0;i<UNION_LEN;i++)
      {
         delim_send(pDATA_IN->LOG_RXD[i]);      
      }
//      delim_send((int8)(bag_len_Fitted));
//      delim_send((int8)(bag_len_Fitted >> 8));
      loo++;
      if(loo > no_of_loos - 1) {loo = 0;break;}
   }
   TXREG2 = ETX;
}   

void delim_send (int8 data_in)
{
   if((data_in == STX) || (data_in == ETX) || (data_in == DLE))   
   {
      TXREG2 = DLE;
      delay_ms(1);
   }
   TXREG2 = data_in;
   delay_ms(1);
}



















