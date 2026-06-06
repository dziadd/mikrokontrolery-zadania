// PIC24FJ128GA010 Configuration Bit Settings
// CONFIG1
#pragma config POSCMOD = XT 
#pragma config OSCIOFNC = ON 
#pragma config FCKSM = CSDCMD 
#pragma config FNOSC = PRI 
#pragma config IESO = ON 
#pragma config WDTPS = PS32768 
#pragma config FWPSA = PR128 
#pragma config WINDIS = ON 
#pragma config FWDTEN = OFF 
#pragma config ICS = PGx2 
#pragma config GWRP = OFF 
#pragma config GCP = OFF 
#pragma config JTAGEN = OFF 
// CONFIG2
#include <xc.h>
#include <libpic30.h>
#include <stdbool.h>    
#include <stdint.h>      
#include "buttons.h"
#include "lcd.h" 

// 8-bitowy licznik binarny w gór?
int prog1(void) {
    unsigned char count = 0; 
    int i;
    
    LCD_ClearScreen();
    LCD_PutString("P1: BIN W GORE", 14);
    
    //czekamy na puszczenie przycisków
    while(BUTTON_IsPressed(BUTTON_S6) || BUTTON_IsPressed(BUTTON_S3)) { __delay32(10000); }

    while(1) {
        LATA = count; 
        count++;      
        

        for(i = 0; i < 200; i++) {
            if (BUTTON_IsPressed(BUTTON_S6)) return 2;
            if (BUTTON_IsPressed(BUTTON_S3)) return 6;
            __delay32(10000); 
        }
    }
}

// 8-bitowy licznik binarny w dó?
int prog2(void) {
    unsigned char count = 255; 
    int i;
    
    LCD_ClearScreen();
    LCD_PutString("P2: BIN W DOL", 13);
    
    while(BUTTON_IsPressed(BUTTON_S6) || BUTTON_IsPressed(BUTTON_S3)) { __delay32(10000); }

    while(1) {
        LATA = count;
        count--; // Dekrementacja
        
        for(i = 0; i < 200; i++) {
            if (BUTTON_IsPressed(BUTTON_S6)) return 3;
            if (BUTTON_IsPressed(BUTTON_S3)) return 1;
            __delay32(10000);
        }
    }
}

// 8-bitowy licznik kod Graya w gór?
int prog3(void) {
    unsigned char count = 0;
    int i;
    
    LCD_ClearScreen();
    LCD_PutString("P3: GRAY W GORE", 15);
    
    while(BUTTON_IsPressed(BUTTON_S6) || BUTTON_IsPressed(BUTTON_S3)) { __delay32(10000); }

    while(1) {
        // Konwersja kodu binarnego na kod Graya
        unsigned char grayValue = count ^ (count >> 1);
        LATA = grayValue;
        
        count++; 
        
        for(i = 0; i < 200; i++) {
            if (BUTTON_IsPressed(BUTTON_S6)) return 4;
            if (BUTTON_IsPressed(BUTTON_S3)) return 2;
            __delay32(10000);
        }
    }
}

// 8-bitowy licznik kod Graya w dó?
int prog4(void) {
    unsigned char count = 255;
    int i;
    
    LCD_ClearScreen();
    LCD_PutString("P4: GRAY W DOL", 14);
    
    while(BUTTON_IsPressed(BUTTON_S6) || BUTTON_IsPressed(BUTTON_S3)) { __delay32(10000); }

    while(1) {
        unsigned char grayValue = count ^ (count >> 1);
        LATA = ~grayValue;
        
        count--; 
        
        for(i = 0; i < 200; i++) {
            if (BUTTON_IsPressed(BUTTON_S6)) return 5;
            if (BUTTON_IsPressed(BUTTON_S3)) return 3;
            __delay32(10000);
        }
    }
}

// 2x4 bitowy licznik w kodzie BCD w gór? 
int prog5(void) {
    int8_t tens = 0;  
    int8_t units = 0; 
    int i;
    
    LCD_ClearScreen();
    LCD_PutString("P5: BCD W GORE", 14);
    
    while(BUTTON_IsPressed(BUTTON_S6) || BUTTON_IsPressed(BUTTON_S3)) { __delay32(10000); }

    while(1) {
        LATA = ((tens << 4) | units);

        units++;
        if (units > 9) {
            units = 0;
            tens++;
            if (tens > 9) {
                tens = 0; // wyzerowanie po doj?ciu do 99
            }
        }
        
        for(i = 0; i < 200; i++) {
            if (BUTTON_IsPressed(BUTTON_S6)) return 6;
            if (BUTTON_IsPressed(BUTTON_S3)) return 4;
            __delay32(10000);
        }
    }
}

// 2x4 bitowy licznik w kodzie BCD w dó? 
int prog6(void) {
    int8_t tens = 9; 
    int8_t units = 9;
    int i;
    
    LCD_ClearScreen();
    LCD_PutString("P6: BCD W DOL", 13);
    
    while(BUTTON_IsPressed(BUTTON_S6) || BUTTON_IsPressed(BUTTON_S3)) { __delay32(10000); }

    while(1) {
        LATA = ((tens << 4) | units);

        units--;
        if (units < 0) {
            units = 9;
            tens--;
            if (tens < 0) {
                tens = 9;
            }
        }
        
        for(i = 0; i < 200; i++) {
            if (BUTTON_IsPressed(BUTTON_S6)) return 1;
            if (BUTTON_IsPressed(BUTTON_S3)) return 5;
            __delay32(10000);
        }
    }
}

int main(void){
    AD1PCFG = 0xFFFF; 
    TRISA = 0x0000;   
    
    LCD_Initialize();

    // start od programu nr 1
    int active_program = 1; 

    // glowna p?tla steruj?ca prze??czaniem programów
    while(1) {
        switch(active_program) {
            case 1: active_program = prog1(); break;
            case 2: active_program = prog2(); break;
            case 3: active_program = prog3(); break;
            case 4: active_program = prog4(); break;
            case 5: active_program = prog5(); break;
            case 6: active_program = prog6(); break;
            default: active_program = 1; break; 
        }
    }
    
    return 0;
}