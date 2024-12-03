#include <stdlib.h>
#include <msp430.h>
#include <stdint.h>

// Pitch
#define C4 125
#define C4# 117
#define D4 110
#define D4# 104
#define E4 99
#define F4 93
#define F4# 88
#define G4 83
#define Ab4 78
#define A4 73
#define B4b 69
#define B4 65
#define C5 62
#define C5# 58
#define D5 55
#define E5b = 52
#define E5 49
#define F5 46
#define F5# 43
#define G5 41
#define A5b 38
#define A5 36

// Duration
#define whole 1000
#define half 500
#define quar 250
#define eight 125

//typedef note Note;
struct Note {
  int pitch;
  int duration;
};

// Enum for the game state
typedef enum {
  INIT,
  COUNTDOWN,
  MAIN_GAME,
  END_SCREEN,
} state;

//Sets LEDs based on pitch
void set_leds(int pitch);

// Initializes the two user LEDs
void init_user_leds();

//Sets the two user LEDs
void set_user_leds(unsigned char uled);

// Initializes the buttons for input
void init_buttons();

//Reads the four buttons
unsigned int read_buttons();

//Turn on the buzzer
void buzzer_on(int ticks);

void runtimerA2(void);
//Sets a specific note duration to the TimerA2
//Assumption: A2 timer, Div1, and Up mode
//void note_duration(int duration) {
//	TA2CTL1 = TASSEL1 + ID_0 + MC_1;
//	TA2CCRO = duration - 1; //assume that we already converted seconds to ticks
//	TA2CCTL0 = CCIE;
//}

//
int check_input(int pitch, unsigned int user_input);
