/*
 * File:   Principal.c
 * Author: schwe
 * Interrupciones y uso de librerías
 * Created on 13 de julio de 2023, 10:53 AM
 */

//CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT                        //Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF                                   //Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF                                  //Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF                                  //RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF                                     //Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF                                    //Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF                                  //Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF                                   //Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF                                  //Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF                                    //Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V                               //Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF                                    //Flash Program Memory Self Write Enable bits (Write protection off)

//Librerías
#include <xc.h>
#include "PORTB_int.h"
#include "ADC_int.h"
#include "Display.h"

//Definición de variables
#define _XTAL_FREQ 4000000
int rebote1;
int rebote2;
int ADC;
int F1;
int F2;
int contador;

//Prototipos
void setup(void);
void main(void);

//Interrupcion
void __interrupt() isr(void) {
    if (PIR1bits.ADIF){
        ADC = read_ADC();
        F1 = numero(ADC/16);
        F2 = numero(ADC%16);
    }
    
    if (INTCONbits.RBIF){
//        rebote1 = antiRebote(0);
//        rebote2 = antiRebote(1);
        if (!PORTBbits.RB6){
            rebote1 = 1;
        }
        if (!PORTBbits.RB7){
            rebote2 = 1;
        }
    }
    if (INTCONbits.T0IF){
        if (PORTDbits.RD0 == 1){
            PORTDbits.RD0 = 0;
            PORTC = F2;
            PORTDbits.RD1 = 1;
        }
        else if (PORTDbits.RD1 == 1){
            PORTDbits.RD1 = 0;
            PORTC = F1;
            PORTDbits.RD0 = 1;
        }
        TMR0 = 193;
    }
    
    INTCONbits.T0IF = 0;
    PIR1bits.ADIF = 0;                                      //Limpia la bandera de interrupción
    INTCONbits.RBIF = 0;
    return;
}

//Setup General
void setup(void){
    //Oscilador
    OSCCON = 0B01100001;                                    //Oscilador a 4Mhz
    
    //Interrupciones
    INTCON = 0B11101000;                                    //Int globales, PIE1, TMR0, PORTB activadas
    PIE1 = 0B01000000;                                      //Int ADC activada
    
    //ADC
    setup_ADC(0);
    
    //TMR0
    OPTION_REG = 0B00000101;                                // 4ms
    TMR0 = 193;
    
    //PORTB
    TRISB = 0;
    setup_PORTB(6);
    setup_PORTB(7);
        
    //Entradas y salidas
    TRISC = 0;
    TRISD = 0;
    
    //Valores iniciales de variables y puertos
    PORTA = 0;
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    PORTDbits.RD0 = 1;
    PORTE = 0;
    
    rebote1 = 0;
    rebote2 = 0;
    ADC = 0;
    F1 = 0;
    F2 = 0;
    contador = 0;
    
    return;
}

//Loop
void main(void) {
    setup();
    while(1){
        if (ADCON0bits.GO == 0){
            __delay_ms(100);
            ADCON0bits.GO = 1;
        }
        
        if (PORTBbits.RB6 && rebote1 == 1){
            contador += 1;
            PORTB = contador;
            rebote1 = 0;
        }
        else if (PORTBbits.RB7 && rebote2 == 1){
            contador -= 1;
            PORTB = contador;
            rebote2 = 0;
        }
        
        if (ADC >= contador){
            PORTDbits.RD7 = 1;                
        }
        else{
            PORTDbits.RD7 = 0;
        }
    }
}