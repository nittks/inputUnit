
//************************************************************
// 外部割込みレジスタ関係
//************************************************************
#define		REG_ISC0		(3)		//立上り時
#define		REG_ISC1		(3)		//立上り時
#define		REG_INT0		(1)		//許可
#define		REG_INT1		(1)		//許可

#define		SET_EICRA		(EICRA = ((REG_ISC0<<ISC00) | (REG_ISC1<<ISC10)))
#define		SET_EIMSK		(EIMSK = ((REG_INT0<<INT0) | (REG_INT1<<INT1)))

typedef enum{
	PORT_SPEED,
	PORT_REV,
	PORT_MAX
}PORT_NO;




