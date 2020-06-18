/***************************************************************************************
* Name:      LW_Controller_user_set_up.h
*
* Revision: draft
*
* Author:     Phil Hockley
*
* Description:
*          prototype header file.
*
* History:    19/04/17   Initial code started
*
****************************************************************************************/
#define BAG_LENGTH 16000      //mm
#define KNOT_LEN  1500         //mm
#define LOW_LIMIT 1000          //Bag low limit in mm (note Knot length already taken into account)
#define STATE_CHANGED_LIMIT 2   //0.2 second: Time taken before the state will actually change (it will beep)
#define RESET_STATE_LIMIT 200    //2 seconds: Time taken before the unit will reset the bag length (it will beep)
#define max_long_flushes  1     //allow one long flush
#define max_short_flushes 10    //allow 10 further short flushes
#define Flush_release_time 100    //10mS steps: Flush button must be released before next flush is allowed
#define flush_TO_limit 1000      //10mS steps: time between all allowed flushes, and next flush
                        //i.e. once all long/short flushes done you can't use flush button until
                        //time out has expired.
#define last_press_timeout 2000   //10mS steps: Resets number of short/long flushes if not used for timeout period.
#define flash_slow_on   10       //100mS steps: Next four are Flash times for the RUN LED.
#define flash_slow_period 20    //100mS steps
#define flash_fast_on   5       //100mS steps
#define flash_fast_period 10    //100mS steps
#define beep_cmode 5         //100mS steps: beep time when changing mode
#define beep_reset 10         //100mS steps: beep time when resetting bag

//NOTE - changing the below will also effect the attendant unit.
#define long_flush   880   //encoder pulses: bag length used mm = pulses_in / bag_used_constant;
#define short_flush 880  //encoder pulses: bag length used mm = pulses_in / bag_used_constant;
