//********************************************************************************
// �W������p�萔
//********************************************************************************
#define		SPEED_MAX		((unsigned char)200)
#define		SPEED_MIN		((unsigned char)0)
#define		REV_MAX			((unsigned short)10000)
#define		REV_MIN			((unsigned short)0)


//********************************************************************************
// �e�X�g���[�h
//********************************************************************************
//�e�X�g�f�[�^�v�Z����
#define		TEST_CYC_SPEED	((unsigned char)10-1)	//10x10ms=100ms������
#define		TEST_CYC_REV	((unsigned char)2-1)	//10x10ms=100ms������
//�v�Z���̕ω��l
#define		CHG_VAL_SPEED	((unsigned char)2)
#define		CHG_VAL_REV		((unsigned char)200)
//���[�^���[�N���b�N���̕ω��l
#define		CHG_VAL_SPEED_MANUAL	((unsigned char)5)
#define		CHG_VAL_REV_MANUAL		((unsigned char)200)


typedef enum{
	TEST_STATE_UP,
	TEST_STATE_DOWN
}TEST_STATE;

//********************************************************************************
// �ԑ̐M��
//********************************************************************************
enum{
	APL_COM_SW_OFF,
	APL_COM_SW_ON
};
