/*
 * File:   carro.c
 * Author: jairo
 *
 * Created on June 21, 2020, 7:54 PM
 */


#include <xc.h>
#include "LibreriaMicros.h"
#include <pic18f4550.h>
#include "PWM.h"

volatile int contador = 0;
char comandoRecibido;
int numeroComando;
__bit ledEncendida = 0;
volatile __bit emergencia = 0;

void main(void) {
    ADCON1 = 0b1111;
    USART_Init(9600);
    PWM_Init();
    LATC=0;
    LATA=0;
    TRISAbits.RA6 = 0;
    TRISBbits.RB1 = 0;
    TRISBbits.RB2 = 0;
    TRISBbits.RB3 = 0;
    TRISBbits.RB4 = 0;
    TRISAbits.RA1 = 0;
    
    // Configura entrada para parada de emergencia
    INTCON2bits.RBPU = 1;                   // Desactiva pull-ups internas
    TRISBbits.RB0 = 1;                      // Coloca INT0 como entrada
    INT0IF = 0;                             // Resetea flag
    INT0IE = 1;                             // Habilita el interrupt de INT0
    
    
    //timer para LED's
    LATBbits.LB4 = 1;
    timer_init();
    
    
    RCONbits.IPEN = 1;                      //Habilitad modo prioridad
    INTCONbits.GIEH = 1;                    //habilita interrupts de alta prioridad
    INTCONbits.GIEL = 1;                    //habilita interrupts de baja prioridad
    
    while(1){
        if(emergencia == 0){
            comandoRecibido = USART_Rx();
            numeroComando = comandoRecibido - '0';
            switch (numeroComando){
                case 0: //adelante
                    LATAbits.LA6 = 1;
                    LATBbits.LB1 = 0;
                    LATBbits.LB2 = 1;
                    LATBbits.LB3 = 0;
                    break;
                case 1: //atrás
                    LATAbits.LA6 = 0;
                    LATBbits.LB1 = 1;
                    LATBbits.LB2 = 0;
                    LATBbits.LB3 = 1;
                    break;
                case 2: //izquierda
                    LATAbits.LA6= 1;
                    LATBbits.LB1 = 0;
                    LATBbits.LB2 = 0;
                    LATBbits.LB3 = 1;
                    break;
                case 3: //derecha
                    LATAbits.LA6 = 0;
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
                    PWM_Speed(0);
                    break;
                case 7: //velocidad 2
                    PWM_Speed(1);
                    break;
                case 8: //velocidad 3
                    PWM_Speed(2);
                    break;
                case 9: //velocidad 4
                    PWM_Speed(3);
                    break;
                case 10://velocidad 5
                    PWM_Speed(4);
                    break;    
                default:
                    PWM_Speed(0);
                    //apagar motores
                    break;
            }
        }   
    }
}

void __interrupt(low_priority) ISR(){
    if (PIR1bits.TMR1IF == 1){
        contador++;
        if (contador == 19){
            LATBbits.LB4 ^= 1;
            contador = 0;
        }
        PIR1bits.TMR1IF = 0;
    }
}

void __interrupt(high_priority) ISR_Emergencia(){
    if(INT0IF){
        if(emergencia == 0){
            PWM_Speed(0);
            TRISBbits.RB4 = 1;
            T1CONbits.TMR1ON = 0;
            emergencia = 1;
        }
        else{
            emergencia = 0;
        }
        INT0IF = 0;
    }   
}
