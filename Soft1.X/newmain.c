/*
 * File:   Initilization.c
 * Author: Mohammad Omar
 *
 * Created on April 15, 2023, 9:31 AM
 */


#define _XTAL_FREQ   4000000UL     // needed for the delays, set to 4 MH= your crystal frequency
// CONFIG1H
#pragma config OSC = XT         // Oscillator Selection bits (XT oscillator)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = SBORDIS  // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 3         // Brown Out Reset Voltage bits (Minimum setting)

// CONFIG2H
#pragma config WDT = ON         // Watchdog Timer Enable bit (WDT enabled)
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = PORTC   // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = ON      // PORTB A/D Enable bit (PORTB<4:0> pins are configured as analog input channels on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = ON         // Single-Supply ICSP Enable bit (Single-Supply ICSP enabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-003FFFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (004000-007FFFh) not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (008000-00BFFFh) not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (00C000-00FFFFh) not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-003FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (004000-007FFFh) not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (008000-00BFFFh) not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (00C000-00FFFFh) not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot Block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (004000-007FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (008000-00BFFFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (00C000-00FFFFh) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot Block (000000-0007FFh) not protected from table reads executed in other blocks)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.
#include <pic18.h>
#include "lcd_x8.h"
#include"my_timer.h"
#include "my_ser.h"
#include"initial.h"
#include"my_adc.h"
#include <stdio.h>

void main(void) {

    initPorts();
    PORTDbits.RD0 = 1;
    PORTDbits.RD1 = 1;
    lcd_init();
    setupSerial();
    init_adc_no_lib();
    initIIntterupts();
    reloadTimer();
    float AN[3]; // To store the voltages of AN0, AN1, AN2
    int raw_val;
    unsigned char channel;
    float Tempreture;
    char Buffer[30];
    short j = 0;

    while (1) {

        ClrWdt();
        char tmpbuffer[20];
        for (channel = 0; channel < 3; channel++) {
            // read the adc voltage
            Tempreture = read_adc_voltage((unsigned char) channel);
            AN[channel] = Tempreture; // store in array AN0--AN2

        }
        raw_val = read_adc_raw_no_lib(0);

        lcd_gotoxy(9, 1);
        sprintf(Buffer, " %4.2f", AN[2]*100); // read raw value for POT1 
        lcd_puts(Buffer);
        lcd_gotoxy(1, 2);
        char *Cooler;
        char *Heater;
        if (PORTCbits.RC2) {
            lcd_puts("C: ON ");
            Cooler = "ON";
        }
        if (!PORTCbits.RC2) {
            lcd_puts("C: OFF");
            Cooler = "OFF";
        }
        lcd_gotoxy(9, 2);
        if (PORTCbits.RC5) {
            lcd_puts("H: ON ");
            Heater = "ON";
        }
        if (!PORTCbits.RC5) {
            lcd_puts("H: OFF");
            Heater = "OFF";
        }


        if (mode != 1) {
            INTCONbits.TMR0IE = 1;
            INTCON3bits.INT2IE = 0;
            lcd_gotoxy(1, 3);
            lcd_puts("Normal");
            lcd_gotoxy(10, 3);
            lcd_puts(" ");


            lcd_gotoxy(1, 1);
            sprintf(Buffer, "%2d:%2d:%2d", hours, minutes, seconds);
            lcd_puts(Buffer);

        } else if (mode == 1) {
            INTCONbits.TMR0IE = 0;
            INTCON3bits.INT2IE = 1;
            lcd_gotoxy(1, 3);
            lcd_puts("Setup ");
            lcd_gotoxy(10, 3);
            sprintf(Buffer, "%1c", seconds_minutes_hours); // read raw value for POT1 
            lcd_puts(Buffer);

        }
        if (!PORTBbits.RB3 && mode == 1) {
            INTCONbits.TMR0IE = 0;


            delay_ms(100);
            if (!PORTBbits.RB3) // re-check the keypress
            {

                incrementSet(seconds_minutes_hours);

                lcd_gotoxy(1, 1);
                sprintf(Buffer, "%2d:%2d:%2d", hours, minutes, seconds);
                lcd_puts(Buffer);
            }
        }
        if (!PORTBbits.RB4 && mode == 1) {
                        INTCONbits.TMR0IE = 0;
// S1 is the pressed key
            delay_ms(100); // 100ms delay for avoiding spike detection
            if (!PORTBbits.RB4) // re-check the keypress
            {
                Decremnet(seconds_minutes_hours);
                lcd_gotoxy(1, 1);
                sprintf(Buffer, "%2d:%2d:%2d", hours, minutes, seconds);
                lcd_puts(Buffer);
            }
        }
        if (lineFourFlag == 1) {
            lcd_gotoxy(0, 4);
            sprintf(Buffer, "V1=%4.2f V2=%4.2f", AN[0], AN[1]);
            // read raw value for POT1 
            lcd_gotoxy(1, 4);
            lcd_puts(Buffer);
        } else if (lineFourFlag == 0) {
            lcd_gotoxy(0, 4);
            sprintf(Buffer, "Mohammad Osama  "); // read raw value for POT1 
            lcd_gotoxy(1, 4);
            lcd_puts(Buffer);
        }

        delay_ms(100);
        char command[32];
        if (readTime == 1) {
            send_string_no_lib((unsigned char *) "TIME");
            sprintf(command, " %2d:%2d:%2d HH \n", hours, minutes, seconds);
            send_string_no_lib(command);
            reloadIT();
        } else if (readCooler == 1) {
            send_string_no_lib((unsigned char *) "CoolerState: ");

            sprintf(command, "%3s \n", Cooler);
            send_string_no_lib(command);
            reloadIT();
        } else if (readHeater == 1) {
            send_string_no_lib((unsigned char *) "HeaterState: ");
            sprintf(command, "%3s \n", Heater);
            send_string_no_lib(command);
            reloadIT();

        } else if (readA0 == 1) {
            send_string_no_lib((unsigned char *) "A0 Value : ");
            sprintf(command, "%4.2f \n", AN[0]);
            send_string_no_lib(command);
            reloadIT();
        } else if (readA1 == 1) {
            send_string_no_lib((unsigned char *) "A1 Value : ");
            sprintf(command, "%4.2f \n", AN[1]);
            send_string_no_lib(command);
            reloadIT();
        } else if (readA2 == 1) {
            send_string_no_lib((unsigned char *) "A2 Value : ");
            sprintf(command, "%4.2f \n", AN[2]);
            send_string_no_lib(command);
            reloadIT();
        } else if (readD == 1) {
            send_string_no_lib((unsigned char *) "PortD : ");
            sprintf(command, "%8d \n", LATD);
            send_string_no_lib(command);
            reloadIT();
        } else if (readHeater == 2) {
            Heater = writeMode;
            PORTCbits.RC5 = Heater;
            reloadIT();

        } else if (readCooler == 2) {
            Cooler = writeMode;
            PORTCbits.RC2 = Cooler;
            reloadIT();

        } else {
            reloadIT();
        }




        // send_string_no_lib((unsigned char *) "\r\rReading AN0, AN1, AN2\r\r");
        //        for (int j = 0; j < 4; j++) {
        //            send_string_no_lib((unsigned char *) command);
        //        }

    }






}



