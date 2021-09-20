#include <avr/io.h>

#include "lnkInCarSw_inc.h"
#include "lnkInCarSw.h"

#include "drvInCarSw.h"
#include "aplData.h"

//‰Šú‰»
void initLnkInCarSw( void )
{
}
//ƒƒCƒ“ˆ—
void lnkInCarSwMain( void )
{
	DRV_IN_CAR_SW	*inDrvInCarSw;
	APL_DATA_CAR_SW		aplDataCarSw;
	
	inDrvInCarSw	= getDrvInCarSw();

	//IG
	if( inDrvInCarSw->ig == DRV_IN_CAR_SW_OFF ){
		aplDataCarSw.ig	= APL_DATA_SW_OFF;
	}else{
		aplDataCarSw.ig	= APL_DATA_SW_ON;
	}

	//ACC
	if( inDrvInCarSw->acc == DRV_IN_CAR_SW_OFF ){
		aplDataCarSw.acc	= APL_DATA_SW_OFF;
	}else{
		aplDataCarSw.acc	= APL_DATA_SW_ON;
	}

	//ILL
	if( inDrvInCarSw->ill == DRV_IN_CAR_SW_OFF ){
		aplDataCarSw.ill	= APL_DATA_SW_OFF;
	}else{
		aplDataCarSw.ill	= APL_DATA_SW_ON;
	}
	
	//VTC
	if( inDrvInCarSw->vtc == DRV_IN_CAR_SW_OFF ){
		aplDataCarSw.vtc	= APL_DATA_SW_OFF;
	}else{
		aplDataCarSw.vtc	= APL_DATA_SW_ON;
	}

	//Test
	if( inDrvInCarSw->test == DRV_IN_CAR_SW_OFF ){
		aplDataCarSw.test	= APL_DATA_SW_OFF;
	}else{
		aplDataCarSw.test	= APL_DATA_SW_ON;
	}
	setAplDataCarSw( &aplDataCarSw );
}
