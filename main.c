#include <stdlib.h>
#include <msp430.h>
#include "peripherals.h"
#include "functions.h"
#define SONG_LENGTH 28 //Subject to change

// Declare global variables
volatile int note_end = 0, timer = 0, current_note = 0, song_start = 0;
volatile unsigned int score = 0;
volatile state game_state;

// Program the struct that holds the song - Christmas Songs!
volatile struct Note scale[] = {
  [0]  = {73,1000},
  [1]  = {69,1000},
  [2]  = {65,1000},
  [3]  = {62,1000},
  [4]  = {58,1000},
  [5]  = {55,1000},
  [6]  = {52,1000},
  [7]  = {49,1000},
  [8]  = {46,1000},
  [9]  = {43,1000},
  [10] = {41,1000},
  [11] = {38,1000},
  [12] = {36,1000}
};

#pragma vector=TIMER2_A0_VECTOR //What does this do? No one knows...
__interrupt void timer_a2() {
  timer++;
  if (timer >= note_end && song_start) {
    BuzzerOff; //Turn off buzzer if exceeds note duration
    if (current_note < SONG_LENGTH) { //Play until the last note
      BuzzerOff();
      note_end = timer + scale[current_note].duration;
      set_leds(scale[current_note].pitch);
      buzzer_on(scale[current_note].pitch);
      //set LEDs logic
      current_note++;
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
/*   if (key == '#') {
            BuzzerOff();
            song_start = 0;
            current_note = 0;
            game_state = INIT;
            break;
          }
          song_start = 1;
          char key = 0;
          char user_input = 0;
          while ((key && user_input) == 0) {
            key = getKey();
            user_input = read_buttons();
          } */ /*
        //Push button polling logic
        while(song_start) {
          //If the player hit the note
          if(current_note < SONG_LENGTH && timer <= note_end + 100) {
            unsigned char button = read_buttons();
            set_user_leds(button);
            score ++;
          }
        }
        */
        char key = 0;
        unsigned int user_input = 0;
        key = getKey();
        user_input = read_buttons();
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
      }
        break;
    case END_SCREEN: {
      BuzzerOff(); //turn off the buzzer
      song_start = 0; //Stop the song
      set_leds(0); //turn off the expansion LEDs
      set_user_leds(0); //turn off the user LEDs
      Graphics_clearDisplay(&g_sContext); // Clear the display
      Graphics_drawStringCentered(&g_sContext, "MSP430 HERO", 11, 48, 15, TRANSPARENT_TEXT);
      if (score >= SONG_LENGTH - 5) {
        Graphics_drawStringCentered(&g_sContext, "You WON!", 10, 48, 15, TRANSPARENT_TEXT);
      } else {
        Graphics_drawStringCentered(&g_sContext, "You LOST!", 10, 48, 15, TRANSPARENT_TEXT);
      }
      Graphics_drawStringCentered(&g_sContext, "Your score:", 8, 48, 15, TRANSPARENT_TEXT);
      Graphics_drawStringCentered(&g_sContext, (char)(score), 7, 48, 15, TRANSPARENT_TEXT);
      Graphics_drawStringCentered(&g_sContext, "Press # to", 6, 48, 35, TRANSPARENT_TEXT);
      Graphics_drawStringCentered(&g_sContext, "exit", 4, 48, 45, TRANSPARENT_TEXT);
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
