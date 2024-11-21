#include <stdlib.h>

//Define your struct here

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
