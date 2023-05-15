#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

//acender: PORTB |= (1 << PB0)
//apagar: PORTB &= (0 << PB0)

// Rotina de tratamento da interrupção PCINT0
unsigned int ledsAcesos = 0;
unsigned int modo = 1; // 0 = desligar | 1 = ligar
unsigned int ledsLigados = 5;
unsigned int contador5 = 0;
unsigned int contador3 = 0;

ISR(PCINT0_vect) //interrupção de quando o botão é apertado
{
    if (PINB & (1 << PB0)) { //se o botão foi pressionado
        if (modo == 1) modo = 0;
        else if (modo == 0) modo = 1;
        contador3 = 0;
        contador5 = 0;
    }
}

int main(void)
{
    DDRB |= (1 << PB1) | (1 << PB2) | (1 << PB3) | (1 << PB4) | (1 << PB5);      // seta PB1 a PB5 saída
    DDRB &= ~(1 << PB0);            //seta PB0 como entrada
    PORTB &= ~(1 << PB0);          // desabilita pull-up de PB1
    
    PCICR |= (1 << PCIE0);         // habilita vetor de interrupção para PB7 .. PB0
    PCMSK0 |= (1 << PCINT0);       // habilita interrupção para PB1

    // habilita interrupções
	sei();


    while (1)
    {
        if (modo == 0) { //desligando leds
            //modo = 1;
            contador3 = 0;
            if(contador5 == 25){
                contador5 = 0;
                if (ledsLigados == 5)
                {
                    //_delay_ms(2000); - timer ao invés de delay - como fazer?
                    PORTB ^= (1 << PB5);
                    ledsLigados--;
                    ledsAcesos = 4;
                }
                

                if (ledsLigados == 4)
                {
                    //_delay_ms(2000);
                    PORTB ^= (1 << PB4);
                    ledsLigados--;
                    ledsAcesos = 3;
                }

                if (ledsLigados == 3)
                {
                    //_delay_ms(2000);
                    PORTB ^= (1 << PB3);
                    ledsLigados--;
                    ledsAcesos = 2;
                }

                if (ledsLigados == 2)
                {
                    //_delay_ms(2000);
                    PORTB ^= (1 << PB2);
                    ledsLigados--;
                    ledsAcesos = 1;
                }

                if (ledsLigados == 1)
                {
                    //_delay_ms(2000);
                    PORTB ^= (1 << PB1);
                    modo = 1;
                    ledsLigados--;
                    ledsAcesos = 0;
                }

                contador5 = 0;
            }
        }
        
        if (modo == 1) { //ligando leds
            //modo = 0;
            contador5 = 0;
            if(contador3 == 15){
                if(ledsAcesos == 0) {
                    //if(flag5s == 1){
                        //_delay_ms(1000);
                        PORTB |= (1 << PB1);
                        ledsAcesos++;
                        ledsLigados = 1;
                    //}
                }

                if(ledsAcesos == 1) {
                    //if(flag5s == 1) {
                        //_delay_ms(1000);
                        PORTB |= (1 << PB2);
                        ledsAcesos++;
                        ledsLigados = 2;
                    //}
                }

                if(ledsAcesos == 2) {
                    //if(flag5s == 1) {
                        //_delay_ms(1000);
                        PORTB |= (1 << PB3);
                        ledsAcesos++;
                        ledsLigados = 3;
                    //}
                }
                
                if(ledsAcesos == 3) {
                    //if(flag5s == 1) {
                        //_delay_ms(1000);
                        PORTB |= (1 << PB4);
                        ledsAcesos++;
                        ledsLigados = 4;
                    //}
                }

                if(ledsAcesos == 4) {
                    //if(flag5s == 1) {
                        //_delay_ms(1000);
                        PORTB |= (1 << PB5);
                        modo = 0;
                        ledsAcesos++;
                        ledsLigados = 5;
                    //}
                }

                contador3 = 0;
            }
        } 

        contador5++; 
        contador3++;
        _delay_ms(200);
    }
}