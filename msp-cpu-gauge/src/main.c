#include <msp430.h>
#include <gpio.h>

/**
 * Uwaga - mylące nazwy :
 * TAxCTL - główny rejestr kontrolny.
 * TAxCCTL[0-6] - Timer_Ax Capture/Compare Control 0-6
 * TAxCCR[0-6] - Timer_Ax Capture/Compare 0-6. Mają podwójną funkcję. Kiedy pracują jako 'capture', współpracują z jakimś wejściem. Gdy
 *               na tym wejściu pojawi się zdarzenie, wartość z licznika TAxR jest kopiowana do rejestru.
 *               W trybie compare jest odwrotnie. Do rejestrów TAxCCR zapisujemy jakąś wartość, która jest porównywana z licznikiem TAxR.
 *               Gdy są równe, generowane jest zdarzenie (zazwyczaj zmiana stanu jakiegoś wyjścia i/lub przerwanie). W ten sposób generujemy
 *               PWM.
 */
int main(void) {
        // Stop watchdog timer
        WDTCTL = WDTPW | WDTHOLD;

        /*
         * Konfiguracja pierwszego timera typu A. Wybór źródła sygnału zegarowego za pomocą :
         * TASSEL_0 : TAxCLK sygnał zewnętrzny. ?
         * TASSEL_1 : ACLK sygnał wewnętrzny. Auxiliary clock.
         * TASSEL_2 : SMCLK sygnał wewnętrzny. Subsystem master clock.
         * TASSEL_3 : INCLK sygnał zewnętrzny. ?
         *
         */
        TA0CTL |= TASSEL_2 |

        /*
         * Pierwszy divider. Możliwe opcje to:
         * ID_0 : /1
         * ID_1 : /2
         * ID_2 : /4
         * ID_3 : /8
         */
        ID_0 |

        /*
         * Tryb działania
         * Timer mode control: 0 - Stop
         *
         * Timer mode control: 1 - Up to TAxCCR0 i znów od 0. W tym przypadku używany jest tylko TAxCCR0 (Timer_Ax Capture/Compare 0).
         *                         Kiedy timer dojdzie do wartości TAxCCR0, to ustawiany jest bit CCIFG w rejestrze TAxCCTL0. Natomiast
         *                         zaraz potem, kiedy  tylko timer się wyzeruje µC ustawia bit TAIFG w rejestrze TAxCTL (Timer_Ax Control).
         *                         Czyli te zdarzenia następują zaraz po sobie.
         *
         * Timer mode control: 2 - Continuous up, czyli liczy do 0xffff i znów od zera. Kiedy dojdzie do 0xffff, to ustawia TAIFG w TAxCTL
         *                         (Timer_Ax Control), tak samo jak w poprzednim wypadku.
         *
         * Timer mode control: 3 - Up/Down, cyzli od 0 do TAxCCR0 i potem do 0 i w kółko.
         *
         */
        MC_1;

        /*
         * Dalszy podział sygnału zegarowego. Możliwe wartości:
         * TAIDEX_0 : /1
         * TAIDEX_1 : /2
         * TAIDEX_2 : /3
         * TAIDEX_3 : /4
         * TAIDEX_4 : /5
         * TAIDEX_5 : /6
         * TAIDEX_6 : /7
         * TAIDEX_7 : /8
         */
        TA0EX0 = TAIDEX_4;

        // Można ustawić aktualny stan timera, ale nie trzeba.
        TA0R = 0;

        /*
         * Reset timera. Zawsze wykonać po ustawieniu dzielników sygnału zegarowego.
         */
        TA0CTL |= TACLR;


/*---------------------------------------------------------------------------*/













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


