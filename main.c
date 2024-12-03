#include <stdlib.h>
#include <msp430.h>
#include "peripherals.h"
#include "functions.h"
#define SONG_LENGTH 41 //Subject to change

// Declare global variables
volatile int note_end = 0, timer = 0, current_note = 0, song_start = 0, score = 0;
volatile state game_state;

// Program the struct that holds the song - Christmas Songs!
// Mariah Carey's All I Want For Christmas Is You...
volatile struct Note scale[] = {
  [0]  = {0, 10}, //sacrificial first note
  [1]  = {G4, quar},
  [2]  = {A4, quar},
  [3]  = {B4, quar},
  [4]  = {D5, half},
  [5]  = {B4, quar},
  [6]  = {A4, quar},
  [7]  = {G4, half},
  [8]  = {G4, quar},
  [9]  = {G4, quar},
  [10]  = {A4, quar},
  [11] = {B4, quar},
  [12] = {D5, half},
  [13] = {B4, quar},
  [14] = {A4, quar},
  [15] = {G4, half},
  [16] = {F4s, quar},
  [17] = {G4, quar},
  [18] = {A4, quar},
  [19] = {B4, quar},
  [20] = {G4, half},
  [21] = {G4, quar},
  [22] = {A4, quar},
  [23] = {B4, quar},
  [24] = {D5, half},
  [25] = {C5, quar},
  [26] = {B4, quar},
  [27] = {A4, quar},
  [28] = {G4, half},
  [29] = {G4, quar},
  [30] = {G4, quar},
  [31] = {A4, quar},
  [32] = {B4, quar},
  [33] = {C5, quar},
  [34] = {D5, half},
  [35] = {C5, quar},
  [36] = {B4, quar},
  [37] = {A4, quar},
  [38] = {G4, half},
  [39] = {G4, quar},
  [40] = {A4, quar}
};

#pragma vector=TIMER2_A0_VECTOR //What does this do? No one knows...
__interrupt void timer_a2() {
  timer++;
  if (timer >= note_end && song_start) {
    BuzzerOff(); //Turn off buzzer if exceeds note duration
    if (current_note < SONG_LENGTH) { //Play until the last note
      BuzzerOff();
      current_note++;
      note_end = timer + scale[current_note].duration;
      set_leds(scale[current_note].pitch);
      buzzer_on(scale[current_note].pitch);
      //set LEDs logic
    } else {
      song_start = 0;
    }
  }
}

void main() {
  WDTCTL = WDTPW | WDTHOLD;    // Stop watchdog timer. Always need to stop this!!
  // You can then configure it properly, if desired

  // Global interrupt enable
  _BIS_SR(GIE);

  // Initialize the MSP430
  initLeds();
  init_user_leds();
  init_buttons();
  configDisplay();
  configKeypad();
  // Clear the display
  Graphics_clearDisplay(&g_sContext);
  //Start the A2 timer

  // Initialize the game_state struct and variables
  game_state = INIT;
  while (1) {
    switch(game_state) {
      case INIT: {
        BuzzerOff(); //turn off the buzzer
        song_start = 0; //Stop the song
        current_note = 0; //Reset the note tracker
        set_leds(0); //turn off the expansion LEDs
        set_user_leds(0); //turn off the user LEDs
        Graphics_clearDisplay(&g_sContext); // Clear the display
        Graphics_drawStringCentered(&g_sContext, "MSP430 HERO", 11, 48, 15, TRANSPARENT_TEXT);
        Graphics_drawStringCentered(&g_sContext, "Press * to", 10, 48, 35, TRANSPARENT_TEXT);
        Graphics_drawStringCentered(&g_sContext, "start", 5, 48, 45, TRANSPARENT_TEXT);
        Graphics_flushBuffer(&g_sContext);
        char key = 0;
        while (key == 0) {
          key = getKey();
        }
        if (key != '*')
          break;
        game_state = COUNTDOWN;
        break;
      }
      case COUNTDOWN: {
        // Start the main game timer
        runtimerA2();
        Graphics_clearDisplay(&g_sContext); // Clear the display
        Graphics_drawStringCentered(&g_sContext, "3...", 4, 48, 35, TRANSPARENT_TEXT);
        Graphics_flushBuffer(&g_sContext);
        set_user_leds(2);
        int countdown_start = timer;
        while (timer < (countdown_start + 1000))
          __no_operation();
        Graphics_clearDisplay(&g_sContext); // Clear the display
        Graphics_drawStringCentered(&g_sContext, "2...", 4, 48, 35, TRANSPARENT_TEXT);
        Graphics_flushBuffer(&g_sContext);
        set_user_leds(1);
        while (timer < (countdown_start + 2000))
          __no_operation();
        Graphics_clearDisplay(&g_sContext); // Clear the display
        Graphics_drawStringCentered(&g_sContext, "1...", 4, 48, 35, TRANSPARENT_TEXT);
        Graphics_flushBuffer(&g_sContext);
        set_user_leds(0);
        while (timer < (countdown_start + 3000))
          __no_operation();
        Graphics_clearDisplay(&g_sContext); // Clear the display
        Graphics_drawStringCentered(&g_sContext, "GO!", 3, 48, 35, TRANSPARENT_TEXT);
        Graphics_flushBuffer(&g_sContext);
        set_user_leds(3);
        while (timer < (countdown_start + 4000))
          __no_operation();
        game_state = MAIN_GAME;
        set_user_leds(0);
        break;
        }

      case MAIN_GAME: {
          song_start = 1;
        char key = 0;
        unsigned int user_input = 0;
        int temp_current_note = current_note;
        while (1) {
          key = getKey();
          user_input = read_buttons();
          if ((key || user_input) || (temp_current_note != current_note ))
            break;
        }
        if (key == '#') {
          BuzzerOff();
          song_start = 0;
          current_note = 0;
          game_state = INIT;
          break;
        } else if (current_note >= SONG_LENGTH) {
          game_state = END_SCREEN;
          break;
        }
        //Check (for accuracy) user input
        if (user_input != 0) {
          score += check_input(scale[current_note].pitch, user_input);
        }

        if (score < current_note - 5) {
          BuzzerOff();
          song_start = 0;
          current_note = 0;
          game_state = END_SCREEN;
          break;
        }
        break;
      }
    case END_SCREEN: {
      BuzzerOff(); //turn off the buzzer
      int score_tens, score_ones;
      score_tens = ((score - (score / 10)) % 10) + '0';
      score_ones = (score % 10) + '0';
      char disp[3] = {score_tens, score_ones, '\0'};
      song_start = 0; //Stop the song
      set_leds(0); //turn off the expansion LEDs
      set_user_leds(0); //turn off the user LEDs
      Graphics_clearDisplay(&g_sContext); // Clear the display
      Graphics_drawStringCentered(&g_sContext, "MSP430 HERO", 11, 48, 15, TRANSPARENT_TEXT);
      if (score >= SONG_LENGTH - 5) {
        Graphics_drawStringCentered(&g_sContext, "You WON!", 10, 48, 35, TRANSPARENT_TEXT);
      } else {
        Graphics_drawStringCentered(&g_sContext, "You LOST!", 10, 48, 35, TRANSPARENT_TEXT);
      }
      Graphics_drawStringCentered(&g_sContext, "Your score:", 11, 48, 55, TRANSPARENT_TEXT);
      Graphics_drawStringCentered(&g_sContext, disp, 3, 48, 75, TRANSPARENT_TEXT);
//      Graphics_drawStringCentered(&g_sContext, "Press # to", 6, 48, 95, TRANSPARENT_TEXT);
//      Graphics_drawStringCentered(&g_sContext, "exit", 4, 48, 95, TRANSPARENT_TEXT);
      Graphics_flushBuffer(&g_sContext);
      char key = 0;
      while (key == 0) {
        key = getKey();
      }
      if (key != '#')
        break;
      game_state = INIT;
      break;
    }
    }
  }
}
