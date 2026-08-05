#include "int_SI24R1.h"

// ����һ����̬���͵�ַ�����͵�ַ����յ�ַ��ͬ����һλ��ַ0x0A��Ҫ�Ҹģ�
uint8_t TX_ADDRESS[TX_ADR_WIDTH] = {0x0A, 0x01, 0x07, 0x0E, 0x01};

// SPI��дһ���ֽڡ�����ΪҪд����ֽڣ�����ֵΪ��ȡ�����������ֽڣ�
static uint8_t SPI_RW(uint8_t byte)
{
	uint8_t rx_data;
	HAL_SPI_TransmitReceive(&hspi1, &byte, &rx_data, 1, 1000);
	return rx_data;
}
/****************************************************
�������ܣ����RX FIFO����
��ڲ�������
����ֵ����
****************************************************/
void Int_SI24R1_FlushRX(void)
{
    CS_LOW();
    SPI_RW(FLUSH_RX);  // �����͵��ֽ�ָ�������д����
    CS_HIGH();
}

/****************************************************
�������ܣ����TX FIFO����
��ڲ�������
����ֵ����
****************************************************/
void Int_SI24R1_FlushTX(void)
{
    CS_LOW();
    SPI_RW(FLUSH_TX);  // �����͵��ֽ�ָ�������д����
    CS_HIGH();
}

/********************************************************
�������ܣ�SI24R1���ų�ʼ��
��ڲ�������
����  ֵ����
*********************************************************/

/********************************************************
�������ܣ�д�Ĵ�����ֵ�����ֽڣ�
��ڲ�����reg:�Ĵ���ӳ���ַ����ʽ��SI24R1_WRITE_REG��reg��
					value:�Ĵ�����ֵ
����  ֵ��״̬�Ĵ�����ֵ
*********************************************************/
uint8_t Int_SI24R1_Write_Reg(uint8_t reg, uint8_t value)
{
	uint8_t status;

	CS_LOW();
	status = SPI_RW(reg);
	SPI_RW(value);
	CS_HIGH();

	return (status);
}

/********************************************************
�������ܣ�д�Ĵ�����ֵ�����ֽڣ�
��ڲ�����reg:�Ĵ���ӳ���ַ����ʽ��SI24R1_WRITE_REG��reg��
					pBuf:д�����׵�ַ
					size:д�����ֽ���
����ֵ�� ״̬�Ĵ�����ֵ
*********************************************************/
uint8_t Int_SI24R1_Write_Buf(uint8_t reg, const uint8_t *pBuf, uint8_t size)
{
	uint8_t status, byte_ctr;

	CS_LOW();
	status = SPI_RW(reg);
	for (byte_ctr = 0; byte_ctr < size; byte_ctr++)
	{
		SPI_RW(*pBuf++);
	}
	CS_HIGH();
	return (status);
}

/********************************************************
�������ܣ���ȡ�Ĵ�����ֵ�����ֽڣ�
��ڲ�����reg:�Ĵ���ӳ���ַ����ʽ��SI24R1_READ_REG��reg��
����  ֵ���Ĵ���ֵ
*********************************************************/
uint8_t Int_SI24R1_Read_Reg(uint8_t reg)
{
	uint8_t value;

	CS_LOW();
	SPI_RW(reg);
	value = SPI_RW(0);
	CS_HIGH();

	return (value);
}

/********************************************************
�������ܣ���ȡ�Ĵ�����ֵ�����ֽڣ�
��ڲ�����reg:�Ĵ���ӳ���ַ��SI24R1_READ_REG��reg��
					pBuf:���ջ��������׵�ַ
					size:��ȡ�ֽ���
����  ֵ��״̬�Ĵ�����ֵ
*********************************************************/
uint8_t Int_SI24R1_Read_Buf(uint8_t reg, uint8_t *pBuf, uint8_t size)
{
	uint8_t status, byte_ctr;

	CS_LOW();
	status = SPI_RW(reg);
	for (byte_ctr = 0; byte_ctr < size; byte_ctr++)
	{
		pBuf[byte_ctr] = SPI_RW(0);
	}
	CS_HIGH();

	return (status);
}

/********************************************************
�������ܣ�SI24R1����ģʽ��ʼ��
��ڲ�������
����  ֵ����
*********************************************************/
void Int_SI24R1_RX_Mode(void)
{
	CE_LOW();
	/* �������ú����Ĳ������� ��ַ + ����ֵ����ַ�����Ĵ����ĵ�ַ������ֵ����Ҫ���øüĴ�����ֵ����ַ��ֵ�ĺ�����ο�SI24R1�����ֲ� */
	Int_SI24R1_Write_Buf(SI24R1_WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH); // �����豸����ͨ��0ʹ�úͷ����豸��ͬ�ķ��͵�ַ
	Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + EN_AA, 0x01);						   // ʹ�ܽ���ͨ��0�Զ�Ӧ��
	Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + EN_RXADDR, 0x01);					   // ʹ�ܽ���ͨ��0
	Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + RF_CH, 40);							   // ѡ����Ƶͨ��40
	Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + RX_PW_P0, TX_PLOAD_WIDTH);			   // ����ͨ��0ѡ��ͷ���ͨ����ͬ��Ч���ݿ���
	Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + RF_SETUP, 0x06);					   // ���ݴ�����1Mbps�����书��4dBm
	Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + CONFIG, 0x0f);						   // CRCʹ�ܣ�16λCRCУ�飬�ϵ磬����ģʽ
	Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + STATUS, 0xff);						   // ������е��жϱ�־λ

	//���FIFO���У���ֹ��������ʱ��FIFO�������в������ݵ��½���ʧ�ܣ�
	Int_SI24R1_FlushTX();
    Int_SI24R1_FlushRX();
    Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + 0x07, 0xFF);
	
	CE_HIGH();																	   // ����CE���������豸
}

/********************************************************
�������ܣ�SI24R1����ģʽ��ʼ��
��ڲ�������
����  ֵ����
*********************************************************/
void Int_SI24R1_TX_Mode(void)
{
	CE_LOW();
	Int_SI24R1_Write_Buf(SI24R1_WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);	   // д�뷢�͵�ַ
	Int_SI24R1_Write_Buf(SI24R1_WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH); // Ϊ��Ӧ������豸������ͨ��0��ַ�ͷ��͵�ַ��ͬ
	Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + EN_AA, 0x01);						   // ʹ�ܽ���ͨ��0�Զ�Ӧ��
	Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + EN_RXADDR, 0x01);					   // ʹ�ܽ���ͨ��0
	Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + SETUP_RETR, 0x0a);					   // �Զ��ط���ʱ�ȴ�250us+86us���Զ��ط�10��
	Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + RF_CH, 40);							   // ѡ����Ƶͨ��0x40
	Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + RF_SETUP, 0x06);					   // ���ݴ�����1Mbps�����书��4dBm
	Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + CONFIG, 0x0e);						   // CRCʹ�ܣ�16λCRCУ�飬�ϵ�

	//���FIFO���У���ֹ��������ʱ��FIFO�������в������ݵ��·���ʧ�ܣ�
	Int_SI24R1_FlushTX();
    Int_SI24R1_FlushRX();
    Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + 0x07, 0xFF);

	CE_HIGH();																	   // ����CE���������豸
}

/********************************************************
�������ܣ���ȡ�������ݡ�Ӳ��ֱ�ӽ������ݱ��浽 FIFO�����У���ͨ����־λ�ж϶������Ƿ�������
��ڲ�����rxbuf:�������ݴ���׵�ַ
����  ֵ��0:���յ�����
		  1:û�н��յ�����
*********************************************************/
uint8_t Int_SI24R1_RxPacket(uint8_t *rxbuf)
{

	uint8_t state;
	state = Int_SI24R1_Read_Reg(STATUS); // ��ȡ״̬�Ĵ�����ֵ
	// RX_DR�������յ����ݱ�־λ��ֻҪ���յ������ݣ�Ӳ�����Զ������ݷ���FIFO���У�����RX_DR��־λ��1
	// Ӳ���涨�����֮ǰ״̬�Ĵ�����ֵΪ1����״̬�Ĵ�����ֵд��״̬�Ĵ����󣬻����RX_DR��־λ��
	// ���������RX_DR���´ε��ú����������� ���������ݡ����ظ���ȡͬһ��
	Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + STATUS, state); // ����RX_DR��־λ
	if (state & RX_DR)	// ���յ����ݡ�RX_DR�Ǻ꣬Ϊ0x40��ת��Ϊ�����ƣ���6λΪ1����01000000�������յ����ݣ�״̬�Ĵ����ĵ�6λ�ᱻ��1����state��RX_DR����������Ϊ0x40����Ϊ0���������յ�����
	{
		Int_SI24R1_Read_Buf(RD_RX_PLOAD, rxbuf, TX_PLOAD_WIDTH); // ��ȡ����
		Int_SI24R1_FlushRX();					 // ���RX FIFO�Ĵ��������յ������ݣ�
		return 0;
	}
	return 1; // û�յ��κ�����
}

//��ȡָ���޷�������ȡ����ʱ����ֱ��ʹ�ö�ȡFIFO���еķ�ʽ��ȡ���ݣ�����쳣��
// uint8_t Int_SI24R1_RxPacket(uint8_t *rxbuf)
// {
//     uint8_t status;
//     CS_LOW();
//     // ���Ͷ�RX FIFO���ͬʱ��ȡʵʱSTATUS
//     status = SPI_RW(RD_RX_PLOAD);
//     // ��ȡ17�ֽ��غ�
//     for(uint8_t i=0; i<TX_PLOAD_WIDTH; i++)
//     {
//         rxbuf[i] = SPI_RW(0);
//     }
//     CS_HIGH();
        
//     if (status & RX_DR)
//     {
//         Int_SI24R1_FlushRX();
//         // ���RX_DR��־
//         Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + STATUS, RX_DR);
//         return 0;
//     }
//     return 1;
// }

//���ȶ���д��������ʱ����һ�궨���ͻ���¶�ȡʧ�ܣ�
// uint8_t Int_SI24R1_RxPacket(uint8_t *rxbuf)
// {
//     // 1. �ȶ�״̬�Ĵ�����ȷ���������ݰ���ֱ���õ�ַ0x07����ܺ��ͻ��
//     uint8_t state = Int_SI24R1_Read_Reg(0x07);
    
//     // û�������ݣ�ֱ�ӷ��أ���������FIFO
//     if (!(state & 0x40)) // 0x40 = RX_DR��־λ
//     {
//         return 1;
//     }

//     // 2. ȷ�������ݣ��ٶ�ȡ�غ�
//     CS_LOW();
//     SPI_RW(RD_RX_PLOAD); // ���Ͷ��غ�����
//     for(uint8_t i = 0; i < TX_PLOAD_WIDTH; i++)
//     {
//         rxbuf[i] = SPI_RW(0);
//     }
//     CS_HIGH();

//     // 3. �����RX_DR�жϱ�־��д1���㣩
//     Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + 0x07, 0x40);

//     // ע�⣺�������վ�������FlushRX��Ӳ���Զ��Ƴ��Ѷ����ݰ�
//     return 0;
// }
/********************************************************
�������ܣ�����һ�����ݰ�
��ڲ�����txbuf:Ҫ���͵�����
����  ֵ��0:���ͳɹ� 1:����ʧ��
*********************************************************/
uint8_t Int_SI24R1_TxPacket(uint8_t *txbuf)
{
	uint8_t state;
	CE_LOW();												  // CE���ͣ�ʹ��SI24R1���ã����뵽����ģʽ���������������Ͷ��м� TX FIFO ��д���ݣ�
	Int_SI24R1_Write_Buf(WR_TX_PLOAD, txbuf, TX_PLOAD_WIDTH); // д���ݵ�TX FIFO,32���ֽڣ�WR_TX_PLOAD���������Ͷ�����д���ݵ����
	CE_HIGH();												  // CE�øߣ�ʹ�ܷ���
	//�˺�SI24R1���Զ��������ݣ����������õ��ط��������ط���ʱʱ�����ط����ݣ�����ֻ��ȴ����жϷ����Ƿ�ɹ���

	// while (IRQ == 1)
	// 	;										 // �ȴ��������(�жϱ�־��ʽ�жϣ�����ʹ����ѯ��ʽ�ж�)

	state = Int_SI24R1_Read_Reg(STATUS); // ��ȡ״̬�Ĵ�����ֵ
	// TX_DS����������ɱ�־λ��ֻҪ������ɣ�Ӳ�����Զ���TX_DS��־λ��1
	// MAX_RT�����ﵽ����ط�������־λ��ֻҪ�ﵽ����ط�������Ӳ�����Զ���MAX_RT��־λ��1
	while (((state & TX_DS) == 0) && ((state & MAX_RT) == 0)) // ��ѯ״̬�Ĵ�����ֱ��TX_DS��MAX_RT�жϱ�־λ����1��֤��������ɻ�ﵽ����ط�����
	{
		state = Int_SI24R1_Read_Reg(STATUS); // ����state
		vTaskDelay(1);//vtask��ʱ1ms�����⺯���������µ����ȼ������޷�ִ��
	}
	Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + STATUS, state); // ���TX_DS��MAX_RT�жϱ�־
	if (state & MAX_RT)										// �ﵽ����ط�����
	{
		Int_SI24R1_FlushTX(); // �ֶ����TX FIFO�Ĵ�������Ϊ�ﵽ����ط����������Զ����TX FIFO�Ĵ���
		return 1;//�ﵽ����ط�����������ʧ�ܣ�����1
	}
	if (state & TX_DS) // �������
	{
		// ������ɣ�Ӳ�����Զ����TX FIFO�Ĵ���
		return 0;//������ɣ�����0
	}
	return 1; // ����ʧ��
}

uint8_t si24r1_rx_buf[TX_ADR_WIDTH] = {0};//��������

/********************************************************
�������ܣ�SI24R1��ʼ�����
��ڲ�������
����  ֵ��0:��ʼ���ɹ� 1:��ʼ��ʧ��
*********************************************************/
uint8_t Iny_SI24R1_Check(void)//�����Ƿ��ʼ�����
{
	//1.����SPI�ܷ�������д�Ĵ���
	//1.0����si24ri������趨�������ȶ�ȡһ�μĴ�������������д��Ĵ���������д���ֵ�ᱻ���������������ȶ�ȡһ�μĴ���
	Int_SI24R1_Read_Buf(SI24R1_READ_REG + TX_ADDR, si24r1_rx_buf, TX_ADR_WIDTH);

	//1.1 ������д�뵽�Ĵ����������д�� ���͵�ַ(TX_ADDRESS)
	Int_SI24R1_Write_Buf(SI24R1_WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);

	//1.2 ��ȡ�Ĵ�����ֵ�����ｫֵ��ȡ������������
	Int_SI24R1_Read_Buf(SI24R1_READ_REG + TX_ADDR, si24r1_rx_buf, TX_ADR_WIDTH);

	//2.����ȡ���������Ƿ���ȷ
	for(uint8_t i = 0; i < TX_ADR_WIDTH; i++)
	{
		if(si24r1_rx_buf[i] != TX_ADDRESS[i])
		{
			return 1; //��ʼ��ʧ��
		}
	}
	return 0; //��ʼ���ɹ�
}

//Ӳ���ӿڲ�SI24R1�ĳ�ʼ������:
void Int_SI24R1_Init(void) 
{
	HAL_Delay(200);//оƬ�ϵ���ʱ��Ӧ����100ms
	while(Iny_SI24R1_Check() == 1)//�����Ƿ��ʼ�����
	{
		HAL_Delay(10);//ÿ���μ����10ms
	}
	
	//SI24R1Ĭ�Ͻ������ģʽ����Ҫ���ͣ����ֶ��л�Ϊ����ģʽ��
    Int_SI24R1_RX_Mode();
	debug_printf("SI24R1��ʼ�����\r\n");
}
