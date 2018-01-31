#include <ioCC2530.h>
#include "sys_init.h"
#include "uart.h"
#include"sysinit.h"

#define uchar unsigned char
#define uint unsigned  int

void TaskRermark();
void delay(uint);
void sys_init();
void TaskProcess();
void TaskLED1();
void TaskLED2();


uchar n = 0;


/*  ����ṹ��  */
typedef struct _TAST_COMPONENTS
{
	uchar Run;      //�������б�־
	uchar Timer;  	//��ʱ��
	uchar ItvTime;  //����������ʹ�õ�ʱ��Ƭ
	void(* TaskHook)(void);   //����ָ��,��Ҫ���е�������
}TASK_COMPONENTS;

/*   ����ṹ������(** ���Ҫ�ӹ���,�����������)   */
static TASK_COMPONENTS Taskcomps[] = 
{
	{0,2,2,TaskLED1},    //����Ҫ������ṹ�����
	{0,3,3,TaskLED2},
};

uint TASK_MAX = sizeof(Taskcomps)/sizeof(Taskcomps[0]);	     //sizeof�������ڱ���ʱ���������,�õ�����ĵ�ַ����,���Ե����͵õ�������

void  sys_init()
{
	init_timer1();
        led_init();
        xtal_init();                          //�ⲿ32MHZ����
        uart0_init(0x00, 0x00);            //��ʼ�����ڣ�����żУ�飬ֹͣλΪ1λ
	ISR();          //�жϿ���
} 

void main()
{
	sys_init();
	while(1)
	{
		TaskProcess();
	}
} 

/*********    �жϴ�������Ӻ���	*********/
#pragma vector = T1_VECTOR
__interrupt void T1_ISR()
{
         T1IE = 0;          //�ض�ʱ��T1���ж�
	 n++;
	 if(n >5)	   //5msΪһ��ʱ��Ƭ
	 {
	 	n = 0;
		TaskRermark();
	 }
	 T1CC0L = 62500/256;
	 T1CC0H = 62500%256; //��ʱ1ms
	 T1IE = 1;    //����ʱ��T1���ж�
}


/*   �������б�־����   */
void TaskRermark()
{
	uchar i;
	for(i=0;i<TASK_MAX;i++)		     //���������
	{
		if(Taskcomps[i].Timer)		 //��������Ϊ"0"ʱ
		{
			Taskcomps[i].Timer--;
		}
		else
		{
		   Taskcomps[i].Timer = Taskcomps[i].ItvTime;   //�ָ���ʱ
		   Taskcomps[i].Run = 1;          //ʱ��Ƭ����,���б�־��1   
		}
	}
} 

/**      ��������     **/
void TaskProcess()
{
	uchar i;
	for(i=0;i<TASK_MAX;i++)
	{
		if(Taskcomps[i].Run)		   //���б�־Ϊ"1"
		{
			Taskcomps[i].TaskHook();   //ִ����Ӧ����
			Taskcomps[i].Run = 0;      //����֮����"0"
		}

	}
}

/*ms�������ʱ����*/
void delay(uint z)	
{
	uint x,y;
	for(x = z; x > 0; x--)
		for(y = 320; y > 0 ; y--);
} 

/**********   ����Ҫִ�еĺ��� *********/
void  TaskLED1()
{
	  D7 = 0;
	  delay(100);
          D7 = 1;
          delay(100);
}

void TaskLED2()
{
    D6 = 0;
    delay(50);
    D6 = 1;
    delay(50);   
}



