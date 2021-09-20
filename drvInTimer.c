
#include <avr/io.h>
#include <avr/interrupt.h>
#include "drvInTimer_inc.h"
#include "drvInTimer.h"
#include "aplData.h"

static DRV_IN_TIMER		drvInTimer[DRV_IN_TIMER_NUM];

static TIMER_INS		timerIns[DRV_IN_TIMER_NUM];
static TIMER_STATE		timerRegState;		//�}�C�R���^�C�}���

//************************************************************
//������
//************************************************************
void initDrvInTimer( void )
{
	unsigned char	i;

	for(i=0;i<DRV_IN_TIMER_NUM;i++){
		drvInTimer[i].state	= DRV_IN_TIMER_STATE_STOP;
		drvInTimer[i].cnt	= 0;
		timerIns[i].cnt	= 0;
		timerIns[i].startCnt	= 0;
	}

	timerRegState = TIMER_STATE_STOP;
	
	SET_TCCR1A;
	SET_TCCR1B;
	SET_OCR1A;

}

//************************************************************
//���C������
//************************************************************
void drvInTimerMain( void )
{
}

//************************************************************
// �^�C�}�X�^�[�g
//************************************************************
void startDrvInTimer( DRV_IN_TIMER_ID timerId )
{
	unsigned short	inTimerReg;

	cli();
	//���W�X�^����^�C�}�l�擾(�덷�����Ȃ����邽�߁A�ŏ��ɓǎ��
	inTimerReg	= GET_TIMER_CNT;

	//�}�C�R���̃^�C�}���~�܂��Ă��鎞�́A�^�C�}�@�\����J�n������
	if( timerRegState == TIMER_STATE_STOP ){
		SET_TIMER_CNT( 0 );
		inTimerReg	= 0;
		EN_INTER_OVERFLOW_1A;
		START_TIMER_1A;
		timerRegState = TIMER_STATE_START;
	}
	//�X�^�[�g���N���A
	drvInTimer[timerId].state		= DRV_IN_TIMER_STATE_START;
	drvInTimer[timerId].cnt			= 0;

	timerIns[timerId].cnt		= 0;
	timerIns[timerId].startCnt	= inTimerReg;
	
	sei();
}

//************************************************************
// �^�C�}�N���A
//************************************************************
void clearDrvInTimer( DRV_IN_TIMER_ID timerId )
{
	drvInTimer[timerId].cnt		= 0;
	
}
//************************************************************
// �^�C�}�X�g�b�v
//************************************************************
void stopDrvInTimer( DRV_IN_TIMER_ID timerId )
{
	unsigned char	i;
	DRV_IN_TIMER_STATE		chkTimerState;

	cli();
	//�S�Ẵ^�C�}���~�܂��Ă�����A�}�C�R���̃^�C�}���~�߂�
	chkTimerState = DRV_IN_TIMER_STATE_STOP;
	for(i=0;i<DRV_IN_TIMER_NUM;i++){
		if( drvInTimer[i].state	!= DRV_IN_TIMER_STATE_STOP ){
			chkTimerState = DRV_IN_TIMER_STATE_START;	//�����Ă���
		}
	}
	if( (chkTimerState == DRV_IN_TIMER_STATE_STOP ) &&
		(timerRegState == TIMER_STATE_START)){
		STOP_TIMER_1A;
		DI_INTER_OVERFLOW_1A;
		SET_TIMER_CNT( 0 );
	}

	drvInTimer[timerId].state	= DRV_IN_TIMER_STATE_STOP;
	drvInTimer[timerId].cnt		= 0;
	timerIns[timerId].cnt		= 0;
	timerIns[timerId].startCnt	= 0;

	sei();
}
//************************************************************
// �^�C�}�l�擾
//************************************************************
DRV_IN_TIMER getDrvInTimer( DRV_IN_TIMER_ID timerId )
{
	unsigned short	inTimerReg;
	
	cli();

	//���W�X�^����^�C�}�l�擾
	inTimerReg	= GET_TIMER_CNT;

	//�v���J�n�^�C�}�J�E���g���W�X�^�l���猻�݂̒l�������A�o�ߎ��Ԃ��o��
	timerIns[timerId].cnt += (inTimerReg - timerIns[timerId].startCnt);

	//�I�[�o�[�t���[(0km/h)���o
	if(	timerIns[timerId].cnt > TIMER_CNT_MAX[N1_04] ){
		timerIns[timerId].state	= DRV_IN_TIMER_STATE_OVERFLOW;
	}
	//���J�p�ϐ��փR�s�[
	drvInTimer[timerId].cnt		= timerIns[timerId].cnt;
	//���̎����J�E���g�p�ɃN���A
	timerIns[timerId].cnt		= 0;
	timerIns[timerId].startCnt = inTimerReg;		//�v���J�n�J�E���g�l�X�V

	
	sei();

	//get�����u�Ԃ̃f�[�^���~�������߁A���Ԃ�Ԃ�
	//(�|�C���^���Ǝg�p����O�Ɋ��荞�݂ɂ��l���ω�����\��������
	return( drvInTimer[timerId] );
}
//************************************************************
// �^�C�}��Ԃ̂ݎ擾
//************************************************************
DRV_IN_TIMER getDrvInTimerState( DRV_IN_TIMER_ID timerId )
{
	return( drvInTimer[timerId] );
}
//************************************************************
// �}�C�R���̃^�C�}�I�[�o�[�t���[
//************************************************************
void interDrvInTimerOverflow( void )
{
	unsigned char	i;
	
	cli();
	
	for(i=0;i<DRV_IN_TIMER_NUM;i++){
		//�v���J�n�^�C�}�J�E���g���W�X�^�l���猻�݂̒l�������A�o�ߎ��Ԃ��o��
		timerIns[i].cnt += (TIMER_REG_MAX - timerIns[i].startCnt);
		timerIns[i].startCnt = 0;		//�v���J�n�J�E���g�l�X�V
	//�I�[�o�[�t���[(0km/h)���o
		if(	timerIns[i].cnt > TIMER_CNT_MAX[N1_04] ){
			drvInTimer[i].state	= DRV_IN_TIMER_STATE_OVERFLOW;
//			drvInTimer[i].cnt		= 0;
//			stopTimer(i);
		}
	}
	
	sei();
}



