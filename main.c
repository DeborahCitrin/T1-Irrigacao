#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

//acender: PORTB |= (1 << PB0)
//apagar: PORTB &= (0 << PB0)

// Rotina de tratamento da interrupção PCINT0
int leds[5] = {0};
unsigned int ledsAcesos = 0;
unsigned int modo = 1; // 0 = desligar | 1 = ligar
unsigned int ledsLigados = 5;
unsigned int contador5 = 0;
unsigned int contador3 = 0;

ISR(PCINT0_vect) //interrupção de quando o botão é apertado
{
    // if (PINB & (1 << PB0)) { //se o botão foi pressionado
    //     if (modo == 1) modo = 0;
    //     else if (modo == 0) modo = 1;
    //     contador3 = 0;
    //     contador5 = 0;
    // }
    // if (modo == 1) modo = 0;
    // else modo = 1;
    modo ^= 1;  // muda o modo
}

void mudaLED(int led)
{
    leds[led] ^= 1;

    if (leds[led] == 1) {
        PORTB |= (1 << led);        // acende led
    }
    else PORTB &= ~(1 << led);      // apaga led
}

int main(void)
{
    DDRB |= (1 << PB1) | (1 << PB2) | (1 << PB3) | (1 << PB4) | (1 << PB5);      // seta PB1 a PB5 saída
    DDRD &= ~(1 << PD2);
    PORTD |= (1 << PD2);

    // Configuração de interrupção externa INT0 (pino PD2)
    EICRA |= (1 << ISC00);    // Interrupção na borda de descida
    EIMSK |= (1 << INT0);

    // DDRB &= ~(1 << PB0);            //seta PB0 como entrada
    // PORTB &= ~(1 << PB0);          // desabilita pull-up de PB1
    
    // PCICR |= (1 << PCIE0);         // habilita vetor de interrupção para PB7 .. PB0
    // PCMSK0 |= (1 << PCINT0);       // habilita interrupção para PB1

    // habilita interrupções
	sei();


    while (1)
    {
        if (modo == 1)      // acender LEDs
        {
            for (int i = 0; i < 5; i++) {
                if (leds[i] == 0) {
                    mudaLED(i);
                    _delay_ms(500);
                }
                if (modo == 0) break;
            }
        }

        if (modo == 0)      // apagar LEDs
        {
            for (int i = 4; i >= 0; i--) {
                if (leds[i] == 1) {
                    mudaLED(i);
                    _delay_ms(1000);
                }
                if (modo == 1) break;
            }
        }
    }
}