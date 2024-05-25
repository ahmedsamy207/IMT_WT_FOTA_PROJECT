/*******************************************************************************************************/                                       */
/* Module  Features  :                                                                                 */
/*      01- MSUART_voidInit                                                                            */
/*      02- MUSART_voidEnable                                                                          */
/*      03- MUSART_voidDisable                                                                         */
/*      04- MSUART_u8SendData                                                                          */
/*      05- MUSART_u8ReadData                                                                          */
/*      06- MUSART_GetHandler                                                                          */
/*******************************************************************************************************/


/*******************************************************************************************************/
/*                                      Standard Types LIB                                             */
/*******************************************************************************************************/

#ifndef UART_INTERFACE_H
#define UART_INTERFACE_H



void MSUART_voidInit(void);

void MUSART_voidEnable(u8 copy_u8Index);

void MUSART_voidDisable(u8 copy_u8Index);

void MUSART_u8SendData(u8 copy_u8Index, u8* copy_u8Data, u8 copy_u8Len);

u8 MUSART_u8ReadData(u8 copy_u8Index);

void MUSART1_voidSendCharSynch( u8 Copy_u8Char ) ;

void MUSART1_voidSendSrtingSynch( u8 *  Copy_u8String ) ;
u8 MUSART1_u8ReceiveCharSync( u8* Copy_u8DataCome ) ;
u8 MUSART1_u8ReceiveCharSync2( u8 *  Copy_u8DataCome );



#endif //UART_INTERFACE_H_
