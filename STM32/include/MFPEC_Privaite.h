/*
 * MFPEC_Privaite.h
 *
 *  Created on: Apr 4, 2024
 *      Author: Ahmed Megahed
 */

#ifndef MFPEC_PRIVAITE_H_
#define MFPEC_PRIVAITE_H_


typedef struct{

	volatile u32 ACR      ;
	volatile u32 KEYR     ;
	volatile u32 OPTKEYR  ;
	volatile u32 SR       ;
	volatile u32 CR       ;
	volatile u32 OPTCR    ;
}FPEC_Type;

#define MFPEC  ((volatile FPEC_Type*)0x40023C00)

#define  FPEC_SECTOR_MASK            0xFFFFFF87

#define FPEC_Flash_WRITE_SIZE_MASK   0xFFFFFCFF


#define WRITE_BYTE             0
#define WRITE_HALF_WORD        1
#define WRITE_WORD             2
#define WRITE_DOUBLE_WORD      3

#endif /* MFPEC_PRIVAITE_H_ */
