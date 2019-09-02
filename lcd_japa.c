
#include "control_fly.h"




void putch(char c) {

    lcdSend(1, c);

}


void lcdInit() {

    char i;

    DATA_DIR = 0x00; //  portb like output
    DATA = 0x00;

    RS_DIR = 0x00; // PIN RS
    EN_DIR = 0x00; //PIN E
    RS = 0x00; // RS
    EN = 0x00;

    __delay_ms(15);



    for (i = 0; i <= 2; i++) {

        lcdCmd(0x30); //send three times 0x30

        __delay_ms(5);
    }

    lcdCmd(0x02); //lcd will work with 4 bits
    __delay_us(40);
    lcdCmd(0x28); // 4 bits comunication , display 2 row and matrix 7x5
    __delay_us(40);
    lcdCmd(0x01); //clear screen
    __delay_ms(1);
    lcdCmd(0x0C); //turn on display without cursor
    __delay_us(40);
    lcdCmd(0x06); // cursor goes to the right after the new character
    __delay_us(40);




}

void lcdCmd(char cmd)
{

    PORTD = cmd & 0xf0;
   

    PORTEbits.RE1 = 0x01; // PORT E display
    PORTEbits.RE1 = 0x00; //

    __delay_ms(1);


    PORTD = (cmd << 4) & 0xf0;


    PORTEbits.RE1 = 0x01; // PORT E display
    PORTEbits.RE1 = 0x00; //





}

void lcdSend(char level, char data)
{

    PORTEbits.RE0 = level; //enable comand write
    __delay_us(100);
    PORTEbits.RE1 = 0x00; // enable write E lcd
    lcdCmd(data);





}


void lcdWrite(char *str) 
{



    unsigned char tamBuffer = 16; // control maximo size 16


    while (tamBuffer--)
    { // while tamBuffer > 0 

        lcdSend(1, *str); //  write character


        str++;


    }
}

void lcdPositionxy(char x, char y) 
{

    char address;

    if (x != 1)

        address = 0xc0; // put in second line

    else

        address = 0x80; // put in firt line

    address = (address + y) - 1;

    lcdSend(0, address);

}

