
//----------------------------------------
// ���M
//----------------------------------------
#define DRV_UART_TX_BUF_SIZE	8
typedef struct{
	unsigned char	txData[DRV_UART_TX_BUF_SIZE];
	unsigned char	txDataNum;
}DRV_UART_TX;

//----------------------------------------
// ��M
//----------------------------------------
#define DRV_UART_RX_BUF_SIZE	6
#define UART_RX_ID			(0x11)

typedef struct{
	unsigned char	rxData[DRV_UART_RX_BUF_SIZE];
	unsigned char	rxDataNum;
	unsigned char	rxFlag;
}DRV_UART_RX;


//----------------------------------------
// ���J�֐�
//----------------------------------------
extern void initDrvUart( void );
extern void drvUartMain( void );
extern void interSetUartTxData(void);
extern void interUartTxFin(void);
extern void interGetUartRxData(void);
extern void setDrvUartTx( DRV_UART_TX *inP );
extern DRV_UART_RX *getDrvUartRx( void );