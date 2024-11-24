#include <stdlib.h>
#include <msp430.h>
#include "peripherals.h"
#include "functions.h"

// Use an enum as the game state
typedef enum{
  INIT,
  MAIN_GAME,
  END_SCREEN,
} state;

// Define a struct to hold note information
struct Note {
  int pitch;
  int duration;
};

__interrupt void timer_a2()
{
  timer++;
  if (timer >= note_end && song_start) {
    BuzzerOff;
    note_end = timer + scale[current_note].duration;
    buzzer_on(scale[current_note].pitch);
    current_note++;
  }
}
// Declare global variables
int note_end = 0, timer = 0, current_note = 0, song_start = 0;
void main()
{
  WDTCTL = WDTPW | WDTHOLD;    // Stop watchdog timer. Always need to stop this!!
  // You can then configure it properly, if desired

  // Initialize the MSP430
  initLeds();
  init_user_leds();
  init_buttons();
  configDisplay();
  configKeypad();
  // Clear the display
  Graphics_clearDisplay(&g_sContext);
  //Start the A2 timer

  // Program the struct that holds the song
  struct Note scale[] {
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

  // Initialize the game_state struct and variables
  state game_state = INIT;
  while (1) {
    switch(game_state) {
      case INIT: {
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
        game_state = MAIN_GAME;
        break;
      }
    case MAIN_GAME: {
      // Start the main game timer
      runtimerA2();
      Graphics_clearDisplay(&g_sContext); // Clear the display
      Graphics_drawStringCentered(&g_sContext, "3...", 4, 48, 35, TRANSPARENT_TEXT);
      Graphics_flushBuffer(&g_sContext);
      int countdown = timer;
      while (countdown < 1000)
        __no_operation();
      Graphics_clearDisplay(&g_sContext); // Clear the display
      Graphics_drawStringCentered(&g_sContext, "2...", 4, 48, 35, TRANSPARENT_TEXT);
      Graphics_flushBuffer(&g_sContext);
      while (countdown < 2000)
        __no_operation();
      Graphics_clearDisplay(&g_sContext); // Clear the display
      Graphics_drawStringCentered(&g_sContext, "1...", 4, 48, 35, TRANSPARENT_TEXT);
      Graphics_flushBuffer(&g_sContext);
      while (countdown < 3000)
        __no_operation();
      Graphics_clearDisplay(&g_sContext); // Clear the display
      Graphics_drawStringCentered(&g_sContext, "GO!", 3, 48, 35, TRANSPARENT_TEXT);
      Graphics_flushBuffer(&g_sContext);
      while (countdown < 4000)
        __no_operation();
      song_start = 1;
      //
    }
    }
  }
/*  while(1) {
    switch(game_state) {
    case INIT: {
      // Display the starting text
      Graphics_clearDisplay(&g_sContext); // Clear the display
      Graphics_drawStringCentered(&g_sContext, "SIMON", 5, 48, 15, TRANSPARENT_TEXT);
      Graphics_drawStringCentered(&g_sContext, "Press * to", 10, 48, 35, TRANSPARENT_TEXT);
      Graphics_drawStringCentered(&g_sContext, "start", 5, 48, 45, TRANSPARENT_TEXT);
      Graphics_flushBuffer(&g_sContext);
      // Initialize the loop counter
      loop_num = 1;
      // Initialize the answer array to random characters 1, 2, 3, 4.
      for (i = 0; i < ARRAY_LEN; i++) {
        // Generate random numbers, take the remainder, then add an offset to convert to the correct representation as char
        answer_key[i] = (rand() % 4) + '1';
      }
      // Wait in "Press any key to start"
      char ans =get_input();
      if (ans != '*')
        break;
      fake_delay(1);
      Graphics_clearDisplay(&g_sContext); // Clear the display
      Graphics_drawStringCentered(&g_sContext, "3...", 4, 48, 35, TRANSPARENT_TEXT);
      Graphics_flushBuffer(&g_sContext);
      fake_delay(1);
      Graphics_clearDisplay(&g_sContext); // Clear the display
      Graphics_drawStringCentered(&g_sContext, "2...", 4, 48, 35, TRANSPARENT_TEXT);
      Graphics_flushBuffer(&g_sContext);
      fake_delay(1);
      Graphics_clearDisplay(&g_sContext); // Clear the display
      Graphics_drawStringCentered(&g_sContext, "1...", 4, 48, 35, TRANSPARENT_TEXT);
      Graphics_flushBuffer(&g_sContext);
      fake_delay(1);
      game_state = MAIN_GAME;
      break;
    }
    case MAIN_GAME: {
      Graphics_clearDisplay(&g_sContext); // Clear the display
      Graphics_drawStringCentered(&g_sContext, "Watch closely...", 16, 48, 35, TRANSPARENT_TEXT);
      Graphics_flushBuffer(&g_sContext);
      set_leds(0);
      fake_delay(1);
      // Enter loop for N times
      for (i = 0; i < loop_num; i++) {
        // Blink LED
        set_leds(answer_key[i]);
        // Sound Buzzer
        buzzer_on(answer_key[i]);
        // WRITE A FUNCTION FOR SPECIFIC NOTES
        fake_delay(7);
        BuzzerOff();
        set_leds('0');
        fake_delay(4);
      }
      fake_delay(2);
      set_leds('0');
      game_state = WAIT_FOR_INPUT;
      break;
    }
    case WAIT_FOR_INPUT: {
      Graphics_clearDisplay(&g_sContext); // Clear the display
      Graphics_drawStringCentered(&g_sContext, "now input", 9, 48, 35, TRANSPARENT_TEXT);
      Graphics_flushBuffer(&g_sContext);
      // Enter loop for N times
      for (i = 0; i < loop_num; i++) {
        // Store user input
        char ans = get_input();
        char disp[2] = {ans, '\0'};
        Graphics_clearDisplay(&g_sContext); // Clear the display
        Graphics_drawStringCentered(&g_sContext, disp, 1, 48, 35, TRANSPARENT_TEXT);
        Graphics_flushBuffer(&g_sContext);
        // Set LEDs
        set_leds(ans);
        // Play buzzer
        buzzer_on(ans);
        fake_delay(1);
        BuzzerOff();
        if (ans != answer_key[i]) {
          fake_delay(1);
          game_state = INPUT_FALSE;
          break;
        }
      }
      if (game_state != INPUT_FALSE) {
        fake_delay(1);
        game_state = INPUT_TRUE;
      }
      break;
    }
    case INPUT_TRUE: {
      // Display Correct!
      Graphics_clearDisplay(&g_sContext); // Clear the display
      Graphics_drawStringCentered(&g_sContext, "Correct!", 8, 48, 15, TRANSPARENT_TEXT);
      Graphics_flushBuffer(&g_sContext);
      // Increment counter
      loop_num++;
      fake_delay(6);
      // Branch to OUTPUT
      game_state = OUTPUT;
      break;
    }
    case INPUT_FALSE: {
      // Display WRONG
      Graphics_clearDisplay(&g_sContext); // Clear the display
      Graphics_drawStringCentered(&g_sContext, "WRONG!", 6, 48, 15, TRANSPARENT_TEXT);
      Graphics_flushBuffer(&g_sContext);
      // Branch to GAME_OVER
      fake_delay(4);
      game_state = GAME_OVER;
      break;
    }
    case GAME_OVER: {
      set_leds('0'); // Clear LEDs
      Graphics_clearDisplay(&g_sContext); // Clear the display
      Graphics_drawStringCentered(&g_sContext, "GAME OVER", 9, 48, 15, TRANSPARENT_TEXT);
      Graphics_flushBuffer(&g_sContext);
      fake_delay(3);
      // Display Play again?
      Graphics_drawStringCentered(&g_sContext, "Play again?", 12, 48, 45, TRANSPARENT_TEXT);
      Graphics_flushBuffer(&g_sContext);
      // Branch to INIT
      get_input();
      game_state = INIT;
      // Display Quit?
      // Exit
      // Timeout after 5 seconds
    }
    }
  } */
}
