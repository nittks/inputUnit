enum{
	DRV_EEP_MAP_PALSE_SPEED,
	DRV_EEP_MAP_PALSE_REV,
	DRV_EEP_MAP_SUM,
	DRV_EEP_MAP_MAX
};

typedef struct{
	unsigned char	readState;
	unsigned char	val[DRV_EEP_MAP_MAX];
}DRV_EEP_READ;

typedef struct{
	unsigned char	val[DRV_EEP_MAP_MAX];
}DRV_EEP_WRITE;

enum{
	DRV_EEP_READ_STATE_UNREAD,
	DRV_EEP_READ_STATE_READED
};

extern void initDrvEep( void );
extern DRV_EEP_READ *getDrvEep( void );
extern unsigned char setDrvEep( DRV_EEP_WRITE *inP );
extern void interEepRedy( void );
