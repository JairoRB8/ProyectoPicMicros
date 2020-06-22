/*
 * File:   carro.c
 * Author: jairo
 *
 * Created on June 21, 2020, 7:54 PM
 */


#include <xc.h>
#include "USARTlib.h"
#include <pic18f4550.h>
volatile int contador = 0;
char comandoRecibido;
int numeroComando;
__bit ledEncendida = 0;
void main(void) {
    USART_Init(9600);
    LATC=0;
    LATA=0;
    TRISBbits.RB0 = 0;
    TRISBbits.RB1 = 0;
    TRISBbits.RB2 = 0;
    TRISBbits.RB3 = 0;
    TRISBbits.RB4 = 0;
    TRISAbits.RA1 = 0;
    
    //timer para LED's
    LATBbits.LB4 = 1;
    timer_init();
    while(1){
        comandoRecibido = USART_Rx();
        numeroComando = comandoRecibido - '0';
        PORTAbits.RA1 = 1;
        switch (numeroComando){
            case 0: //adelante
                LATBbits.LB0 = 1;
                LATBbits.LB1 = 0;
                LATBbits.LB2 = 1;
                LATBbits.LB3 = 0;
                break;
            case 1: //atrás
                LATBbits.LB0 = 0;
                LATBbits.LB1 = 1;
                LATBbits.LB2 = 0;
                LATBbits.LB3 = 1;
                break;
            case 2: //izquierda
                LATBbits.LB0 = 1;
                LATBbits.LB1 = 0;
                LATBbits.LB2 = 0;
                LATBbits.LB3 = 1;
                break;
            case 3: //derecha
                LATBbits.LB0 = 0;
                LATBbits.LB1 = 1;
                LATBbits.LB2 = 1;
                LATBbits.LB3 = 0;
                break;
            case 4: //toggle luces
                TRISBbits.RB4 ^= 1; //cambia entre input y output para hacer toggle
                break;
            case 5: //toggle modo luces
                T1CONbits.TMR1ON ^= 1;
                if (PORTBbits.RB4 == 0){
                    LATBbits.LB4 = 1;
                }
                break;
            case 6: //velocidad 1
                break;
            case 7: //velocidad 2
                break;
            case 8: //velocidad 3
                break;
            case 9: //velocidad 4
                break;
            case 10://velocidad 5
                break;    
            default:
                //apagar motores
                break;
        }   
    }
}

void __interrupt() ISR(){
    if (PIR1bits.TMR1IF == 1){
        contador++;
        if (contador == 19){
            LATBbits.LB4 ^= 1;
            contador = 0;
        }
        PIR1bits.TMR1IF = 0;
    }
}