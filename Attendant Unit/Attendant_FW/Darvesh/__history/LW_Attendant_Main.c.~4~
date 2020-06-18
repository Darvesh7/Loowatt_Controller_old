/*****************************************************************************
*                               System Include Files
*****************************************************************************/

#INCLUDE <18F66K90.h>   //Flash programmable device 

/*****************************************************************************
*                             Pre-processor Directives
*****************************************************************************/ 
#USE DELAY (Clock = 8000000)    //the value used for the library routines            
#USE FAST_IO(ALL)   //compiler to perform I/O without programming Dir. reg. Port A    
#FUSES NOWDT,NOPROTECT,HSH,NOPUT,NOPLLEN
/*****************************************************************************
*                               File Includes
*****************************************************************************/
#INCLUDE <string.h>
#INCLUDE <LW_Attendant_MAIN.h>
#INCLUDE <LW_Attendant_display.h>
#INCLUDE <LW_Attendant_Func.h>
#INCLUDE <LW_Attendant_RS485.h>
#INCLUDE <LW_Attendant_Menu.h>
#INCLUDE <LW_Attendant_USB.h>
#INCLUDE <LW_Attendant_USB.c>
#INCLUDE <LW_Attendant_Display.c>
#INCLUDE <LW_Attendant_Func.c>
#INCLUDE <LW_Attendant_RS485.c>
#INCLUDE <LW_Attendant_Menu.c>
 
/****************************************************************************
* File Name: LooWatt Attendant_MAIN
* Purpose:
*      MAIN application program.         
* Parameters Passed: 
* Parameters Returned: 
* Globals:
* Date Started    10/05/2015
* Last Edit/By   28/02/2016   Code re-written by PH
* Version      Not released
****************************************************************************/  
void main (void)
{
   extern union LOG_UNION DATA_IN[Max_no_LOOS];
   extern union LOG_UNION *pDATA_IN;
   extern char TRAILER_ID; 
   extern int8 FLASH_REG[2];
   extern int8 ALARM_FLAG;
   extern int8 FLASH_FLAG;
   extern int8 target_USB;
   extern int8 no_of_loos;
   extern int8 max_scroll_offset;
   extern int8 *Ploo[11];
   extern int8 Loo_FREG[11];
   int8 FLT_LOG[11],PLT_LOG[11];
   extern int8 Loo_FL[11];
   int8 MUTED_FLAG = 0;
   int8 NEW_ALARM = 0;
   int8 loo;
   char text[20],FLSH_REM[4];
   int16 value16;
   int32 value32;
   int8 WEB_TO = 0;

   INIT_ALL();
   INIT_USART1();
   INIT_USART2();

   init_display();
   FLASH_REG[0] = 0;
   FLASH_REG[1] = 0;
   ALARM_FLAG = 0;
   FLASH_FLAG = 0;
   
   menu_sys();
   TRAILER_ID = read_eeprom(trailer_addr);
   if(TRAILER_ID < 'A') TRAILER_ID = 'Z';
   if(TRAILER_ID > 'Z') TRAILER_ID = 'A';

   set_init_mess();   //also checks for the USB device.
   no_of_loos = read_eeprom(no_of_units);
   initalise_USB_files(TRAILER_ID);
   set_run_display_ram();
   for(loo = 1;loo < 11;loo++)
   {
      FLT_LOG[loo] = 0;
   }
   PLT_LOG[0] = 0xFF;      // a flag to detect first time through loop.

   target_USB = RESET_TO;
   for( ; ; )
   {
      loo = 1;
      //first get all the active loo's data, and set alarms as required.
      do
      {
         pDATA_IN = &DATA_IN[loo];   //get correct loo data

         //Check for key presses etc...
         if(chk_ent()) 
         {
            MUTED_FLAG = 1;
            NEW_ALARM = 0;
         }
         if(chk_up())
         {   //scroll up
            if(scroll_offset != 0) scroll_offset--;

         }
         if(chk_dn())
         {   //scroll up
            scroll_offset++;
            if(scroll_offset > max_scroll_offset) scroll_offset = max_scroll_offset;
         }
         delay_ms(50);
         FLASHING_OFF
         if(!send_RS485(SND_DATA,loo))
         {   //This loo has a comms error, ignore everything else 
            FLT_LOG[loo] = ERR_BIT;
            (*(pLoo[loo] + flash_col)) = 'E';
            (*(pLoo[loo] + flash_col+1)) = 'R';
            (*(pLoo[loo] + flash_col+2)) = 'R';
            if(!MUTED_FLAG) FLASH_REG[Loo_FREG[loo]] = Loo_FL[loo];
            FLASHING_ON
         }
         else
         {
            FLT_LOG[loo] &= ~ERR_BIT;
            (*(pLoo[loo] + flash_col)) = ' ';
            (*(pLoo[loo] + flash_col+1)) = ' ';
            (*(pLoo[loo] + flash_col+2)) = ' ';
            FLASHING_ON
            //calculate flushes left, and any error messages
            if((pDATA_IN->LooSTATUS & OOSERVICE) == OOSERVICE)
            {

               (*(pLoo[loo] + flash_col)) = 'O';
               (*(pLoo[loo] + flash_col+1)) = 'F';
               (*(pLoo[loo] + flash_col+2)) = 'F';
               (*(PLoo[loo] + flush_rem_col)) = '-';
               (*(PLoo[loo] + flush_rem_col + 1)) = '-';
               (*(PLoo[loo] + flush_rem_col + 2 )) = '-';
            }
            else
            {

            //   value32 = (10*((int32)pDATA_IN->BAG_LEFT))/866;      //86.6 = flush length: Changed PH 05/05/18
value32 = (10*((int32)pDATA_IN->BAG_LEFT))/670;
               value16 = (int16)value32;

               if(!(FLT_LOG[loo] & ERR_BIT)) 
               {
                  convert_to_3digits (value16,&FLSH_REM);
                  (*(PLoo[loo] + flush_rem_col)) = FLSH_REM[0];
                  (*(PLoo[loo] + flush_rem_col + 1)) = FLSH_REM[1];
                  (*(PLoo[loo] + flush_rem_col + 2 )) = FLSH_REM[2];
               }
               if(((int8)pDATA_IN->LooSTATUS) & blockage)
               {
                  FLT_LOG[loo] |= BLK_BIT;   
               }
               else
               {
                  FLT_LOG[loo] &= ~BLK_BIT;
               }
                if(((int8)pDATA_IN->LooSTATUS) & SERSTATE)
               {
                  FLT_LOG[loo] |= SER_BIT;
               }
               else
               {
                  FLT_LOG[loo] &= ~SER_BIT;
               }
               if(((int8)pDATA_IN->LooSTATUS) & RUNSTATE)
               {
                  FLT_LOG[loo] |= RUN_BIT;
               }
               else
               {
                  FLT_LOG[loo] &= ~RUN_BIT;
               }
               if(((int8)pDATA_IN->LooSTATUS) & LOW_FILM)  
               {
                  FLT_LOG[loo] |= LOW_BIT;
               }
               else
               {
                  FLT_LOG[loo] &= ~LOW_BIT;
               }

               //initialise PLT_LOG.
               if(PLT_LOG[0] == 0xFF) 
               {
                  PLT_LOG[loo] = FLT_LOG[loo];
                  if(loo >= no_of_loos) PLT_LOG[0] = 0;
               }
               //now deal with any messages/faults, except COMMS
               if(FLT_LOG[loo] & BLK_BIT)
               {
                  (*(Ploo[loo] + flash_col)) = 'B';
                  (*(Ploo[loo] + flash_col+1)) = 'L';
                  (*(Ploo[loo] + flash_col+2)) = 'K';
                  if(!MUTED_FLAG) FLASH_REG[Loo_FREG[loo]] = Loo_FL[loo];
               }
               else if(FLT_LOG[loo] & RUN_BIT)
               {
                  (*(Ploo[loo] + flash_col)) = 'R';
                  (*(Ploo[loo] + flash_col+1)) = 'U';
                  (*(Ploo[loo] + flash_col+2)) = 'N';
               }
               else if(FLT_LOG[loo] & SER_BIT)
               {
                  (*(Ploo[loo] + flash_col)) = 'S';
                  (*(Ploo[loo] + flash_col+1)) = 'R';
                  (*(Ploo[loo] + flash_col+2)) = 'V';
               }
               else if(FLT_LOG[loo] & LOW_BIT)
               {
                  (*(Ploo[loo] + flash_col)) = 'L';
                  (*(Ploo[loo] + flash_col+1)) = 'O';
                  (*(Ploo[loo] + flash_col+2)) = 'W';
               }
               else
               {
                  (*(Ploo[loo] + flash_col)) = ' ';
                  (*(Ploo[loo] + flash_col+1)) = ' ';
                  (*(Ploo[loo] + flash_col+2)) = ' ';
               }
            }
         }
         loo++;
      }while(loo < no_of_loos+1);

      //check to see if we need to stop flashing
      if(MUTED_FLAG)
      {   //Stop all flashing and beeps etc.....
         FLASH_REG[0] = 0;
         FLASH_REG[1] = 0;
         ALARM_FLAG = 0;
         FLASH_FLAG = 0;
         text = "          ";
         strcpy(&display_ram[0][line1_flash_A], &text);
      }
      //check to see if there is a new fault.
      for(loo=1;loo<no_of_loos+1;loo++)
      {
         if(FLT_LOG[loo] != PLT_LOG[loo])
         {
            SEND_FLT_FLAG = 1;      //when any new fault occurs send all info to webserver.
            if(FLT_LOG[loo] & ERR_BIT)
            {
               text = "COMMS FAIL";
               NEW_ALARM = 1;
            }
            else if (FLT_LOG[loo] & BLK_BIT)
            {
               text = "BLOCKAGE  ";
               NEW_ALARM = 1;
            }
         }
         PLT_LOG[loo] = FLT_LOG[loo];         
      }
      if(NEW_ALARM)
      {   //start flashing and beeps.
         strcpy(&display_ram[0][line1_flash_A], &text);
         MUTED_FLAG = 0;
         ALARM_FLAG = 1;
         FLASH_FLAG = 1;
         FLASH_REG[0] |= BIT0;
      }
      if(check_USB_TO())
      {
         if(!NO_USB)
         {
            display_USB_warning(disp_warning);
            send_log_data_to_USB();
            display_USB_warning(rst_warning);
         }
         //WEB_TO++;
         //if((WEB_TO >= W_HOUR) || (SEND_FLT_FLAG == 1))
         //{
            WEB_TO = 0;
            SEND_FLT_FLAG = 0;
            send_data_to_WEB(&FLT_LOG[0]);
            if(no_of_loos >= 6)
            {   //wait 6 seconds and send the next block of three, if any.
               delay_ms(6000);
               send_data_to_WEB(&FLT_LOG[3]);
            }
         //}
      }
   }
}
