#ifndef APL_DATA_H
#define APL_DATA_H

typedef enum{
	APL_DATA_SW_OFF,
	APL_DATA_SW_ON
}APL_DATA_SW;

typedef enum{
	APL_DATA_EEP_STATE_UNREAD,
	APL_DATA_EEP_STATE_READED,
	APL_DATA_EEP_STATE_SUMERROR
}APL_DATA_EEP_STATE;

typedef struct{
	unsigned char	speed;
	unsigned short	rev;
}APL_DATA_PALSE;

typedef struct{
	APL_DATA_SW		ig;
	APL_DATA_SW		acc;
	APL_DATA_SW		ill;
	APL_DATA_SW		vtc;
}APL_DATA_CAR_SW;

typedef struct{
	unsigned char		chReq;
	unsigned char		palseSpeed;
	unsigned char		palseRev;
}APL_DATA_COM;

typedef struct{
	APL_DATA_EEP_STATE	read;
	unsigned char		palseSpeed;
	unsigned char		palseRev;
}APL_DATA_EEP;


//********************************************************************************
// 公開関数
//********************************************************************************
extern void initAplData( void );
extern void aplDataMain( void );
extern void setAplDataPalse( APL_DATA_PALSE * inData );
extern void setAplDataCarSw( APL_DATA_CAR_SW * inData );
extern void setAplDataEep( APL_DATA_EEP * inData );
extern void setAplDataCom( APL_DATA_COM * inData );
extern APL_DATA_PALSE *getAplDataPalse( void );
extern APL_DATA_CAR_SW *getAplDataCarSw( void );
extern APL_DATA_COM *getAplDataCom( void );
extern APL_DATA_EEP *getAplDataEep( void );
#endif
