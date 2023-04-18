
#include <stdio.h>

#include "my_timer.h"
#include "my_ser.h"
#include "lcd_x8.h"
short secminhor = -1;
short cnt = 0;

void reloadTimer(void) {

    TMR0H = (unsigned char) ((STARTVALUE >> 8) & 0x00FF);
    TMR0L = (unsigned char) (STARTVALUE & 0x00FF);
}

void checkTimeValidation(void) {
    if (seconds >= 60) {
        minutes++;
        seconds = 50;
    }
    if (minutes >= 60) {
        hours++;
        minutes = 0;
    }
    if (hours >= 23) {
        hours = 0;
    }


}

void increment(unsigned char setting) {
    char tmp2Buffer[30];
    
 
    switch (setting) {
        case 's':
        {
            seconds++;
            checkTimeValidation();
            break;
        }
        case 'm':
        {
            minutes++;
            checkTimeValidation();
            break;
        }
        case 'h':
        {
            // hours = 4;
            hours++;
            checkTimeValidation();
            break;
        }

    }
   

}

void Decremnet(unsigned char setting) {
    char tmpBuffer[30];
    INTCONbits.TMR0IE = 0;

    switch (setting) {
        case 's':
        {
            if(seconds>=1){
                seconds--;
            }
            
            lcd_gotoxy(1, 1);
            sprintf(tmpBuffer, "%2d:%2d:%2d", hours, minutes, seconds);
            lcd_puts(tmpBuffer);
            break;
        }
        case 'm':
        {
            if(minutes>=1){
                minutes--;
            }
            lcd_gotoxy(1, 1);
            sprintf(tmpBuffer, "%2d:%2d:%2d", hours, minutes, seconds);
            lcd_puts(tmpBuffer);
            break;
        }
        case 'h':
        {
             if(hours>=1){
                minutes--;
            }
            lcd_gotoxy(1, 1);
            sprintf(tmpBuffer, "%2d:%2d:%2d", hours, minutes, seconds);
            lcd_puts(tmpBuffer);
            break;
        }

    }

}

void VaildateDecre(void) {
    if (seconds < 1) {
        seconds = 0;
    }
    if (minutes < 1) {
        minutes = 0;
    }
    if (hours < 1) {
        hours = 0;
    }
}

void Timer0_isr(void) {
    INTCONbits.TMR0IF = 0;
    increment('s');
    reloadTimer();
}

void RX_isr(void) {
    CLRWDT();
    ReceivedChar = RCREG;
    ++cnt2;
    if (ReceivedChar == '<') {
        commandStart = 1;
    }
    if (commandStart != 1 && cnt2 > 1) {
        reloadIT();
        PORTDbits.RD3 = !PORTDbits.RD3;
    }
    if (cnt2 == 1) {

        if (commandStart == 1 && ReceivedChar == 'R') {

            readMode = 1;


        } else if (commandStart && ReceivedChar == 'W') {
            readMode = 2;
        }

    }
    if (cnt2 == 2 && readMode == 1) {
        if (ReceivedChar == 't') {
            readTime = 1;
        } else if (ReceivedChar == 'A') {
            readAnalog = 1;
        } else if (ReceivedChar == 'H') {
            readHeater = 1;
        } else if (ReceivedChar == 'C') {
            readCooler = 1;
        } else if (ReceivedChar == 'D') {
            readD = 1;
        } else if (ReceivedChar == 't') {
            readTime = 1;
        }

    } else if (readMode == 2 && cnt2 == 2) {
        if (ReceivedChar == 'H') {
            readHeater = 2;
        } else if (ReceivedChar == 'C') {
            readCooler = 2;
        } else if (ReceivedChar == 'D') {
            readD = 2;
        } else if (ReceivedChar == 't') {
            readTime = 2;
        } else if (ReceivedChar == '0') {
            writeD0 = 1;
        } else if (ReceivedChar == '1') {
            writeD1 = 1;
        } else if (ReceivedChar == '2') {
            writeD2 = 1;
        } else if (ReceivedChar == '3') {
            writeD3 = 1;
        } else if (ReceivedChar == '4') {
            writeD4 = 1;
        } else if (ReceivedChar == '5') {
            writeD5 = 1;
        } else if (ReceivedChar == '6') {
            writeD6 = 1;
        } else if (ReceivedChar == '7') {
            writeD7 = 1;
        }
    }
    if (cnt2 == 3) {
        if (readAnalog == 1 && readMode == 1) {

            if (ReceivedChar == '0') {
                readA0 = 1;
                PORTDbits.RD3 = !PORTDbits.RD3;
            } else if (ReceivedChar == '1')readA1 = 1;
            else if (ReceivedChar == '2')readA2 = 1;
        } else if (readMode == 2) {
            if (ReceivedChar != 'V') {
                ReceivedChar = 'V';
                cnt2 = 4;
            }
        }


    }
    if (cnt2 == 4 && readMode == 2 && ReceivedChar != '>') {
        if (ReceivedChar == '0') {
            writeMode = 0;

        }
        if (ReceivedChar == '1') {
            writeMode = 1;
        }
    }

}

void reloadIT(void) {
    commandStart = 0;
    cnt2 = -1;
    readMode = 0;
    readD = 0;
    readHeater = 0;
    readCooler = 0;
    readTime = 0;
    readA0 = 0;
    readA1 = 0;
    readA2 = 0;
    writeMode = 0;
    writeD0 = 0;
    writeD1 = 0;
    writeD2 = 0;
    writeD3 = 0;
    writeD4 = 0;
    writeD5 = 0;
    writeD6 = 0;
    writeD7 = 0;
    readAnalog = 0;
}

void EXT_Int1_isr(void) {
    cnt++;
    delay_ms(500);
    char tmpbuffer[2];
    if (cnt % 2) {
        mode = 0;
        INTCON3bits.INT1F = 0;

        return;



    } else {
        mode = 1;
        INTCON3bits.INT1F = 0;

        return;
    }


}

void EXT_Int2_isr(void) {
    secminhor++;
    delay_ms(500);

    if (secminhor == 0) {
        seconds_minutes_hours = 's';
    } else if (secminhor == 1) {
        seconds_minutes_hours = 'm';
    } else if (secminhor == 2) {
        seconds_minutes_hours = 'h';
        secminhor = -1;
    }
    INTCON3bits.INT2F = 0;

}


void EXT_Int0_isr(void) {
    lineFourFlag=!lineFourFlag;
    INTCONbits.INT0F = 0;

}

void __interrupt(high_priority) highIsr(void)
//void interrupt high_priority highIsr(void)
{
    if (INTCONbits.TMR0IF) Timer0_isr();
    if (PIR1bits.RCIF) RX_isr();
    if (INTCON3bits.INT1F) EXT_Int1_isr();
    if (INTCON3bits.INT2F) EXT_Int2_isr();
    if(INTCONbits.INT0F) EXT_Int0_isr();

    //    else if(PIR1bits.TXIF &&  PIE1bits.TXIE ) TX_isr();

}

