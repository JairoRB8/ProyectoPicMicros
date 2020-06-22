

#include <xc.h>
#include <pic18f4550.h>
#include "PWM.h"

// PWM Period = [(PR2)+1] *4 Tosc * TMR2 Prescaler
// PR2 = 124, con TMR2 Prescaler = 16, 1kHz
// PWM Duty Cycle  =    (CCPR1L:CCP1CON<5:4> ?TOSC ? (TMR2 Prescale Value) 
// CCPR1L = DC / (Tosc * Prescaler)
// 100% -> 500 
// 75%  -> 375
// 50%  -> 250
// 25%  -> 125
// 0%   -> 0 

void PWM_Init(){
    // Configurando el modo PWM 
    TRISCbits.RC2 = 1;                                         // Salida 
    PR2 = 124;
    
    P1M1 = 0;
    P1M0 = 0;
    CCP1M3 = 1;
    CCP1M2 = 1;
    CCP1M1 = 0;
    CCP1M0 = 0;
    
    CCPR1L = 0;
    DC1B1 = 0;
    
    // Configurando Timer 2
    TMR2ON = 0;
    TMR2IF = 0;
    T2CKPS1 = 1;
    TMR2ON = 1;
    
    while(!TMR2IF){}
    TRISCbits.RC2 = 0;
    ECCPASE = 0;
}

void PWM_Speed(char speed){
    switch(speed){
        case 0:
            CCPR1L = 0;
            DC1B0 = 0;
            DC1B1 = 0;
            break;
        case 1:
            CCPR1L = 125>>2;
            DC1B0 =  125&0b1;
            DC1B1 = (125&0b10)>>1;
            break;
        case 2:
            CCPR1L = 250>>2;
            DC1B0 = 250&0b11;
            DC1B1 = (250&0b10)>>1;
            break;
        case 3:
            CCPR1L = 375>>2;
            DC1B0 = 375&0b1;
            DC1B1 = (375&0b10)>>1;
            break;
        case 4:
            CCPR1L = 499>>2;
            DC1B0 = 500&0b1;
            DC1B1 = (500&0b10)>>1;
            break;
        default:
            CCPR1L = 0;
            DC1B0 = 0;
            DC1B1 = 0;
    }
    
}