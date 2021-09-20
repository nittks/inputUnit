#include <avr/io.h>

#include "lnkInPalseCnt.h"
#include "lnkInCarSw.h"
#include "lnkInEep.h"
#include "lnkInCom.h"

void initLnkIn( void )
{
	initLnkInPalseCnt();
	initLnkInCarSw();
	initLnkInEep();
	initLnkInCom();
}

void lnkInMain( void )
{
	lnkInPalseCntMain();
	lnkInCarSwMain();
	lnkInEepMain();
	lnkInComMain();
}
