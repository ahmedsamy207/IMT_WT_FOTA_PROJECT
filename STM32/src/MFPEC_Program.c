/*
 * MFPEC_Program.c
 *
 *  Created on: Apr 4, 2024
 *      Author: Ahmed Megahed
 */

#include "STD_TYPES.h"
#include "BIT_MATH.h"



#include "MFPEC_Privaite.h"
#include "MFPEC_Config.h"
#include "MFPEC_Interface.h"

void MFPEC_voidEraseAppArea(void){

	u8 LOC_u8Iterator ;
	for ( LOC_u8Iterator = 1 ; LOC_u8Iterator < 6 ; LOC_u8Iterator++ ){

		MFPEC_voidSectorErase( LOC_u8Iterator );

	}

}

void MFPEC_voidSectorErase   ( u8 Copy_u8SectorNumber ){

	/*Step1: Wait till Busy flag = 0*/
	while( GET_BIT( MFPEC -> SR ,16 ) == 1 );

	/*Step2 :Set Key to unlock flash if lock bit = 1*/
	if( GET_BIT( MFPEC -> CR , 31 ) == 1 ){

		MFPEC -> KEYR = 0x45670123 ;
		MFPEC -> KEYR = 0xCDEF89AB ;

	}
	/*Step 3: Set SER bit to activate sector erase*/
	SET_BIT( MFPEC -> CR , 1 );

	/*Step 4: Select The Sector Number From 0 - 5 */

	MFPEC -> CR &= FPEC_SECTOR_MASK ;

	MFPEC -> CR |= (Copy_u8SectorNumber<<3) ;

	/*Step 5 : Start Erase Operation*/
	SET_BIT( MFPEC -> CR , 16 );

	/*Step6: Wait till Busy flag = 0*/
	while( GET_BIT( MFPEC -> SR ,16 ) == 1 );

	/*step7: Set EOP bit to CLRar it(End of operation)*/
	SET_BIT( MFPEC -> SR , 0 );

	/*Step 8: CLRar SER bit to deactivate sector erase*/
	CLR_BIT( MFPEC -> CR , 1 );

}

void MFPEC_voidFlashWrite( u32 Copy_u32Address , u16 * Copy_u16Data , u8 Copy_u8DataLength ){

	u8 LOC_u8Iterator ;

	/*Step1: Wait till Busy flag = 0*/
	while( GET_BIT( MFPEC -> SR ,16 ) == 1 );

	/*Step2 :Set Key to unlock flash if lock bit = 1*/
	if( GET_BIT( MFPEC -> CR , 31 ) == 1 ){

		MFPEC -> KEYR = 0x45670123 ;
		MFPEC -> KEYR = 0xCDEF89AB ;

	}
	/*Step 3: Select The Program size byte - halfword-word -DoubleWord */


	MFPEC -> CR &= FPEC_Flash_WRITE_SIZE_MASK ;

	MFPEC -> CR |= (WRITE_HALF_WORD<<8) ;

	for( LOC_u8Iterator = 0 ; LOC_u8Iterator < Copy_u8DataLength ; LOC_u8Iterator++ ){
		/*Step 4: Set PG bit to activate flash programming*/
		SET_BIT( MFPEC -> CR , 0 );

		*( (volatile u16 *)Copy_u32Address ) = Copy_u16Data[ LOC_u8Iterator ] ;
		Copy_u32Address += 2 ;

		/*Step5: Wait till Busy flag = 0*/
		while( GET_BIT( MFPEC -> SR ,16 ) == 1 );

		/*step6: Set EOP bit to CLRar it(End of operation)*/
		SET_BIT( MFPEC -> SR , 0 );

		/*Step7: CLRar PG bit to deactivate flash programming*/
		CLR_BIT( MFPEC -> CR , 0 );

	}

}

