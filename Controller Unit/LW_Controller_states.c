/***************************************************************************************
* Name:   	LW_Controller_states.C
* 
* Revision: draft
*
* Author:  	Phil Hockley
*
* Description:
*		 	Control PCB functions.
*
* History: 	12/04/17	Initial code started
*			
****************************************************************************************/
int8 run_mode (state_in)
{
	extern int8 IO_STATE;
	extern int16 TL_DATA[TX_MESS_WORDS];
	extern int1 batt_low_flag;

	//Cannot assume bag has been changed (could have looped through all modes)
	//so only reset function can reset the log data etc.. This only works in SERVICE MODE.

	reset_flash();
	IO_STATE = RESET_IO;
	IO_STATE = (FLUSH | MODE_RUN);
	LooSTATUS &= RST_STATE;
	LooSTATUS |= RUNSTATE;

	for( ; ; )
	{
		wait_for_tick();
		if(batt_low_flag) return SERVICE;
		UPDATE_IO(IO_STATE);

		if(check_flush() == BLOCKED)
		{	//unit blocked, log number of blocks, and go to service mode
			LooSTATUS |= blockage;
			LooSTATUS &= RST_STATE;
			LooSTATUS |= SERSTATE;
			NO_BLOCKS++;
			IO_STATE &= FLUSH;
			return SERVICE;
		}
		state_in = check_mode(state_in);
		if(state_in != RUNNING) return state_in;

		state_in = check_fwd (state_in);
		if(state_in == BLOCKED)
		{	//unit blocked, log number of blocks, and go to service mode
			LooSTATUS |= blockage;
			LooSTATUS &= RST_STATE;
			LooSTATUS |= SERSTATE;
			NO_BLOCKS++;
			IO_STATE &= FLUSH;
			return SERVICE;
		}
		state_in = check_bck (state_in);
		if(state_in == BLOCKED)
		{	//unit blocked, log number of blocks, and go to service mode
			LooSTATUS |= blockage;
			LooSTATUS &= RST_STATE;
			LooSTATUS |= SERSTATE;
			NO_BLOCKS++;
			IO_STATE &= FLUSH;
			return SERVICE;
		}

		//now check for low film, or out of film
		if(BAG_LEFT < LOW_LIMIT)
		{	//Bag low or out.
			if(BAG_LEFT <= 40)
			{	//bag is out
				LooSTATUS &= RST_STATE;
				LooSTATUS |= SERSTATE;
				return SERVICE;
			}
			else if((LooSTATUS & LOW_FILM) !=LOW_FILM)
			{	//Bag is low, only start flashing once though.
				set_flash(flash_slow_on, flash_slow_period, LED_RUN);		//slow flash in RUN mode
				LooSTATUS |= LOW_FILM;
			}
		}
	}
}

int8 service_mode (state_in)
{
	extern int8 IO_STATE;

	reset_flash();
	IO_STATE = RESET_IO;
	IO_STATE |= MODE_SER;
	LooSTATUS &= RST_STATE;
	LooSTATUS |= SERSTATE;

	for( ; ; )
	{
		wait_for_tick();
		UPDATE_IO(IO_STATE);
		check_fwd (state_in);
		check_bck (state_in);
		state_in = check_mode(state_in);
		if(state_in != SERVICE)
		{
			LooSTATUS &= ~blockage;	//clear any blockages
			return state_in;
		}
		state_in = check_reset (state_in);
		if(state_in == RUNNING) 
		{	//reset all to start a new bag)
			reset_all_log_data();
			//reset the bag
			BAG_USED = 0;			//Total bag used
			BAG_LEFT = BAG_LENGTH - KNOT_LEN;
			NO_RESETS++;
			LooSTATUS = RUNSTATE;
			return RUNNING;
		}
	}
}



int8 oo_service_mode (state_in)
{	//only way out is by going to next state, but without updating anything!!

	extern int8 IO_STATE;
	extern int1 batt_low_flag;

	reset_flash();
	IO_STATE = RESET_IO;
	IO_STATE |= MODE_OFF;
	LooSTATUS &= RST_STATE;
	LooSTATUS |= OOSERVICE;

	for( ; ; )
	{
		wait_for_tick();
		if(batt_low_flag) return SERVICE;
		UPDATE_IO(IO_STATE);
		state_in = check_mode(state_in);
		if(state_in != OO_SERVICE)
		{
			return state_in;
		}
	}
}

