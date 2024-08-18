#include "stm32f10x.h"   // Device header
#include "TM1638.h"

unsigned char TM1638_Arr_SEG[]= {0xB9,0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07, //����������ܶ��룬����С����
                                 0x7F,0x6F,0x77,0x7C,0x39,0x76,0x79,0x71,
                                 0xBF,0x86,0xDB,0xCF,0xE6,0xED,0xFD,0x87,  //����������ܶ��룬��С����
                                 0xFF,0xEF,0xF7,0xFC,0xDE,0xF9,0xf1,0x40,
                                }; //0��F��1��0��
 
//����������ܶ��룬����С����,display������
unsigned char TM1638_Arr_SEG_Display[]= {   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
                                            0,   0,   0,   0,   0,   0,0x3F,0x06,0x5B,0x4F,//0-9
                                            0x66,0x6D,0x7D,0x07,0x7F,0x6F,   0,   0,   0,   0,
                                            0,   0,   0,0x77,0x7C,0x39,0x5E,0x79,0x71,   0
                                        };
 
uint8_t TM1638_Addr_SEG[8]= {0xC0,0xC2,0xC4,0xC6,0xC8,0xCA,0xCC,0xCE}; //ģ����ҵ���İ˸������
uint8_t TM1638_Addr_LED[8]= {0xC1,0xC3,0xC5,0xC7,0xC9,0xCB,0xCD,0xCF}; //ģ����ҵ���İ˸����������



/**
 * @brief TM1638-GPIO��ʼ������
 * @param
 * @retval 
 */
static void TM1638_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure; 
    RCC_TIM1638_CMD(RCC_TM1638,ENABLE);  
	//��3ֻ���ȫ������Ϊͬ�����������ģʽ
    GPIO_InitStructure.GPIO_Pin = TM1638_GPIO_Pin; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_Init(TM1638_GPIO, &GPIO_InitStructure); 
	STB_1();
	CLK_1();
	DIO_1();
}
void TM1638_IO_INPUT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure; 
    RCC_TIM1638_CMD(RCC_TM1638,ENABLE);  
	//��3ֻ���ȫ������Ϊͬ�����������ģʽ
    GPIO_InitStructure.GPIO_Pin = DIO; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_Init(TM1638_GPIO, &GPIO_InitStructure); 	
}
void TM1638_IO_OUTPUT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure; 
    RCC_TIM1638_CMD(RCC_TM1638,ENABLE);  
	//��3ֻ���ȫ������Ϊͬ�����������ģʽ
    GPIO_InitStructure.GPIO_Pin = DIO; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_Init(TM1638_GPIO, &GPIO_InitStructure); 
	DIO_1();	
}
/**
 * @brief TM1638��ʼ������
 * @param
 * @retval 
 */
void TM1638_Init(void) 
{
    TM1638_GPIO_Config();
    TM1638_Clear();
}
 
/**
 * @brief ��TM1638д������
 * @param byte-д�������
 * @retval 
 */
void TM1638_Write_Byte(uint8_t byte) 
{
    uint8_t Count;
    for(Count = 0; Count < 8; Count++)
    {
        CLK_0();         //��ʱ�ӵ�������д������
        if(byte & 0x01)
        {
            DIO_1(); 
        }
        else
        {
            DIO_0(); 
        }
        byte >>= 1;        //8λȫ����ȡ
        CLK_1();  		 //��ʱ�ӵ�������д������
    }
}
 
/**
 * @brief ��TM1638д������(��������)
 * @param byte-д�������
 * @retval 
 */
void TM1638_Write_Data(uint8_t data) //TM1638д���ݺ���
{
    STB_0();                         //Ƭѡ,Ϊ��ʱ��ʼ������Ϣ
    TM1638_Write_Byte(data);
    STB_1();
}
 
void (*TM1638_Write_Cmd)(u8)=TM1638_Write_Data;//��������һ������������ָ��
 
/**
 * @brief д���ַ��������
 * @param 
		addr-��Ӧ��ַ
		data-д������
 * @retval 
 */
void TM1638_Write_Addr_Byte(uint8_t addr,uint8_t data)
{
    STB_0();
    TM1638_Write_Byte(addr);        //STB Ϊ�ͺ�ĵ�һ���ֽ���Ϊָ��
    TM1638_Write_Byte(data);
    STB_1();
}
 
/**
 * @brief �������������ʾ����
 * @param 
		data-д������
 * @retval 
 */
void TM1638_Display_Num(uint32_t data)
{
    unsigned int dataL=0,dataR=0;
    dataL=data/10000;
    dataR=data%10000;
    TM1638_Write_Cmd(0x44);//�̶���ַ��д����
    TM1638_Write_Cmd(0x88);//��ʾ��������1
 
    TM1638_Write_Addr_Byte(TM1638_Addr_SEG[0],TM1638_Arr_SEG[dataL/1000]);
    TM1638_Write_Addr_Byte(TM1638_Addr_SEG[1],TM1638_Arr_SEG[dataL%1000/100]);
    TM1638_Write_Addr_Byte(TM1638_Addr_SEG[2],TM1638_Arr_SEG[dataL%100/10]);
    TM1638_Write_Addr_Byte(TM1638_Addr_SEG[3],TM1638_Arr_SEG[dataL%10]);
    TM1638_Write_Addr_Byte(TM1638_Addr_SEG[4],TM1638_Arr_SEG[dataR/1000]);
    TM1638_Write_Addr_Byte(TM1638_Addr_SEG[5],TM1638_Arr_SEG[dataR%1000/100]);
    TM1638_Write_Addr_Byte(TM1638_Addr_SEG[6],TM1638_Arr_SEG[dataR%100/10]);
    TM1638_Write_Addr_Byte(TM1638_Addr_SEG[7],TM1638_Arr_SEG[dataR%10]);
}
 
/**
 * @brief �������������ʾ�¶�
 * @param 
		data-д������
 * @retval 
 */
void TM1638_Display_Tmp(uint32_t data)
{
    TM1638_Write_Cmd(0x44);//�̶���ַ��д����
    TM1638_Write_Cmd(0x88);//��ʾ��������1
 
    TM1638_Write_Addr_Byte(TM1638_Addr_SEG[0],TM1638_Arr_SEG[data/10]);
    TM1638_Write_Addr_Byte(TM1638_Addr_SEG[1],TM1638_Arr_SEG[data%10]);
}
 
/**
 * @brief �������������ʾʪ��
 * @param 
		data-д������
 * @retval 
 */
void TM1638_Display_Hum(uint32_t data)
{
    TM1638_Write_Cmd(0x44);//�̶���ַ��д����
    TM1638_Write_Cmd(0x88);//��ʾ��������1
 
    TM1638_Write_Addr_Byte(TM1638_Addr_SEG[4],TM1638_Arr_SEG[data/10]);
    TM1638_Write_Addr_Byte(TM1638_Addr_SEG[5],TM1638_Arr_SEG[data%10]);
}
 
 
/**
 * @brief TM1638ָ����ַ��ʾ���룬0-F
 * @param 
		num-�����λ��12345678
		seg-�ַ�0-F
		p-N/Y
 * @retval 
 */
void TM1638_Display_SEG(unsigned int num,unsigned char seg,PointState p)
{
    TM1638_Write_Cmd(0x44);//�̶���ַ��д����
    TM1638_Write_Cmd(0x88);//��ʾ��������1
 
    TM1638_Write_Addr_Byte(TM1638_Addr_SEG[num-1],TM1638_Arr_SEG_Display[seg-32]|p);
}
 
/**
 * @brief TM1638ָ��LED����
 * @param 
		num-LEDλ��
		light-OFF/ON
 * @retval 
 */
void TM1638_Display_LED(unsigned int num,LightState light)
{
    TM1638_Write_Cmd(0x44);//�̶���ַ��д����
    TM1638_Write_Cmd(0x88);//��ʾ��������1
 
    TM1638_Write_Addr_Byte(TM1638_Addr_LED[num],light);
}
 
 
/**
 * @brief TM1638�ر�ָ������ܺ���
 * @param serial-��������1-8
 * @retval 
 */
void TM1638_SEG_Off(unsigned char num)  //TM1638�ر�ָ������ܺ���
{
    TM1638_Write_Cmd(0x44);//�̶���ַ��д����
    TM1638_Write_Cmd(0x88);//��ʾ��������1
 
    TM1638_Write_Addr_Byte(TM1638_Addr_SEG[num-1],0x80);
}
 
/**
 * @brief TM1638�����ݺ���
 * @param
 * @retval ��ȡ��8λ����
 */
unsigned char TM1638_Read(void) //�����ݺ���
{
    uint8_t i;
	uint8_t data,temp=0;
 
    TM1638_IO_INPUT();
    for(i=0; i<8; i++)
    {
        temp>>=1;
        CLK_0(); //CLK=0
        data = DIO_Read();  //��ȡDIOֵ
        if(data)
		{
			temp|=0x80; //��λ����0�򲻱䡢��1����1
        }
		CLK_1(); //CLK=1
    }
	TM1638_IO_OUTPUT();
    return temp;
}
 
/**
 * @brief TM1638����ɨ���ݺ���
 * @param
 * @retval ��ȡ�İ����ţ�1~8
 */
unsigned char TM1638_ReadKey(void)  //TM1638����ɨ���ݺ���
{
    unsigned char c[4],i,key_value=0;
 
    STB_0();  //STB=0,��ʼд����
    TM1638_Write_Byte(0x42);  //��ͨģʽ����ַ�Զ����ӣ�����ɨ����
    for(i=0; i<4; i++)	
	{
		c[i]=TM1638_Read(); //��ȡ��ֵ
    }
	STB_1();  //STB=1,����ֵ����
 
    //���ݴ���
    for(i=0; i<4; i++)
	{
        key_value|=(c[i]<<i);
    }
	for(i=0; i<8; i++)
	{
        if((0x01<<i)==key_value) break;
    }
	return (i+1);//���ذ���ֵ����ģ���ϵ����Ӧ
}
 
 
 
/**
 * @brief TM1638ȫ��
 * @param 
 * @retval 
 */
void TM1638_Clear(void)
{
    uint8_t i;
    TM1638_Write_Data(0x44); //��ͨģʽ���̶���ַ��д���ݵ���ʾ�Ĵ���
    TM1638_Write_Data(0x88); //��ʾ�������ȵ�1��
    for(i=0; i<16; i++)
    {
        TM1638_Write_Addr_Byte(0XC0+i,0X00); //ȫ��ַд��0X00
    }
}


