#include <avr/io.h>

#include "drvInMain.h"
#include "drvInPalseCnt.h"
#include "drvInCarSw.h"
#include "drvInTimer.h"
#include "drvEep.h"

void initDrvIn( void )
{
	initDrvInPalseCnt();
	initDrvInCarSw();
	initDrvInTimer();

	initDrvEep();
	initDrvUart();
}

void drvInMain( void )
{
	drvInPalseCntMain();
	drvInCarSwMain();
	drvInTimerMain();
}
