#define	TIMER_REG_LSB		1		//1cnt=10us
#define	TIMER_REG_MAX		0xFFFF	//60ms毎、オーバーフロー割り込みを入れる

#define		START_TIMER_1A		(TCB0.CTRLA |= TCB_ENABLE_bm)
#define		STOP_TIMER_1A		(TCB0.CTRLA &= (~TCB_ENABLE_bm))
#define		EN_INTER_OVERFLOW_1A	(TCB0.INTCTRL |= TCB_CAPT_bm)
#define		DI_INTER_OVERFLOW_1A	(TCB0.INTCTRL &= (~TCB_CAPT_bm))

#define		GET_TIMER_CNT		(TCB0.CNT)
#define		SET_TIMER_CNT(data)	(TCB0.CNT=data)

#define		SEC60		60
#define		KMPERH60	60
#define		V1KMPERH	1
#define		N2			637
#define		NS			((unsigned long)1000000)	//sec->ns
enum{
	N1_04,
	N1_08,
	N1_16,
	N1_20,
	N1_25,
	N1_MAX
};

enum{
	N1_PALSE_04 = 4,
	N1_PALSE_08 = 8,
	N1_PALSE_16 = 16,
	N1_PALSE_20 = 20,
	N1_PALSE_25 = 25
};

//タイマカウント最大値(1km/h未満になる周期)。0㎞/h(オーバーフロー)判定用
const static unsigned long TIMER_CNT_MAX[N1_MAX] = {
	(NS*SEC60*KMPERH60)/(V1KMPERH*(N1_PALSE_04 * N2)),
	(NS*SEC60*KMPERH60)/(V1KMPERH*(N1_PALSE_08 * N2)),
	(NS*SEC60*KMPERH60)/(V1KMPERH*(N1_PALSE_16 * N2)),
	(NS*SEC60*KMPERH60)/(V1KMPERH*(N1_PALSE_20 * N2)),
	(NS*SEC60*KMPERH60)/(V1KMPERH*(N1_PALSE_25 * N2))
};

typedef enum{
	TIMER_STATE_STOP,
	TIMER_STATE_START,
	TIMER_STATE_OVERFLOW,
	TIMER_STATE_NUM
}TIMER_STATE;

typedef struct{
	TIMER_STATE		state;		//タイマ状態
	unsigned long	cnt;		//タイマカウント値
	unsigned short	startCnt;	//タイマ動作開始時、マイコンタイマレジスタ値
}TIMER_INS;
