#include <avr/io.h>

#include "lnkOutMain.h"
#include "lnkOutCom.h"
#include "lnkOutEep.h"

void initLnkOut( void )
{
	initLnkOutCom();
	initLnkOutEep();
}

void lnkOutMain( void )
{
	lnkOutComMain();
	lnkOutEepMain();
}
