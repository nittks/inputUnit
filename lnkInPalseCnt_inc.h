

typedef enum{
	NO_SPEED,
	NO_REV,
	NO_MAX
}PORT_NO;

#define N1		4		//1��]������̎ԑ��Z���T�p���X��
#define N2		637		//60km/h���̎ԑ��Z���T��]���xrpm
#define C		2		//1��]������̓_�Ή�
#define R_SP	2		//1�_�Γ�����̉�]��(4st=2,1st=1

#define INIT_PALSE_SPEED	4	//1��]������̃p���X��
#define INIT_PALSE_REV		4	//�C����

#define TIME_US	1000000		//us��s�Ɋ��Z���v�Z
#define ROUND_DIGIT	10		//�����_��1�ʂŊۂ߂�B10�{����5�𑫂���10�Ŋ���
#define ROUND5		5		//�l�̌ܓ��B5�𑫂���10�Ŋ���

const unsigned char SETTING_PALSE_SPEED[6]=	{0,4,8,16,20,25};		//�ԑ��p���X�d�l
const unsigned char SETTING_PALSE_REV[9]={0,1,2,3,4,5,6,8,10};	//��]���p���X�d�l(�C����
