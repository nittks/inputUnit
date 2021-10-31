#include "hardware.h"

typedef enum{
	APL_COM_STATE_STOP,
	APL_COM_STATE_START,
	APL_COM_STATE_MAX
}APL_COM_STATE;


typedef struct {
	unsigned char	speed;
	unsigned short	rev;
	unsigned char	palseSpeed;
	unsigned char	palseRev;
	unsigned char	ig;
	unsigned char	acc;
	unsigned char	ill;
	unsigned char	vtc;

	APL_COM_STATE	comState;
}APL_COM_DATA;


extern void initAplComData( void );
extern void aplComDataMain( void );
extern APL_COM_DATA *getAplComData( void );
