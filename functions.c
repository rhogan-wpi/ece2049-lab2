#include "functions.h"
// User written functions

// Initializes the two user LEDs
void init_user_leds()
{
  //Set digital I/O mode for pins 1.0 and 4.7
  P4SEL &= ~BIT7;
  P1SEL &= ~BIT0;
  //Set the same pins as outputs
  P4DIR |= BIT7;
  P1DIR |= BIT0;
}

//Sets the two user LEDs
void set_user_leds(unsigned char uled)
{
  //zero outputs
  P4OUT &= ~BIT7;
  P1OUT &= ~BIT0;
  //set outputs
  P4OUT = ((uled & BIT1) << 6);
  P1OUT = (uled & BIT0);
}

// Initializes the buttons for input
void init_buttons()
{
  // Set digital I/O mode for pins 3.6, 7.0, 7.4 and 2.2
  P3SEL &= ~BIT6;
  P7SEL &= ~(BIT4|BIT0);
  P2SEL &= ~BIT2;
  // Set the same pins as inputs
  P3DIR &= ~BIT6;
  P7DIR &= ~(BIT4|BIT0);
  P2DIR &= ~BIT2;
  // Set pull resistors on the pins
  P3REN |= BIT6;
  P7REN |= (BIT4|BIT0);
  P2REN |= BIT2;
  // Set them to be pull-up resistors
  P3OUT |= BIT6;
  P7OUT |= (BIT4|BIT0);
  P2OUT |= BIT2;
  }

//Reads the four buttons
unsigned char read_buttons()
{
  unsigned char pressed = 0;
  pressed |= (P7IN & BIT0);
  pressed |= ((P3IN & BIT6) >> 5);
  pressed |= (P2IN & BIT2);
  pressed |= ((P7IN & BIT4) >>1);
  return pressed;
}

//Turn on the buzzer
void buzzer_on(int ticks)
{
/* PWM period in ticks for each note
A = 73
Bb = 69
B = 65
C = 62
C# = 58
D = 55
Eb = 52
E = 49
F = 46
F# = 43
G = 41
Ab = 38
A octave = 36
*/

  // Initialize PWM output on P3.5, which corresponds to TB0.5
  P3SEL |= BIT5; // Select peripheral output mode for P3.5
  P3DIR |= BIT5;

  TB0CTL  = (TBSSEL__ACLK|ID__1|MC__UP);  // Configure Timer B0 to use ACLK, divide by 1, up mode
  TB0CTL  &= ~TBIE;                       // Explicitly Disable timer interrupts for safety

  // Now configure the timer period, which controls the PWM period
  // Doing this with a hard coded values is NOT the best method
  // We do it here only as an example. You will fix this in Lab 2.
  //
  TB0CCR0 = ticks;                    // Set the PWM period in ACLK ticks
  TB0CCTL0 &= ~CCIE;                  // Disable timer interrupts

  // Configure CC register 5, which is connected to our PWM pin TB0.5
  TB0CCTL5  = OUTMOD_7;                   // Set/reset mode for PWM
  TB0CCTL5 &= ~CCIE;                      // Disable capture/compare interrupts
  TB0CCR5   = TB0CCR0/2;                  // Configure a 50% duty cycle
}

//Global timer A2 for note duration, interrupt timing is 0.001007 seconds
void runtimerA2(void)
{
  TA2CTL = TASSEL_1 | MC_1 | ID_0;
  TA2CCR0 = 32;
  TA2CCTL0 = CCIE;
}

