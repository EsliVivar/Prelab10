/* 
 * File:   Prelab10.c
 * Author: Esli Vivar 20286
 *Descripcion:Parte 1. Envíe un caracter desde el microcontrolador hacia la computadora y
mírela en la hiperterminal.
Parte 2. Reciba un carácter desde la hiperterminal y muéstrelo en el puerto B
 */

// Configuracion 1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

//Configuracion 2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

/* 
 *Librerías
 */
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <stdint.h>  // Para poder usar los int de 8 bits

/* 
 *Constantes
 */
#define _XTAL_FREQ 1000000

/* 
 *Variables
 */

/* 
 *Prototipo de Funciones
 */
void setup(void);

/* 
 *Interrupciones
 */
void __interrupt() isr (void){
    //Comprueba si hay algún valor recibido
    if(PIR1bits.RCIF){       
        //Se muestra la información en el PORTB
        //Se limpia la bandera
       PORTB = RCREG;           
    }
    //Se regresa
    return;
}


/* 
 *Ciclo Principal
 */
void main(void) {
    //Configuración Inicial
    setup();
    while(1){
        //Tiempo de retardo
        __delay_ms(500); 
        //Comprobar si existe o no existe la información para que no se sobreescriba
        if(PIR1bits.TXIF){
        TXREG = 0x50;   //CARGAMOS EL VALOR EN HEXA.   
       }       
    }
    //Se regresa
    return;
}

/* 
 *Configuración
 */
void setup(void){
    //Entradas y salidas digitales
    ANSEL = 0;
    ANSELH = 0;       
    
    //Se limpia el puerto B
    TRISB = 0;
    //Puerto B como salida
    PORTB = 0;            
    
    OSCCONbits.IRCF = 0b100;    // 1MHz
    OSCCONbits.SCS = 1;         // Se habilita el oscilador interno
    
    // Configuraciones de comunicacion serial
    TXSTAbits.SYNC = 0;         // Comunicación ascincrona (full-duplex)
    TXSTAbits.BRGH = 1;         // Baud rate de alta velocidad 
    BAUDCTLbits.BRG16 = 1;      // 16-bits para generar el baud rate
    
    SPBRG = 25;
    SPBRGH = 0;                 // Baud rate ~9600, error -> 0.16%
    
    RCSTAbits.SPEN = 1;         // Habilitamos comunicación
    TXSTAbits.TX9 = 0;          // Utilizamos solo 8 bits
    TXSTAbits.TXEN = 1;         // Habilitamos transmisor
    RCSTAbits.CREN = 1;         // Habilitamos receptor
    
    // Configuraciones de interrupciones
    INTCONbits.GIE = 1;         // Habilitamos interrupciones globales
    INTCONbits.PEIE = 1;        // Habilitamos interrupciones de perifericos
    PIE1bits.RCIE = 1;          // Habilitamos Interrupciones de recepción
}