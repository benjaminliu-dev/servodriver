#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint16_t pulse[3] = {1500, 1500, 1500}; // µs

volatile uint8_t state = 0;

void setup() {
  // Set PA1, PA2, PA3 as output
  PORTA.DIRSET = PIN1_bm | PIN2_bm | PIN3_bm;

  // TCB0: periodic interrupt
  TCB0.CCMP = 20000; // 20ms @ 1MHz (prescaler used below)
  TCB0.CTRLA = TCB_CLKSEL_DIV2_gc | TCB_ENABLE_bm;
  TCB0.INTCTRL = TCB_CAPT_bm;

  sei();
}

ISR(TCB0_INT_vect) {
  TCB0.INTFLAGS = TCB_CAPT_bm;

  static uint8_t servo = 0;

  switch (state) {
    case 0:
      PORTA.OUTSET = PIN1_bm | PIN2_bm | PIN3_bm;
      servo = 0;
      state = 1;
      TCB0.CCMP = pulse[0];
      break;

    case 1:
      PORTA.OUTCLR = (1 << (servo + 1));
      servo++;

      if (servo < 3) {
        TCB0.CCMP = pulse[servo];
      } else {
        TCB0.CCMP = 20000;
        state = 0;
      }
      break;
  }
}

void loop() {
  if (USART0.STATUS & USART_RXCIF_bm) {
    char id = USART0.RXDATAL;
    if (USART0.RXDATAL == ':') {
      uint16_t val = 0;
      char c;

      while (!(USART0.STATUS & USART_RXCIF_bm));
      while ((c = USART0.RXDATAL) != '\n') {
        if (c >= '0' && c <= '9')
          val = val * 10 + (c - '0');
        while (!(USART0.STATUS & USART_RXCIF_bm));
      }

      if (val < 1000) val = 1000;
      if (val > 2000) val = 2000;

      if (id >= '1' && id <= '3')
        pulse[id - '1'] = val;
    }
  }
}
