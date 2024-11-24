#include <stdlib.h>

//note struct defintion
//struct note {
///int duration; //in ticks
///    int pitch; //in ticks
//};

//typedef note Note;

// Initializes the two user LEDs
void init_user_leds();

//Sets the two user LEDs
void set_user_leds(unsigned char uled);

// Initializes the buttons for input
void init_buttons();

//Reads the four buttons
unsigned char read_buttons();

//Turn on the buzzer
void buzzer_on(int frequency);

//Sets a specific note duration to the TimerA2
//Assumption: A2 timer, Div1, and Up mode
//void note_duration(int duration) {
//	TA2CTL1 = TASSEL1 + ID_0 + MC_1;
//	TA2CCRO = duration - 1; //assume that we already converted seconds to ticks
//	TA2CCTL0 = CCIE;
//}
