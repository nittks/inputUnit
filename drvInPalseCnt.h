
typedef enum{
	DRV_IN_PALSE_CNT_NO_SPEED,
	DRV_IN_PALSE_CNT_NO_REV,
	DRV_IN_PALSE_CNT_NO_MAX
}PALSE_NO;
typedef struct{
	unsigned long	cyc100ns[DRV_IN_PALSE_CNT_NO_MAX];	// LSB 1=100ns
}DRV_IN_PALSE_CNT;

extern void initDrvInPalseCnt( void );
extern DRV_IN_PALSE_CNT *getDrvInPalesCnt( void );
extern void drvInPalseCntMain( void );
extern void interPortF( void );
