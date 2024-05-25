#include "BIT_MATH.h"                // Include bit manipulation utilities
#include "STD_TYPES.h"               // Include standard type definitions
#include "MRCC_interface.h"          // Include RCC interface definitions
#include "GPIO_interface.h"          // Include GPIO interface definitions
#include "MUSART_Interface.h"        // Include USART interface definitions
#include "SYSTICK_interface.h"       // Include SYSTICK interface definitions
#include "MFPEC_Interface.h"         // Include flash memory interface definitions
#include "SPARSER_Interface.h"       // Include S-record parser interface definitions

// Define a function pointer type for application calls
pf App_Call;

// Control variables
volatile u8 ctrlDataListen = 0;           // Control data listen flag
volatile u8 ctrlDataRec = 0;              // Control data receive flag
volatile u8 u8Rec_Buffer[100] = {0};      // Buffer for received data
volatile u8 u8Rec_Counter = 0;            // Counter for received data
volatile u8 u8Write_Req = 1;              // Write request flag
volatile u8 TimeoutFlag = 0;              // Timeout flag

// Function to jump to Application 1
void App1_Jump(void)
{
    TimeoutFlag = 1;                      // Set the timeout flag

    // Set the vector table offset for App1
    #define SCB_VTOR  *((volatile u32*)(0xE000ED08))
    SCB_VTOR = 0x08004000;

    // Set application call address for App1
    App_Call = *(pf*)0x08004004;

    MGPIO_vWritePin(PORT_A, PIN_1, GPIO_HIGH);  // Indicate App1 call
    App_Call();                                 // Call the application
}

// Function to jump to Application 2
void App2_Jump(void)
{
    TimeoutFlag = 1;                      // Set the timeout flag

    // Set the vector table offset for App2
    #define SCB_VTOR  *((volatile u32*)(0xE000ED08))
    SCB_VTOR = 0x08008000;

    // Set application call address for App2
    App_Call = *(pf*)0x08008004;
    MGPIO_vWritePin(PORT_A, PIN_2, GPIO_HIGH);  // Indicate App2 call
    App_Call();                                 // Call the application
}

void main(void)
{
    // Initialize the RCC (Reset and Clock Control) and enable required peripherals
    MRCC_vInt();
    MRCC_vEnable(RCC_AHB1, AHB1_GPIOA);       // Enable GPIOA clock
    MRCC_vEnable(RCC_APB2, APB2_USART1);      // Enable USART1 clock
    MRCC_vEnable(RCC_AHB1LP, AHB1LP_FLASH);   // Enable Flash memory clock

    // Configure GPIO pins for UART
    MGPIO_vSetMode(PORT_A, PIN_9, MODE_ALTERNATIVE);              // Set PA9 to alternative function mode
    MGPIO_vSetMode(PORT_A, PIN_10, MODE_ALTERNATIVE);             // Set PA10 to alternative function mode
    MGPIO_vSetAlternativeMode(PORT_A, PIN_9, ALTR_UART);          // Set PA9 to UART alternative function
    MGPIO_vSetAlternativeMode(PORT_A, PIN_10, ALTR_UART);         // Set PA10 to UART alternative function

    // Configure GPIO pins for output
    MGPIO_vSetMode(PORT_A, PIN_0, MODE_OUTPUT);                   // Set PA0 to output mode
    MGPIO_vSetOutputMode(PORT_A, PIN_0, OUTPUT_PUSHPULL, SPEED_LOW); // Set PA0 to push-pull, low speed
    MGPIO_vSetMode(PORT_A, PIN_1, MODE_OUTPUT);                   // Set PA1 to output mode
    MGPIO_vSetOutputMode(PORT_A, PIN_1, OUTPUT_PUSHPULL, SPEED_LOW); // Set PA1 to push-pull, low speed
    MGPIO_vSetMode(PORT_A, PIN_2, MODE_OUTPUT);                   // Set PA2 to output mode
    MGPIO_vSetOutputMode(PORT_A, PIN_2, OUTPUT_PUSHPULL, SPEED_LOW); // Set PA2 to push-pull, low speed
    MGPIO_vSetMode(PORT_A, PIN_3, MODE_OUTPUT);                   // Set PA3 to output mode
    MGPIO_vSetOutputMode(PORT_A, PIN_3, OUTPUT_PUSHPULL, SPEED_LOW); // Set PA3 to push-pull, low speed
    MGPIO_vSetMode(PORT_A, PIN_4, MODE_OUTPUT);                   // Set PA4 to output mode
    MGPIO_vSetOutputMode(PORT_A, PIN_4, OUTPUT_PUSHPULL, SPEED_LOW); // Set PA4 to push-pull, low speed

    // Initialize UART
    MSUART_voidInit();
    MUSART_voidEnable(1);                                           // Enable USART1
    MUSART_u8SendData(1, (u8*)"felfel", 6);                         // Send initial data

    // Initialize SysTick
    MSYSYTICK_voidInit();
    MSYSYTICK_voidCtrlIntState(SYSTICKEnable);                      // Enable SysTick interrupt
    MGPIO_vWritePin(PORT_A, PIN_0, GPIO_HIGH);                      // Set PA0 high

    u8 u8Rec_Status = 0;                                            // Receive status flag

    // Set single interval timer for App1 jump
    MSTK_voidSetInterval_single(16000000, App1_Jump);

    // Listen for incoming data
    u8Rec_Status = MUSART1_u8ReceiveCharSync2(&ctrlDataListen);
    if (u8Rec_Status)
    {
        MSYSYTICK_voidStop();                                       // Stop SysTick
        u8Rec_Status = 0;
    }

    // Receive control data
    u8Rec_Status = MUSART1_u8ReceiveCharSync2(&ctrlDataRec);

    while (TimeoutFlag == 0)
    {
        // Handle received control data
        if (ctrlDataRec == 'A') // Flash firmware in APP 1 in Sector 1
        {
            u8Rec_Status = MUSART1_u8ReceiveCharSync(&u8Rec_Buffer[u8Rec_Counter]);
            if (u8Rec_Status == 1)
            {
                MSYSYTICK_voidStop();                              // Stop SysTick
                if (u8Rec_Buffer[u8Rec_Counter] == '\n')
                {
                    if (u8Write_Req == 1)
                    {
                        MFPEC_voidSectorErase(1);                 // Erase Sector 1
                        u8Write_Req = 0;
                    }
                    PARSE_voidParseRecord(u8Rec_Buffer);          // Parse the S-record
                    MUSART1_voidSendSrtingSynch((u8 *)"ok");      // Send acknowledgment
                    u8Rec_Counter = 0;
                }
                else
                {
                    u8Rec_Counter++;
                }

                if (u8Rec_Buffer[8] == '1')
                {
                    MSYSYTICK_voidInit();                         // Re-initialize SysTick
                    MSYSYTICK_voidCtrlIntState(SYSTICKEnable);    // Enable SysTick interrupt
                    MSTK_voidSetInterval_single(2000000, App1_Jump); // Set interval for App1 jump
                }
            }
        }
        else if (ctrlDataRec == 'B') // Flash firmware in APP 2 in Sector 2
        {
            u8Rec_Status = MUSART1_u8ReceiveCharSync(&u8Rec_Buffer[u8Rec_Counter]);
            if (u8Rec_Status == 1)
            {
                MSYSYTICK_voidStop();                              // Stop SysTick
                if (u8Rec_Buffer[u8Rec_Counter] == '\n')
                {
                    if (u8Write_Req == 1)
                    {
                        MFPEC_voidSectorErase(2);                 // Erase Sector 2
                        u8Write_Req = 0;
                    }
                    PARSE_voidParseRecord(u8Rec_Buffer);          // Parse the S-record
                    MUSART1_voidSendSrtingSynch((u8 *)"ok");      // Send acknowledgment
                    u8Rec_Counter = 0;
                }
                else
                {
                    u8Rec_Counter++;
                }

                if (u8Rec_Buffer[8] == '1')
                {
                    MSYSYTICK_voidInit();                         // Re-initialize SysTick
                    MSYSYTICK_voidCtrlIntState(SYSTICKEnable);    // Enable SysTick interrupt
                    MSTK_voidSetInterval_single(2000000, App2_Jump); // Set interval for App2 jump
                }
            }
        }
        else if (ctrlDataRec == 'E') // Erase APP 1 in Sector 1
        {
            MFPEC_voidSectorErase(1);                             // Erase Sector 1
        }
        else if (ctrlDataRec == 'F') // Erase APP 2 in Sector 2
        {
            MFPEC_voidSectorErase(2);                             // Erase Sector 2
        }
        else if (ctrlDataRec == 'C') // Jump to APP 1 in Sector 1
        {
            App1_Jump();
        }
        else if (ctrlDataRec == 'D') // Jump to APP 2 in Sector 2
        {
            App2_Jump();
        }
    }
}
