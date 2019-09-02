
/*
 * File:   controlmotormain.c
 * Author: Roner Augusto Benites
 * 
 * Created on 29 de Junho de 2019, 23:11
 */


#define _XTAL_FREQ 16000000
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "control_fly.h"



void interrupt isr();
void config_interrution();
void config_pins();


char buf[16];
unsigned int change_flag = 0x01;
float timer_total = 0x00;
unsigned int counter = 0x00;
unsigned char timerH, timerL;


void main(void) 
{
 
    
    config_interrution();
    lcdInit();
    config_pins();
    
    
  
   
    
    
    
    while (1) 
    {
        
           
             
            timer_total = (timerH << 8 ) + timerL; // capture the time 
             
             lcdPositionxy(1, 1);
             printf("control distance");
             lcdPositionxy(2, 1);            
             sprintf(buf,"dist = %0.2f cm ",(timer_total/58));
             lcdWrite(buf);
             
           
            
              __delay_ms(1);
               
        
        
    }
   
}






void config_pins()
{
    
      /* Replace with your application code */
    CMCON = 0x07; //desable comparator
    ADCON1 = 0x0f; // desable channel A/D
   
    
    /*config pins like input or output */
    
    TRISCbits.TRISC2 = 0x01; //as input 
    TRISCbits.TRISC1 = 0x00;  // RC1 as input 
    TRISCbits.TRISC7= 0x00;  // as output
    TRISCbits.TRISC4 = 0x00;
    
    
    
    
    /*initial conditions */
    
    PORTCbits.RC2 = 0x00;   //start in zero volt
    PORTCbits.RC1 = 0x00;   // start in zero volt
    PORTCbits.RC7 = 0x00;   // start in zero volt
    PORTCbits.RC4 = 0x00;
    
    
    
    
}








void config_interrution()
{   
     /* Replace with your application code */
    CMCON = 0x07; //desable comparator
    ADCON1 = 0x0f; // desable channel A/D
  
    
    INTCONbits.GIE = 0x01; // enable glogal interrupt
    INTCONbits.PEIE = 0x01; // enable pheripherial interrupt
     ei(); //interrupts in general
    
   


    /*Configuration     PWM we wanna a 20us of period
     period=(PR2 + 1) * machinecycle* prescale of timer02
     period = 4 * 1us * 16   
     period ~ 64us microsegundos    
     */ 

     //PR2 = 0x04;        //load 4 to register
     //T2CON = 0x06;     // 00000111 turn on Timer02 and pre escale 1:16
     //CCP2CON = 0x0C;   // config to pwm mode 
     //CCPR2L = 0x02;    // pwm 50% PWM(IN %)= [(CCPR1L * T osc * tmr2 prescale)/P ]* 100%   P is period  pwm 
    

    
    
    /*interruption ccp1*/
    
     T1CONbits.TMR1ON = 0x01;     // keeep the timer 01 in zero to guarante
     T1CONbits.T1CKPS1 = 0X01;              //timer 01 turn off and prescale 1:4 increment each 1 us 16bits
     T1CONbits.T1CKPS0 = 0x00;
     CCP1CON = 0x05;              // enable interruption to rising edge
     PIE1bits.CCP1IE = 0x01;     //enable interruption ccp1 
     PIR1bits.CCP1IF = 0x00;     // clear the flag interruption
     TMR1H = 0x00;
     TMR1L = 0x00;
     
     
    
     
     
     
     /*****interruption timer zero******/
     /*TMR0 = 2E16 - (desiretime /(machinecile * prescale)) */ 
     /* TMR0 = 65536 - (10us / (250ns * 4)) */
     /* TMR0 = 65526 or  0xfff0*/
     /*overflow in 10 us*/
     
    T0CON = 0xc1;               //0b11000001;        //resulution 16bits and prescale 1:4
    INTCONbits.TMR0IE = 0x01;  //enable timer zero 
    INTCONbits.TMR0IF = 0x00;
    TMR0L = 0xf8;
    TMR0H = 0xff;             //value to generate 10us
     
     
     
     
    
    
}


void interrupt isr()
{
    
        
                /* we're gonna capture timerise and timefall  */
               
    if(PIR1bits.CCP1IF)
    {
                PIR1bits.CCP1IF = 0x00; //reload the flag interruption 
       
                timerH = CCPR1H;
                timerL = CCPR1L;
                TMR1H = 0x00;
                TMR1L = 0x00;
   
    }
    
    if(INTCONbits.TMR0IF)
    {
       
        PORTCbits.RC4 = ~PORTCbits.RC4;
        INTCONbits.TMR0IF = 0x00;
         TMR0L = 0xf8;
         TMR0H = 0xff;             //value to generate 10us
     
        
        
        
    }

    
        
        

               
        
  
    
}

































