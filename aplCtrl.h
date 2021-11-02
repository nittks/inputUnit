#ifndef APL_CTRL_H
#define APL_CTRL_H

#include <avr/io.h>

//状態
typedef enum{
	APL_CTRL_STATE_BOOT,		//起動初回
	APL_CTRL_STATE_NOMARL		//通常
}APL_CTRL_STATE;

typedef struct {
	APL_CTRL_STATE		state;
}APL_CTRL;

typedef struct {
	unsigned char	palseSpeed;			//パルス仕様車速   
	unsigned char	palseRev;			//パルス仕様回転数 
}APL_CTRL_SET;



extern void initAplCtrl( void );
extern void aplCtrlMain( void );
extern APL_CTRL *getAplCtrl( void );
extern APL_CTRL_SET	*getAplCtrlSet( void );

#endif
