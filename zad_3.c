// PIC24FJ128GA010 Configuration Bit Settings
// CONFIG2
#pragma config POSCMOD = XT 
#pragma config OSCIOFNC = ON 
#pragma config FCKSM = CSDCMD 
#pragma config FNOSC = PRI 
#pragma config IESO = ON 
// CONFIG1
#pragma config WDTPS = PS32768 
#pragma config FWPSA = PR128 
#pragma config WINDIS = ON 
#pragma config FWDTEN = OFF 
#pragma config ICS = PGx2 
#pragma config GWRP = OFF 
#pragma config GCP = OFF 
#pragma config JTAGEN = OFF

#include <xc.h>
#include <libpic30.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h> 
#include "buttons.h"
#include "lcd.h"

int main(void) {
    AD1PCFG = 0xFFFF; 
    TRISA = 0x0000;   
    LATA = 0x0000;    
    
    LCD_Initialize();
    LCD_ClearScreen();
    
    //tekst do animacji
    char scrollMsg[] = "        Nie wychodz po zmroku... Ucieczka jest niemozliwa... Obejrzyj STAMTAD NA HBO-MAX!!!                ";
    //obliczanie dlugosci komunikatu
    int msgLen = strlen(scrollMsg);
    
    while (1) {
        
        for (int i = 0; i < 4; i++) {
            LCD_ClearScreen();
            LCD_PutString("  OSTRZEZENIE!  ", 16);
            LATA = 0xFFFF;       
            __delay32(2000000); 
            
            LCD_ClearScreen();
            LATA = 0x0000;
            __delay32(1000000); 
        }
        
        LCD_ClearScreen();
        LCD_PutString(" Zostan w domu. ", 16);
        LATA = 0x0081; 
        __delay32(10000000);
        
        for (int i = 0; i <= msgLen - 16; i++) {
            LCD_ClearScreen();
            
            char buffer[17];
            strncpy(buffer, &scrollMsg[i], 16);
            buffer[16] = '\0'; 
            
            LCD_PutString(buffer, 16);
            
            //migocz?ce diody
            if (i % 2 == 0) {
                LATA = 0x00AA; 
            } else {
                LATA = 0x0055; 
            }
            
            __delay32(1000000); 
        }
        
        LATA = 0x0000;
        __delay32(10000000); //pauza przed powtorka reklamy
    }
    
    return 0;
}