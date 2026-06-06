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

//taktowanie dla delay_32
#define FCY 4000000UL 

#include "lcd.h"
#include <xc.h>
#include <libpic30.h>
#include <stdbool.h>
#include <stdint.h>
#include "buttons.h"
#include "adc.h"

typedef enum {
    STAN_NORMALNY,
    STAN_MRUGANIE,
    STAN_CIAGLY
} StanAlarmu;

int main(void) {
    AD1PCFG = 0xFFFF; 
    TRISA = 0x0000;   
    LATA = 0x0000;   
    
    ADC_SetConfiguration(ADC_CONFIGURATION_DEFAULT);
    ADC_ChannelEnable(ADC_CHANNEL_POTENTIOMETER);
    
    StanAlarmu obecnyStan = STAN_NORMALNY;
    unsigned int wartoscPotencjometru = 0;
    int licznikCzasu = 0; 
    
    LCD_Initialize();
    LCD_ClearScreen();
    

    //glowna petla programu
    while (1) {
        wartoscPotencjometru = ADC_Read10bit(ADC_CHANNEL_POTENTIOMETER);
        if (wartoscPotencjometru == 0xFFFF) {
            continue; 
        }
        
        
        //jesli jest normalnie i przekroczymy 50% odpala sie alarm
        if (obecnyStan == STAN_NORMALNY) {
            if (wartoscPotencjometru >= 512) {
                obecnyStan = STAN_MRUGANIE;
                licznikCzasu = 0;
            }
        } 
        //jesli alarm trwa
        else if (obecnyStan == STAN_MRUGANIE || obecnyStan == STAN_CIAGLY) {
            
            //proba wylaczenia dziala tylko gdy suwak<50%
            if (wartoscPotencjometru < 512 && BUTTON_IsPressed(BUTTON_S3) == true) {
                obecnyStan = STAN_NORMALNY;
                LATA = 0x0000; // Zga? diody
            }
            
            //jesli nie wylaczono nadal mruga
            if (obecnyStan == STAN_MRUGANIE) {
                if ((licznikCzasu / 5) % 2 == 0) {
                    LATA = 0x0001; 
                } else {
                    LATA = 0x0000; 
                }
                
                licznikCzasu++;
                
                //po 5sekundach ledy przechodza w tryb ciagly
                if (licznikCzasu >= 50) {
                    obecnyStan = STAN_CIAGLY;
                }
            } 
            else if (obecnyStan == STAN_CIAGLY) {
                LATA = 0x00FF; //wszystkie diody sie swieca
            }
        }
        
        __delay32(400000); 
    }
    
    return 0;
}