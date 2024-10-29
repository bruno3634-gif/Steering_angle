/*******************************************************************************
  Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This file contains the "main" function for a project.

  Description:
    This file contains the "main" function for a project.  The
    "main" function calls the "SYS_Initialize" function to initialize the state
    machines of all modules in the system
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>
#include <stdio.h>                     // Defines EXIT_FAILURE
#include "definitions.h"
#include "interrupts.h"                // SYS function prototypes


// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

uint8_t message[64];

volatile uint16_t adc_result;
volatile float voltage;
uint32_t voltage_i; 
uint32_t voltage_f;
volatile float voltage_20,voltage_40;
volatile uint16_t adc_0_value;
unsigned long currentMillis = 0;



unsigned int millis(void);
/*

void calibrar(GPIO_PIN pin,uintptr_t context)
{
    
    GPIO_PinIntDisable(GPIO_PIN_RB12);
    LED_Set();
    LED2_Clear();
    //Virar Volante todo para a esquerda
    
    while(calibrate_btn_Get() == 0)
    {
        CORETIMER_DelayMs(50);
    }
    ADCHS_ChannelConversionStart(ADCHS_CH3);
    while(ADCHS_ChannelResultIsReady(ADCHS_CH3) == 0)
    {
        
    }
    voltage_20 = ADCHS_ChannelResultGet(ADCHS_CH3)* 3.3/4095;
    EEPROM_WordWrite(0,(uint32_t)voltage_20*100);
    while(EEPROM_IsBusy())
    {
        
    }
    LED_Clear();
    LED2_Set();
    CORETIMER_DelayMs(50);
    while(calibrate_btn_Get() == 0)
    {
       CORETIMER_DelayMs(50); 
    }
    //Virar o volante todo para a direita
    
     ADCHS_ChannelConversionStart(ADCHS_CH3);
    while(ADCHS_ChannelResultIsReady(ADCHS_CH3) == 0)
    {
        
    }
    voltage_40 = ADCHS_ChannelResultGet(ADCHS_CH3)* 3.3/4095;
    EEPROM_WordWrite(32,(uint32_t)voltage_20*100);
    while(EEPROM_IsBusy())
    {
        
    }
     GPIO_PinIntEnable(GPIO_PIN_RB12,GPIO_INTERRUPT_ON_FALLING_EDGE);
    
}*/


typedef enum {
    EEPROM_OPERATION_WORD_WRITE_READ_CMP = 0,
    EEPROM_OPERATION_PAGE_ERASE_READ_CMP,
    EEPROM_OPERATION_BULK_ERASE_READ_CMP,
    EEPROM_OPERATION_SUCCESS,
    EEPROM_OPERATION_ERROR,
    EEPROM_OPERATION_IDLE
} EEPROM_OPERATION_STATE;





volatile int valor_left,valor_right;
volatile int address = 0;
int main ( void )
{   
    currentMillis = millis();

    /* Initialize all modules */
    SYS_Initialize ( NULL );
    ADCHS_ModulesEnable(ADCHS_MODULE3_MASK);
   // EEPROM_Initialize();
   /* GPIO_PinInterruptCallbackRegister(GPIO_PIN_RB12,&calibrar,(uintptr_t)NULL);
    GPIO_PinIntEnable(GPIO_PIN_RB12,GPIO_INTERRUPT_ON_FALLING_EDGE);
   
    EEPROM_Initialize(); 
    EVIC_INT_Enable();*/
    // angulo 40º

//  1.65 --> 0
//  3.3 -->20

    for(int i = 0;i<=64;i++)
    {
        message[i] = 0;
    }
  /*  
    EEPROM_WordRead( address, valor_left);
    while ( EEPROM_IsBusy() == true )
    {
        address = address + 4;
    }
    EEPROM_WordWrite( address, valor_left);
    while ( EEPROM_IsBusy() == true )
    {
        address = address + 4;
    }
    
    address = 0;
   */
    //LED_Set();
    ADCHS_ChannelConversionStart(ADCHS_CH3);
    while ( true )
    {

        float media_ang = 0;
        
        

        if(ADCHS_ChannelResultIsReady(ADCHS_CH3) && millis() - currentMillis >= 300)
        {
            currentMillis = millis();

            
            adc_result = ADCHS_ChannelResultGet(ADCHS_CH3);
            voltage = adc_result * 3.3/4095;

            
            
            for(int i = 0; i<=3;i++)
            {
                volatile float angulo = voltage*40/3.3;
                //angulo = angulo - 20;
                media_ang = media_ang + angulo;
            }
            

            
            media_ang = media_ang/4;

            
            volatile int ang = media_ang * 100;
            
            for(int i = 0;i<=64;i++)
            {
                message[i] = 0;
            }

            message[0] = (ang >> 8) & 0xFF; // Byte mais significativo
            message[1] = ang & 0xFF;
            
            
            
  
   
            if(CAN1_MessageTransmit(0x500, 8, message, 0, CANFD_MODE_NORMAL, CANFD_MSG_TX_DATA_FRAME))
            {
                LED2_Toggle();
            }
            
            ADCHS_ChannelConversionStart(ADCHS_CH3);


        }
        
     /*   if(calibrate_btn_Get() == 1)
        {
            while(calibrate_btn_Get() == 1)
            {
                
            }
            ADCHS_ChannelConversionStart(ADCHS_CH3);
            while(ADCHS_ChannelResultIsReady(ADCHS_CH3) == 0)
            {
                
            }
            valor_left = (ADCHS_ChannelResultGet(ADCHS_CH3) *3.3/4095)*100;
            while(calibrate_btn_Get() == 0)
            {
                
            }
            while(calibrate_btn_Get() == 1)
            {
                
            }
            ADCHS_ChannelConversionStart(ADCHS_CH3);
            while(ADCHS_ChannelResultIsReady(ADCHS_CH3) == 0)
            {
                
            }
            valor_right = (ADCHS_ChannelResultGet(ADCHS_CH3) *3.3/4095)*100;
            EEPROM_WordWrite( address, valor_left);
            while ( EEPROM_IsBusy() == true )
            {
            address = address + 4;
            }
            EEPROM_WordWrite( address, valor_left);
            while ( EEPROM_IsBusy() == true )
            {
            address = address + 4;
            }
 
        
        }*/
        SYS_Tasks ( );
    /* Execution should not come here during normal operation */

    //return ( EXIT_FAILURE );
    }
}




unsigned int millis(void)
{
  return (unsigned long)(CORETIMER_CounterGet() / (CORE_TIMER_FREQUENCY / 1000));
}


/*******************************************************************************
 End of File
*/

