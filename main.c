//*****************************************************************************
// main.c
// Author: jkrachey@wisc.edu
//*****************************************************************************
#include "lab_buttons.h"
#include <stdint.h>

#define MOVE_PIXELS   1

// Turn Debuggin off
#undef DEBUG

#ifdef DEBUG
#define DEBUG_PRINT(x) printf x
#else
#define DEBUG_PRINT(x) do {} while (0)
#endif

#define UP_BUTTON 0x01

#define LOCAL_ID     0x01
#define REMOTE_ID    0x00


/******************************************************************************
 * Global Variables
 *****************************************************************************/
int game_state = 0; //0: Homescreen, 1: Shootout Screen, 2: Death screen, 3: Winning screen
int host = 0;
int remote_ready = 0;
int local_ready = 0;
int countdown = 0;
int upper = 10000;
int lower = 2000;

//*****************************************************************************
//*****************************************************************************
int 
main(void)
{

	char msg[80];
  uint32_t rx_data;
  uint32_t tx_data;
  uint8_t buttons;
  
  ece210_initialize_board();
  ece210_lcd_add_msg("Wireless TEST CODE\n\r",TERMINAL_ALIGN_CENTER,LCD_COLOR_BLUE);
  sprintf(msg,"Local ID %d",LOCAL_ID);
  ece210_lcd_add_msg(msg,TERMINAL_ALIGN_CENTER,LCD_COLOR_BLUE);
  sprintf(msg,"Remote ID %d",REMOTE_ID);
  ece210_lcd_add_msg(msg,TERMINAL_ALIGN_CENTER,LCD_COLOR_BLUE);
  ece210_wireless_init(LOCAL_ID,REMOTE_ID);
	
	ece210_lcd_add_msg("Wild West Shootout!", TERMINAL_ALIGN_CENTER, LCD_COLOR_YELLOW);
	ece210_lcd_add_msg("Press Down To Begin Pairing", TERMINAL_ALIGN_CENTER, LCD_COLOR_CYAN);
  
  while(1)
  {
		if (game_state==0) { //MENU SCREEN STATE
			
			if (remote_ready == 1 && local_ready == 1) { //Host generates a random countdown and sends it to the other player
					if (host == 1) {
						countdown = 5000;
						ece210_wireless_send(countdown);
						ece210_wait_mSec( 500 );
					} else {
						countdown = ece210_wireless_get();
						ece210_wait_mSec( 500 );
					}
					ece210_lcd_add_msg("READY", TERMINAL_ALIGN_CENTER, LCD_COLOR_GREEN);
					game_state = 1;
			} else {
				
				if(ece210_wireless_data_avaiable())
				{
					rx_data = ece210_wireless_get();
					if( rx_data == UP_BUTTON )
					{
						ece210_lcd_add_msg("Pairing Message Received!", TERMINAL_ALIGN_CENTER, LCD_COLOR_GREEN);
						if (local_ready==1 && remote_ready ==0) {
							host = 1;
						}
						remote_ready = 1;
						
					}
				}
				
				if(AlertButtons)
				{
					AlertButtons = false;
					
					// Transmit data
					buttons = ece210_buttons_read();
					if(buttons == UP_BUTTON)
					{
						ece210_lcd_add_msg("Pairing Message Sent...", TERMINAL_ALIGN_CENTER, LCD_COLOR_RED);
						local_ready = 1;
						ece210_wireless_send(UP_BUTTON);
					}
				}
			}
		}
		
		if (game_state==1) { //GAMEPLAY STATE
			ece210_wait_mSec( 1 );
			countdown-=1;
			
			if (countdown==0) {
				ece210_lcd_add_msg("SHOOT!", TERMINAL_ALIGN_CENTER, LCD_COLOR_RED);
			}
		}
  }
}





