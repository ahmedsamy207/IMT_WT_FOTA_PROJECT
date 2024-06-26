#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include "MFPEC_Interface.h"
#include "SPARSER_Interface.h"


u16 u16Data[100]   = {0}        ;
u32 u32Address     = 0x08000000 ;

u8   PARSE_u8AsciToHex     ( u8 Copy_u8Asci      ){

	u8 LOC_u8Result = 0 ;

	if( ( Copy_u8Asci >= 48 ) && ( Copy_u8Asci <= 57 ) ){

		LOC_u8Result = Copy_u8Asci - 48 ;

	}else{

		LOC_u8Result = Copy_u8Asci - 55 ;

	}

	return LOC_u8Result ;

}

void PARSE_voidParseData   ( u8 * Copy_u8BufData ){

	u8 LOC_u8DigitLow    , LOC_u8DigitHigh  , LOC_u8CC         , LOC_u8Iterator ;
	u8 LOC_u8DataDigit0  , LOC_u8DataDigit1 , LOC_u8DataDigit2 , LOC_u8DataDigit3 ;

	LOC_u8DigitHigh = PARSE_u8AsciToHex( Copy_u8BufData[1] );
	LOC_u8DigitLow  = PARSE_u8AsciToHex( Copy_u8BufData[2] );

	LOC_u8CC        = ( LOC_u8DigitHigh << 4 ) | LOC_u8DigitLow ;


	LOC_u8DataDigit0 = PARSE_u8AsciToHex( Copy_u8BufData[3] );
	LOC_u8DataDigit1 = PARSE_u8AsciToHex( Copy_u8BufData[4] );
	LOC_u8DataDigit2 = PARSE_u8AsciToHex( Copy_u8BufData[5] );
	LOC_u8DataDigit3 = PARSE_u8AsciToHex( Copy_u8BufData[6] );

	u32Address = u32Address & 0xFFFF0000;
	u32Address = ( u32Address ) | ( LOC_u8DataDigit0 << 12 ) | ( LOC_u8DataDigit1 << 8 ) | ( LOC_u8DataDigit2 << 4 ) | ( LOC_u8DataDigit3 );

	for( LOC_u8Iterator = 0 ; LOC_u8Iterator < LOC_u8CC / 2 ; LOC_u8Iterator++ ){

		LOC_u8DataDigit0 = PARSE_u8AsciToHex( Copy_u8BufData[ ( 4 * LOC_u8Iterator ) + 9  ] );
		LOC_u8DataDigit1 = PARSE_u8AsciToHex( Copy_u8BufData[ ( 4 * LOC_u8Iterator ) + 10 ] );
		LOC_u8DataDigit2 = PARSE_u8AsciToHex( Copy_u8BufData[ ( 4 * LOC_u8Iterator ) + 11 ] );
		LOC_u8DataDigit3 = PARSE_u8AsciToHex( Copy_u8BufData[ ( 4 * LOC_u8Iterator ) + 12 ] );

		u16Data [LOC_u8Iterator] = ( LOC_u8DataDigit2 << 12 ) | ( LOC_u8DataDigit3 << 8 ) | ( LOC_u8DataDigit0 << 4 ) | ( LOC_u8DataDigit1 );

	}

	MFPEC_voidFlashWrite( u32Address , u16Data , LOC_u8CC / 2 );

}

void PARSE_voidParseRecord ( u8 * Copy_u8BufData ){

	switch( Copy_u8BufData[8] ){

	case '0' : PARSE_voidParseData( Copy_u8BufData );  break;
	case '4' : /* Set High address*/                   break;
	case '1' : /*End Of File*/                         break;

	}

}
