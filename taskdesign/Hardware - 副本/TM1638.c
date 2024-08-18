#include "stm32f10x.h"   // Device header
#include "TM1638.h"

unsigned char TM1638_Arr_SEG[]= {0xB9,0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07, //共阴极数码管段码，不带小数点
                                 0x7F,0x6F,0x77,0x7C,0x39,0x76,0x79,0x71,
                                 0xBF,0x86,0xDB,0xCF,0xE6,0xED,0xFD,0x87,  //共阴极数码管段码，带小数点
                                 0xFF,0xEF,0xF7,0xFC,0xDE,0xF9,0xf1,0x40,
                                }; //0～F，1亮0灭
 
//共阴极数码管段码，不带小数点,display函数用
unsigned char TM1638_Arr_SEG_Display[]= {   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
                                            0,   0,   0,   0,   0,   0,0x3F,0x06,0x5B,0x4F,//0-9
                                            0x66,0x6D,0x7D,0x07,0x7F,0x6F,   0,   0,   0,   0,
                                            0,   0,   0,0x77,0x7C,0x39,0x5E,0x79,0x71,   0
                                        };
 
uint8_t TM1638_Addr_SEG[8]= {0xC0,0xC2,0xC4,0xC6,0xC8,0xCA,0xCC,0xCE}; //模块从右到左的八个数码管
uint8_t TM1638_Addr_LED[8]= {0xC1,0xC3,0xC5,0xC7,0xC9,0xCB,0xCD,0xCF}; //模块从右到左的八个发光二极管



/**
 * @brief TM1638-GPIO初始化函数
 * @param
 * @retval 
 */
static void TM1638_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure; 
    RCC_TIM1638_CMD(RCC_TM1638,ENABLE);  
	//将3只因脚全部设置为同样的推挽输出模式
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
	//将3只因脚全部设置为同样的推挽输出模式
    GPIO_InitStructure.GPIO_Pin = DIO; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_Init(TM1638_GPIO, &GPIO_InitStructure); 	
}
void TM1638_IO_OUTPUT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure; 
    RCC_TIM1638_CMD(RCC_TM1638,ENABLE);  
	//将3只因脚全部设置为同样的推挽输出模式
    GPIO_InitStructure.GPIO_Pin = DIO; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_Init(TM1638_GPIO, &GPIO_InitStructure); 
	DIO_1();	
}
/**
 * @brief TM1638初始化函数
 * @param
 * @retval 
 */
void TM1638_Init(void) 
{
    TM1638_GPIO_Config();
    TM1638_Clear();
}
 
/**
 * @brief 给TM1638写入数据
 * @param byte-写入的数据
 * @retval 
 */
void TM1638_Write_Byte(uint8_t byte) 
{
    uint8_t Count;
    for(Count = 0; Count < 8; Count++)
    {
        CLK_0();         //在时钟的上升沿写入数据
        if(byte & 0x01)
        {
            DIO_1(); 
        }
        else
        {
            DIO_0(); 
        }
        byte >>= 1;        //8位全部读取
        CLK_1();  		 //在时钟的上升沿写入数据
    }
}
 
/**
 * @brief 给TM1638写入数据(或者命令)
 * @param byte-写入的数据
 * @retval 
 */
void TM1638_Write_Data(uint8_t data) //TM1638写数据函数
{
    STB_0();                         //片选,为低时开始处理信息
    TM1638_Write_Byte(data);
    STB_1();
}
 
void (*TM1638_Write_Cmd)(u8)=TM1638_Write_Data;//给函数起一个别名，函数指针
 
/**
 * @brief 写入地址加上数据
 * @param 
		addr-对应地址
		data-写入数据
 * @retval 
 */
void TM1638_Write_Addr_Byte(uint8_t addr,uint8_t data)
{
    STB_0();
    TM1638_Write_Byte(addr);        //STB 为低后的第一个字节作为指令
    TM1638_Write_Byte(data);
    STB_1();
}
 
/**
 * @brief 在数码管上面显示数据
 * @param 
		data-写入数据
 * @retval 
 */
void TM1638_Display_Num(uint32_t data)
{
    unsigned int dataL=0,dataR=0;
    dataL=data/10000;
    dataR=data%10000;
    TM1638_Write_Cmd(0x44);//固定地址，写数据
    TM1638_Write_Cmd(0x88);//显示开，亮度1
 
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
 * @brief 在数码管上面显示温度
 * @param 
		data-写入数据
 * @retval 
 */
void TM1638_Display_Tmp(uint32_t data)
{
    TM1638_Write_Cmd(0x44);//固定地址，写数据
    TM1638_Write_Cmd(0x88);//显示开，亮度1
 
    TM1638_Write_Addr_Byte(TM1638_Addr_SEG[0],TM1638_Arr_SEG[data/10]);
    TM1638_Write_Addr_Byte(TM1638_Addr_SEG[1],TM1638_Arr_SEG[data%10]);
}
 
/**
 * @brief 在数码管上面显示湿度
 * @param 
		data-写入数据
 * @retval 
 */
void TM1638_Display_Hum(uint32_t data)
{
    TM1638_Write_Cmd(0x44);//固定地址，写数据
    TM1638_Write_Cmd(0x88);//显示开，亮度1
 
    TM1638_Write_Addr_Byte(TM1638_Addr_SEG[4],TM1638_Arr_SEG[data/10]);
    TM1638_Write_Addr_Byte(TM1638_Addr_SEG[5],TM1638_Arr_SEG[data%10]);
}
 
 
/**
 * @brief TM1638指定地址显示数码，0-F
 * @param 
		num-数码管位置12345678
		seg-字符0-F
		p-N/Y
 * @retval 
 */
void TM1638_Display_SEG(unsigned int num,unsigned char seg,PointState p)
{
    TM1638_Write_Cmd(0x44);//固定地址，写数据
    TM1638_Write_Cmd(0x88);//显示开，亮度1
 
    TM1638_Write_Addr_Byte(TM1638_Addr_SEG[num-1],TM1638_Arr_SEG_Display[seg-32]|p);
}
 
/**
 * @brief TM1638指定LED亮灭
 * @param 
		num-LED位置
		light-OFF/ON
 * @retval 
 */
void TM1638_Display_LED(unsigned int num,LightState light)
{
    TM1638_Write_Cmd(0x44);//固定地址，写数据
    TM1638_Write_Cmd(0x88);//显示开，亮度1
 
    TM1638_Write_Addr_Byte(TM1638_Addr_LED[num],light);
}
 
 
/**
 * @brief TM1638关闭指定数码管函数
 * @param serial-数码管序号1-8
 * @retval 
 */
void TM1638_SEG_Off(unsigned char num)  //TM1638关闭指定数码管函数
{
    TM1638_Write_Cmd(0x44);//固定地址，写数据
    TM1638_Write_Cmd(0x88);//显示开，亮度1
 
    TM1638_Write_Addr_Byte(TM1638_Addr_SEG[num-1],0x80);
}
 
/**
 * @brief TM1638读数据函数
 * @param
 * @retval 读取的8位数据
 */
unsigned char TM1638_Read(void) //读数据函数
{
    uint8_t i;
	uint8_t data,temp=0;
 
    TM1638_IO_INPUT();
    for(i=0; i<8; i++)
    {
        temp>>=1;
        CLK_0(); //CLK=0
        data = DIO_Read();  //读取DIO值
        if(data)
		{
			temp|=0x80; //按位或：与0或不变、与1或置1
        }
		CLK_1(); //CLK=1
    }
	TM1638_IO_OUTPUT();
    return temp;
}
 
/**
 * @brief TM1638读键扫数据函数
 * @param
 * @retval 读取的按键号，1~8
 */
unsigned char TM1638_ReadKey(void)  //TM1638读键扫数据函数
{
    unsigned char c[4],i,key_value=0;
 
    STB_0();  //STB=0,开始写命令
    TM1638_Write_Byte(0x42);  //普通模式，地址自动增加，读键扫数据
    for(i=0; i<4; i++)	
	{
		c[i]=TM1638_Read(); //读取键值
    }
	STB_1();  //STB=1,读键值结束
 
    //数据处理
    for(i=0; i<4; i++)
	{
        key_value|=(c[i]<<i);
    }
	for(i=0; i<8; i++)
	{
        if((0x01<<i)==key_value) break;
    }
	return (i+1);//返回按键值，与模块上的相对应
}
 
 
 
/**
 * @brief TM1638全清
 * @param 
 * @retval 
 */
void TM1638_Clear(void)
{
    uint8_t i;
    TM1638_Write_Data(0x44); //普通模式，固定地址，写数据到显示寄存器
    TM1638_Write_Data(0x88); //显示开，亮度第1级
    for(i=0; i<16; i++)
    {
        TM1638_Write_Addr_Byte(0XC0+i,0X00); //全地址写入0X00
    }
}


