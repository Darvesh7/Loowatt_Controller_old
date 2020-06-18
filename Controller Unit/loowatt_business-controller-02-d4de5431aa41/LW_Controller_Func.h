/***************************************************************************************
* Name:   	LW_Controller_Func.h
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
void INIT_ALL (void);
void INIT_ATTENDANT_USART (void);
void INIT_ATTENDANT_USART (void);
void INIT_BOOT_USART (void);
void reset_all_log_data(void);
void wait_for_tick (void);
void CHK_BATT (void);
void UPDATE_IO(int8 IO_STATE);
void Motor_FWD (int16 init_count);
void Motor_BACK (int16 init_count);
void Motor_STOP (void);
int8 check_fwd(int8 state_in);
int8 check_bck(int8 state_in);
int8 check_rst(int8 state_in);
int8 check_mode(int8 state_in);
int16 cal_bagused(int16 pulses_in);
int8 check_stalled (int16 time_in,int16 init_count);
void log_flush_bag_used(int16 flush_type);
void set_beep (int16 time, int16 rate, B_mode);
void reset_beep (void);
void set_flash (int16 time, int16 rate, int16 what);  //, int1 type);
void reset_flash (void);
int8 activate_flush (int16 f_length);
int1 check_motor_over_I (void);