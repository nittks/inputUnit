
//----------------------------------------
// �S��
//----------------------------------------
typedef enum{
	UART_STATE_STANDBY,		//��M��~(�I��
	UART_STATE_TRANS,		//���M��
	UART_STATE_RECEIVE		//��M��
}UART_STATE;

#define RS485PORT	PORTC
#define RS485BIT	PC5
#define RS485_RX	(RS485PORT &= ~(1<<RS485BIT))
#define RS485_TX	(RS485PORT |=  (1<<RS485BIT))

//----------------------------------------
// ���M
//----------------------------------------
#define UART_REG_UDRE	(UCSR0A & (1<<UDRIE0))
#define UDRE_EMPTY		(1<<UDRIE0)
#define UDRE_NOEMPTY	(0<<UDRIE0)
#define DI_UART_TX						(UCSR0B = UCSR0B & (~(1<<TXEN0)))	//���M�֎~
#define EN_UART_TX						(UCSR0B = UCSR0B | ( (1<<TXEN0)))	//���M����
#define DI_INTER_UART_TX_REG_EMPTY		(UCSR0B = UCSR0B & (~(1<<UDRIE0)))	//���M�o�b�t�@�󊄍��݋֎~
#define EN_INTER_UART_TX_REG_EMPTY		(UCSR0B = UCSR0B | ( (1<<UDRIE0)))	//���M�o�b�t�@�󊄍��݋���
#define DI_INTER_UART_TX_FIN			(UCSR0B &= (~(1<<TXCIE0)))		//���M���������݋���
#define EN_INTER_UART_TX_FIN			(UCSR0B |= ( (1<<TXCIE0)))		//���M���������݋���
//----------------------------------------
// ��M
//----------------------------------------
#define UART_REG_RXC	(UCSR0A & (1<<RXC0))	//UART��M�����t���O
#define RXC_IN_DATA		(1<<RXC0)		//��M�f�[�^�L��
#define RXC_NO_DATA		(0<<RXC0)		//��M�f�[�^����

#define RX_BUF_SIZE		0xF;	//��M�o�b�t�@�T�C�Y

#define UART_DATAPOS_ID			0		//�f�[�^�ʒuID
#define UART_DATAPOS_LENGTH		1		//�f�[�^�ʒu�����O�X
#define UART_FRAME_TIMEOUT		2		//�^�C���A�E�g(10(�t���[������)-7(���M����)=2(�󂫎���

#define UART_ID_CARDATA			0x11		//�t���[��ID
#define UART_ID_SETTING			0x21		//�t���[��ID

#define DI_UART_RX						(UCSR0B = UCSR0B & (~(1<<RXEN0)))	//��M�֎~
#define EN_UART_RX						(UCSR0B = UCSR0B | ( (1<<RXEN0)))	//��M����
#define DI_INTER_UART_RX_COMP			(UCSR0B = UCSR0B & (~(1<<RXCIE0)))	//��M���������݋֎~
#define EN_INTER_UART_RX_COMP			(UCSR0B = UCSR0B | ( (1<<RXCIE0)))	//��M���������݋���
