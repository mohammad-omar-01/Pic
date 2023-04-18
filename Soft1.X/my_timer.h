#ifndef XC_MY_TIMER_H
#define	XC_MY_TIMER_H     

void reloadTimer(void);
void Timer0_isr(void);
void EXT_Int1_isr(void);
void EXT_Int0_isr(void);
void RX_isr(void);
void EXT_Int2_isr(void);
void Decremnet(unsigned char setting);
void VaildateDecre(void);
void increment(unsigned char setting);
void delay_ms(unsigned int n);
void reloadIT(void);
    

unsigned short seconds = 50;
unsigned short minutes = 59;
unsigned short hours = 23;
unsigned short cnt2=-1;
unsigned flag=1;
short lineFourFlag=0;
short mode = -1;

unsigned char seconds_minutes_hours = "s";
unsigned char* key = "";
int commandStart=0;
int readMode=0;
int readD=0;
int readCooler=0;
int readHeater=0;
int readTime=0;
int readA0=0;
int readA1=0;
int readA2=0;
int writeMode=0;
int writeD0=0;
int writeD1=0;
int writeD2=0;
int writeD3=0;
int writeD4=0;
int writeD5=0;
int writeD6=0;
int writeD7=0;
int readAnalog=0;

#define STARTVALUE  3036

#endif	/*  XC_MY_SER_H */

