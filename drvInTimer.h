
typedef enum{
	DRV_IN_TIMER_ID_SPEED,
	DRV_IN_TIMER_ID_REV,
	DRV_IN_TIMER_NUM,
}DRV_IN_TIMER_ID;

typedef enum{
	DRV_IN_TIMER_STATE_STOP,
	DRV_IN_TIMER_STATE_START,
	DRV_IN_TIMER_STATE_OVERFLOW,
	DRV_IN_TIMER_STATE_NUM
}DRV_IN_TIMER_STATE;

typedef struct{
	DRV_IN_TIMER_STATE		state;		//タイマ状態
	unsigned long	cnt;		//タイマカウント値
}DRV_IN_TIMER;

extern void initDrvInTimer( void );
extern void drvInTimerMain( void );
extern void startDrvInTimer( DRV_IN_TIMER_ID drvInTimerId );
extern void clearDrvInTimer( DRV_IN_TIMER_ID drvInTimerId );
extern void stopDrvInTimer( DRV_IN_TIMER_ID drvInTimerId );
extern DRV_IN_TIMER getDrvInTimer( DRV_IN_TIMER_ID drvInTimerId );
extern DRV_IN_TIMER getDrvInTimerState( DRV_IN_TIMER_ID drvInTimerId );
extern void interDrvInTimerOverflow( void );
