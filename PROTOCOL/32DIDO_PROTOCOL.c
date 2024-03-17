#include "32DIDO_PROTOCOL.h"


volatile struct NET_DigitalTx Net_DigitalDataIn;//板卡发送给上位机的数据
volatile union DigitalIn DigitalInputNew,DigitalInputOld,DigitalDataInputStates; //数字量采集联合体

volatile union DigitalOut DigitalOutNew,DigitalOutOld={0x00,};//数字量输出联合体
volatile struct NET_DigitalRx Net_DigitalDataOut;//上位机给板卡发送数据 



//这个函数实现将共用体与数字量输入对应上，按键被按下，引脚为低电平，按键没有按下，引脚为高电平
void INPUTDATA_INIT(void)
{
	
	DigitalInputNew.Digitalin_Bit.BIT1  = DI1;
	DigitalInputNew.Digitalin_Bit.BIT2  = DI2;
	DigitalInputNew.Digitalin_Bit.BIT3  = DI3;
	DigitalInputNew.Digitalin_Bit.BIT4  = DI4;
	DigitalInputNew.Digitalin_Bit.BIT5  = DI5;
	DigitalInputNew.Digitalin_Bit.BIT6  = DI6;
	DigitalInputNew.Digitalin_Bit.BIT7  = DI7;
	DigitalInputNew.Digitalin_Bit.BIT8  = DI8;
	DigitalInputNew.Digitalin_Bit.BIT9  = DI9;
	DigitalInputNew.Digitalin_Bit.BIT10 = DI10;
	DigitalInputNew.Digitalin_Bit.BIT11 = DI11;
	DigitalInputNew.Digitalin_Bit.BIT12 = DI12;
	DigitalInputNew.Digitalin_Bit.BIT13 = DI13;
	DigitalInputNew.Digitalin_Bit.BIT14 = DI14;
	DigitalInputNew.Digitalin_Bit.BIT15 = DI15;
	DigitalInputNew.Digitalin_Bit.BIT16 = DI16;
	DigitalInputNew.Digitalin_Bit.BIT17 = DI17;
	DigitalInputNew.Digitalin_Bit.BIT18 = DI18;
	DigitalInputNew.Digitalin_Bit.BIT19 = DI19;
	DigitalInputNew.Digitalin_Bit.BIT20 = DI20;
	DigitalInputNew.Digitalin_Bit.BIT21 = DI21;
	DigitalInputNew.Digitalin_Bit.BIT22 = DI22;
	DigitalInputNew.Digitalin_Bit.BIT23 = DI23;
	DigitalInputNew.Digitalin_Bit.BIT24 = DI24;
	DigitalInputNew.Digitalin_Bit.BIT25 = DI25;
	DigitalInputNew.Digitalin_Bit.BIT26 = DI26;
	DigitalInputNew.Digitalin_Bit.BIT27 = DI27;
	DigitalInputNew.Digitalin_Bit.BIT28 = DI28;
	DigitalInputNew.Digitalin_Bit.BIT29 = DI29;
	DigitalInputNew.Digitalin_Bit.BIT30 = DI30;
	DigitalInputNew.Digitalin_Bit.BIT31 = DI31;
	DigitalInputNew.Digitalin_Bit.BIT32 = DI32;
	
  DigitalInputOld.all = DigitalInputNew.all;
		
}

/*************************************************************************************************************/
/**                                 数字量输出处理函数                                                 ***/
/*************************************************************************************************************/
void OUTPUTDATA_INIT(void)
{
//	DigitalOutOld.all = DigitalOutNew.all;  //接收到的新值赋值给旧值
	//数字量输出，DO(x)，如果x为1 ，则有输出，如果x为0 则没有输出
	DO1(DigitalOutOld.digitaloutbit.BIT1);
	DO2(DigitalOutOld.digitaloutbit.BIT2);
	DO3(DigitalOutOld.digitaloutbit.BIT3);
	DO4(DigitalOutOld.digitaloutbit.BIT4);
	DO5(DigitalOutOld.digitaloutbit.BIT5);
	DO6(DigitalOutOld.digitaloutbit.BIT6);
	DO7(DigitalOutOld.digitaloutbit.BIT7);
	DO8(DigitalOutOld.digitaloutbit.BIT8);
	DO9(DigitalOutOld.digitaloutbit.BIT9);
	DO10(DigitalOutOld.digitaloutbit.BIT10);
	DO11(DigitalOutOld.digitaloutbit.BIT11);
	DO12(DigitalOutOld.digitaloutbit.BIT12);
	DO13(DigitalOutOld.digitaloutbit.BIT13);
	DO14(DigitalOutOld.digitaloutbit.BIT14);
	DO15(DigitalOutOld.digitaloutbit.BIT15);
	DO16(DigitalOutOld.digitaloutbit.BIT16);
	DO17(DigitalOutOld.digitaloutbit.BIT17);
	DO18(DigitalOutOld.digitaloutbit.BIT18);
	DO19(DigitalOutOld.digitaloutbit.BIT19);
	DO20(DigitalOutOld.digitaloutbit.BIT20);
	DO21(DigitalOutOld.digitaloutbit.BIT21);
	DO22(DigitalOutOld.digitaloutbit.BIT22);
	DO23(DigitalOutOld.digitaloutbit.BIT23);
	DO24(DigitalOutOld.digitaloutbit.BIT24);
	DO25(DigitalOutOld.digitaloutbit.BIT25);
	DO26(DigitalOutOld.digitaloutbit.BIT26);
	DO27(DigitalOutOld.digitaloutbit.BIT27);
	DO28(DigitalOutOld.digitaloutbit.BIT28);
	DO29(DigitalOutOld.digitaloutbit.BIT29);
	DO30(DigitalOutOld.digitaloutbit.BIT30);
	DO31(DigitalOutOld.digitaloutbit.BIT31);
	DO32(DigitalOutOld.digitaloutbit.BIT32);
	
	
	DigitalOutOld.all = Net_DigitalDataOut.Digital_out.all;
}

