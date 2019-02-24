#include "globals.h"
#include "address_map_arm.h"

/* globals used for audio record/playback */
extern volatile int record, play, buffer_index;
extern volatile int left_buffer[];
extern volatile int right_buffer[];
extern volatile int popSong[];

/***************************************************************************************
 * Audio interrupt service routine
****************************************************************************************/
void audio_ISR( void )
{
	volatile int * audio_ptr = (int *) AUDIO_BASE;		// audio port address
  	volatile int * red_LED_ptr = (int *) LEDR_BASE;		// red LED address
	
	int fifospace;


	if (*(audio_ptr) & 0x200)						// check bit WI of the Control register
	{
		if (buffer_index == 0)
			*(red_LED_ptr) = 0x2;					// turn on LEDR_1
		fifospace = *(audio_ptr + 1);	 			// read the audio port fifospace register
		// output data until the buffer is empty or the audio-out FIFO is full
		while ( (fifospace & 0x00FF0000) && (buffer_index < 2*BUF_SIZE_POP-1) )
		{
			*(audio_ptr + 2) = popSong[buffer_index/2];
			*(audio_ptr + 3) = popSong[buffer_index/2];
			++buffer_index;
	
			if (buffer_index == BUF_SIZE)
			{
				// done playback
				play = 0;
				*(red_LED_ptr) = 0x0;				// turn off LEDR
				*(audio_ptr) = 0x0; 					// turn off interrupts
			}
			fifospace = *(audio_ptr + 1);	// read the audio port fifospace register
		}
	}
	return;
}
