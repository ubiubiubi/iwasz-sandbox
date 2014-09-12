#include <msp430.h>
#include <gpio.h>

int main(void) {
        WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer
        P1DIR |= 0x01;                                  // Set P1.0 to output direction

        GPIO_setAsOutputPin (GPIO_PORT_P6, GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2 | GPIO_PIN3);

        for(;;) {
                volatile unsigned int i;        // volatile to prevent optimization

                P6OUT = GPIO_PIN0 | GPIO_PIN2 | GPIO_PIN3;
                for (i = 0; i < 10000; ++i);
                P6OUT = GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2 | GPIO_PIN3;
                for (i = 0; i < 10000; ++i);
                P6OUT = GPIO_PIN1 | GPIO_PIN2 | GPIO_PIN3;
                for (i = 0; i < 10000; ++i);
                P6OUT =  GPIO_PIN2 | GPIO_PIN3;
                for (i = 0; i < 10000; ++i);
        }

        return 0;
}


