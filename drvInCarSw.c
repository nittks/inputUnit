
#define F_CPU 1000000UL
#include <avr/io.h>
#include <avr/interrupt.h>

#include "drvInCarSw_inc.h"
#include "drvInCarSw.h"

//Lnkが取得する
static DRV_IN_CAR_SW	drvInCarSw;

//********************************************************************************
// 初期化
//********************************************************************************
void initDrvInCarSw( void )
{
	drvInCarSw.ig	= DRV_IN_CAR_SW_OFF;
	drvInCarSw.ill	= DRV_IN_CAR_SW_OFF;
}

//********************************************************************************
// LINKが取得
//********************************************************************************
DRV_IN_CAR_SW *getDrvInCarSw( void )
{
	return( &drvInCarSw );
}

//********************************************************************************
// main
//********************************************************************************
void drvInCarSwMain( void )
{

	if( PORT_IG == ON ){
		drvInCarSw.ig	= DRV_IN_CAR_SW_ON;
	}else{
		drvInCarSw.ig	= DRV_IN_CAR_SW_OFF;
	}
	
	if( PORT_ACC == ON ){
		drvInCarSw.acc	= DRV_IN_CAR_SW_ON;
		}else{
		drvInCarSw.acc	= DRV_IN_CAR_SW_OFF;
	}

	if( PORT_ILL == ON ){
		drvInCarSw.ill	= DRV_IN_CAR_SW_ON;
	}else{
		drvInCarSw.ill	= DRV_IN_CAR_SW_OFF;
	}
	
	if( PORT_VTC == ON ){
		drvInCarSw.vtc	= DRV_IN_CAR_SW_ON;
		}else{
		drvInCarSw.vtc	= DRV_IN_CAR_SW_OFF;
	}

}
