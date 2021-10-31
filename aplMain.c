#include <avr/io.h>

#include "aplData.h"
#include "aplComData.h"
#include "aplCtrl.h"

void initApl( void )
{
	initAplData();
	initAplCtrl();
	initAplComData();
}

void aplMain( void )
{
	aplDataMain();
	aplCtrlMain();
	aplComDataMain();
}
