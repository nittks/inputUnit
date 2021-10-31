
typedef enum{
	DRV_IN_PALSE_CNT_NO_SPEED,
	DRV_IN_PALSE_CNT_NO_REV,
	DRV_IN_PALSE_CNT_NO_MAX
}PALSE_NO;
typedef struct{
	unsigned long	cycCnt[DRV_IN_PALSE_CNT_NO_MAX];
}DRV_IN_PALSE_CNT;

extern void initDrvInPalseCnt( void );
extern DRV_IN_PALSE_CNT *getDrvInPalesCnt( void );
extern void drvInPalseCntMain( void );
extern void interPosEdgeSpeed( void );
extern void interPosEdgeRev( void );
