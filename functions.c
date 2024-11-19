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
  P4OUT |= ((uled & BIT1) << 6);
  P1OUT |= (uled & BIT0);
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

