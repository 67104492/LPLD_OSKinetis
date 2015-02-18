/**
 * --------------����"��������K60�ײ��V3"�Ĺ��̣�LPLD_SerialInterrupt��-----------------
 * @file LPLD_SerialInterrupt.c
 * @version 0.1
 * @date 2013-9-29
 * @brief ����UARTģ�鼰���жϹ��ܣ�ʵ�ִ��ڵ������շ���
 *
 * ��Ȩ����:�����������µ��Ӽ������޹�˾
 * http://www.lpld.cn
 * mail:support@lpld.cn
 * Ӳ��ƽ̨:  LPLD K60 Card / LPLD K60 Nano
 *
 * �����̻���"��������K60�ײ��V3"������
 * ���п�Դ�������"lib"�ļ����£��û����ظ��ĸ�Ŀ¼�´��룬
 * �����û������豣����"project"�ļ����£��Թ����������ļ�������
 * �ײ��ʹ�÷���������ĵ��� 
 *
 */
#include "common.h"

/****************************************
 ˵����
   *��MiniUSB�߲���RUSH Kinetis�������USB
    ������������������USB�ӿڡ�
   *ʹ�ô��ڵ������ֲ���������Ϊ9600
   *ʹ�ô��ڵ������ֲ鿴���н��
 ****************************************/
#define USE                (0)
#define UN_USE             (1) 
#define USE_DMA_Tx_Rx      (UN_USE)
#define USE_FIFO_Tx_Rx     (UN_USE)

/****************************************
 ˵����
   1 \#define USE_DMA_Tx_Rx  (UN_USE)
      #define USE_FIFO_Tx_Rx (UN_USE)
        ��ʱ���ó�Ϊ���ڽ����ж�, ��ʹ��FIFO �� DMA ���ܣ�
        PC���͵����ݻᴥ�������жϣ����ж��н����������ٷ��͵�PC
   
   2 \#define USE_DMA_Tx_Rx  (UN_USE)
      #define USE_FIFO_Tx_Rx (USE)
        ��ʱ���÷���,����ΪFIFO��ʽ
                 __
                  /| TX_FIFO:|Dataword3|
                 /           |Dataword2|<----uart->watermark
                /            |Dataword1|_______TX_BUS
    data-->uart->D         
               |\           |Dataword3| _______RX_BUS
                 \          |Dataword2|<----uart->watermark
                  \ RX_FIFO:|Dataword1| 
        ���ͣ����յ�FIFO�����Ҫͨ����ȡ UART_PFIFO_TXFIFOSIZE �� 
        UART_PFIFO_TXFIFOSIZE ����ã�
        ���÷��ͣ����յ�FIFO watermark��������������������´����жϻ���DMA����
            //���Rx FIFO buffer�е�DataWords ���ڵ��� UART_FIFOWaterMark
            //�������жϻ���DMA�źţ�ȡ��C2_RIE��C5_RDMAS��״̬
            //���Tx FIFO buffer�е�DataWords С�ڵ��� UART_FIFOWaterMark
            //�������жϻ���DMA�źţ�ȡ��C2_TIE��C5_TDMAS��״̬
        ����ʱPC���͵����ݻ����UART_FIFOWaterMark��������������жϣ�
        ���ж��н����������ٷ��͵�PC
   
   3 \#define USE_DMA_Tx_Rx  (USE)
      #define USE_FIFO_Tx_Rx (USE)
        ��ʱ���÷���,����ΪFIFO��ʽ,
        Rx FIFO �������watermark�Ĺ�ϵ����DMA���󣬽���PC���յ����ݴ洢��RAMbuffer�С�
        ��DMA����������󣬲���DMA����жϣ��ڸ��ж���ʹ�� DMA ch1.
        Tx FIFO �������watermark�Ĺ�ϵ����DMA����ͨ��DMA ch1��RAMbuffer�е����ݷ��͸�PC
   
   4 \#define USE_DMA_Tx_Rx  (USE)
      #define USE_FIFO_Tx_Rx (UN_USE)
        �����յ����ݺ�ᴥ��DMA���󣬽���PC���յ����ݴ洢��RAMbuffer�С�
        ��DMA����������󣬲���DMA����жϣ��ڸ��ж���ʹ�� DMA ch1.
        UART�ᴥ��DMA����ͨ��DMA ch1��RAMbuffer�е����ݷ��͸�PC��
 ****************************************/
   
//��������  
void uart_init(void);
void uart_isr(void);
void dma_isr(void);
//��������
UART_InitTypeDef uart0_init_struct;
#if USE_DMA_Tx_Rx == USE
DMA_InitTypeDef uart_rev_dma_init_struct;
DMA_InitTypeDef uart_trn_dma_init_struct;
uint8 recv_ram_buf[8];
#endif
void main (void)
{
  uart_init();
 
  while(1)
  {

  } 
}

/*
 * ��ʼ��UART5ģ��
 *
 */
void uart_init(void)
{
  uart0_init_struct.UART_Uartx = UART0;  //ʹ��UART0
  uart0_init_struct.UART_BaudRate = 9600; //���ò�����9600
  uart0_init_struct.UART_RxPin = PTA15;  //��������ΪPTE9
  uart0_init_struct.UART_TxPin = PTA14;  //��������ΪPTE8

#if USE_FIFO_Tx_Rx == USE
  uart0_init_struct.RxFIFO.FIFO_Enable = TRUE;
  uart0_init_struct.TxFIFO.FIFO_Enable = TRUE;
  uart0_init_struct.RxFIFO.FIFO_WaterMark = 1;
  uart0_init_struct.RxFIFO.FIFO_BufFlow_IntEnable = TRUE;
  uart0_init_struct.TxFIFO.FIFO_WaterMark = 1;
  uart0_init_struct.TxFIFO.FIFO_BufFlow_IntEnable = TRUE;
#endif
  
#if USE_DMA_Tx_Rx == USE
  uart0_init_struct.UART_RxDMAEnable = TRUE;    //ʹ�ܽ���DMA
  uart0_init_struct.UART_TxDMAEnable = TRUE;    //ʹ�ܷ���DMA
  //���ý���DMA
  uart_rev_dma_init_struct.DMA_CHx = DMA_CH0;                     //ѡ��DMA CH0��Ϊͨ��
  uart_rev_dma_init_struct.DMA_MajorLoopCnt   = sizeof(recv_ram_buf); //���ü���������Ϊ���鳤��
  uart_rev_dma_init_struct.DMA_MinorByteCnt   = 1;                //����ÿ�εĳ���Ϊbyte
  uart_rev_dma_init_struct.DMA_MajorCompleteIntEnable = TRUE;     //����DMA �����������ж�
  uart_rev_dma_init_struct.DMA_Req            = UART0_REV_DMAREQ; //����DMA ����Ϊ UART rev
  uart_rev_dma_init_struct.DMA_DestAddr       = (uint32)recv_ram_buf; //����Ŀ�ĵ�ַΪ recv_ram_buf
  uart_rev_dma_init_struct.DMA_DestAddrOffset = 1;                //ÿһ��DMA���󣬵�ַ��1
  uart_rev_dma_init_struct.DMA_DestDataSize   = DMA_DST_8BIT;     //����Ŀ�ĳ���Ϊbyte
  uart_rev_dma_init_struct.DMA_SourceAddr     = (uint32)&UART0->D;//����UART DΪԴ��ַ
  uart_rev_dma_init_struct.DMA_SourceDataSize = DMA_SRC_8BIT;     //����Դ����Ϊbyte
  uart_rev_dma_init_struct.DMA_AutoDisableReq = FALSE;            //ѭ������
  uart_rev_dma_init_struct.DMA_Isr = dma_isr;
  //���÷���DMA
  uart_trn_dma_init_struct.DMA_CHx = DMA_CH1;                     //ѡ��DMA CH0��Ϊͨ��
  uart_trn_dma_init_struct.DMA_MajorLoopCnt   = sizeof(recv_ram_buf); //���ü���������Ϊ���鳤��
  uart_trn_dma_init_struct.DMA_MinorByteCnt   = 1;                //����ÿ�εĳ���Ϊbyte
  uart_trn_dma_init_struct.DMA_MajorCompleteIntEnable = FALSE;    //���DMA �����������ж�
  uart_trn_dma_init_struct.DMA_Req            = UART0_TRAN_DMAREQ;//����DMA ����Ϊ UART trn
  uart_trn_dma_init_struct.DMA_SourceAddr     = (uint32)recv_ram_buf; //����Ŀ�ĵ�ַΪ recv_ram_buf
  uart_trn_dma_init_struct.DMA_SourceAddrOffset = 1;               //ÿһ��DMA���󣬵�ַ��1
  uart_trn_dma_init_struct.DMA_SourceDataSize = DMA_SRC_8BIT;     //����Դ����Ϊbyte
  uart_trn_dma_init_struct.DMA_DestAddr       = (uint32)&UART0->D;//����UART DΪԴ��ַ
  uart_trn_dma_init_struct.DMA_DestDataSize   = DMA_DST_8BIT;     //����Ŀ�ĳ���Ϊbyte
  uart_trn_dma_init_struct.DMA_AutoDisableReq = TRUE;             //��ֹѭ������
  
  LPLD_DMA_Init(uart_rev_dma_init_struct); //��ʼ��DMA ����
  LPLD_DMA_Init(uart_trn_dma_init_struct); //��ʼ��DMA ����
  LPLD_DMA_EnableReq(DMA_CH0);         //ʹ��DMA����

  LPLD_DMA_EnableIrq(uart_rev_dma_init_struct);   //ʹ��DMA CH0�ж�
#endif
  
  uart0_init_struct.UART_RxIntEnable = TRUE;    //ʹ�ܽ����ж�
  uart0_init_struct.UART_TxIntEnable = TRUE;    //ʹ�ܷ����ж�
  uart0_init_struct.UART_RxIsr = uart_isr;      //���ý����жϺ���
  LPLD_UART_Init(uart0_init_struct);            //��ʼ��UART
  
#if USE_DMA_Tx_Rx != USE
  //ʹ��UART�ж�
  LPLD_UART_EnableIrq(uart0_init_struct);
#endif
}

/*
 * UART0�����жϺ���
 *
 */
void uart_isr(void)
{
  int8 recv;
  recv = LPLD_UART_GetChar(UART0);
  LPLD_UART_PutChar(UART0, recv);
}

/*
 * DAM CH0�жϺ���
 *
 */
void dma_isr(void)
{
  LPLD_DMA_EnableReq(DMA_CH1);         //ʹ��DMA����
}
