#include <stdint.h>
#include "msp.h"
#include "..\inc\SysTick.h"
#include "..\inc\Clock.h"

#define LED_OFF     0x00
#define LED_RED     0x01
#define LED_GREEN   0x02
#define LED_BLUE    0x04

void Port1_Init2(void){
  P1->SEL0 &= ~0x13;
  P1->SEL1 &= ~0x13;   // 1) configure P1.4  P1.1 P1.0 as GPIO
  P1->DIR &= ~0x12;    // 2) make P1.4 and P1.1 in
  P1->DIR |= 0x01;     // 2) make P1.0 out
  P1->REN |= 0x12;     // 3) enable pull resistors on P1.4 and P1.1
  P1->OUT |= 0x12;     //    P1.4 and P1.1 are pull-up
}

uint8_t Port1_Input2(void) {
  return (P1->IN & 0x12);   // read P1.4,P1.1 inputs
}

void Port1_Output2(uint8_t data) {  // write output to P1.0
  P1->OUT = (P1->OUT & 0xFE)|data;
}

void Port2_Init2(void) {
  P2->SEL0 &= ~0x07;
  P2->SEL1 &= ~0x07;    // 1) configure P2.2-P2.0 as GPIO
  P2->DIR |= 0x07;      // 2) make P2.2-P2.0 out
  P2->DS |= 0x07;       // 3) activate increased drive strength
  P2->OUT &= ~0x07;     //    all LEDs off
}

void Port2_Output2(uint8_t data) {  // write three outputs bits of P2
  P2->OUT = (P2->OUT & 0xF8)|data;
}

int main(void){
    uint8_t status;

    // Initialize SysTick timer
    SysTick_Init();

    // Initialize the 48 MHz Clock
    Clock_Init48MHz();

    // Initialize P1.1 and P1.4 and make them inputs (P1.1 and P1.4 built-in buttons)
    // and then initialize P1.0 as output to Red LED
    Port1_Init2();

    // Initialize P2.2-P2.0 and make them outputs (P2.2-P2.0 built-in LEDs)
    Port2_Init2();

    while(1) {
        // Switches are negative logic on P1.1 and P1.4
        status = Port1_Input2();
        switch (status) {
            // SW1 is pressed
            case 0x10:
                Port2_Output2(LED_OFF);
                Port1_Output2(LED_RED);
            break;
            // SW2 is pressed
            case 0x02:
                Port2_Output2(LED_BLUE);
                Port1_Output2(LED_OFF);
            break;
            // Both SW1 and SW2 are pressed
            case 0x00:
                Port2_Output2(LED_BLUE + LED_RED);
                Port1_Output2(LED_RED);
            break;
            // Neither SW1 and SW2 are pressed
            case 0x12:
                Port2_Output2(LED_OFF);
                Port1_Output2(LED_OFF);
            break;
        }
    }
}
