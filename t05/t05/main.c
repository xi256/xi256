// main.c
// 2025-04-14

#include "common.h"
#include "osc.h"

#define SEG P1

sbit TP0=P0^7;
sbit TP1=P0^6;

sbit TUBE1=P2^0;
sbit TUBE2=P2^1;
sbit TUBE3=P2^2;
sbit TUBE4=P2^3;
sbit TUBE5=P2^4;
sbit TUBE6=P2^5;


unsigned char LED_BUF[6]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
// character table      "0"  "1"  "2"  "3"  "4"  "5"  "6"  "7"  "8"  "9"
unsigned char CHAR[10]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};

void tube_off(void)
{
	TUBE1=0;
	TUBE2=0;
	TUBE3=0;
	TUBE4=0;
	TUBE5=0;
	TUBE6=0;
}

void seg_off(void)
{
	SEG=0xFF;
}	


void myTest(void)
{
	int i;
	int sta=1;
	
	while(1){
		tube_off();
		seg_off();
		switch(sta)
		{
			case 1:
				TUBE1=1;
				sta=2;
				break;
			case 2:
				TUBE2=1;
				sta=3;
				break;
			case 3:
				TUBE3=1;
				sta=4;
				break;
			case 4:
				TUBE4=1;
				sta=5;
				break;
			case 5:
				TUBE5=1;
				sta=6;
				break;
			case 6:
				TUBE6=1;
				sta=1;
				break;
			default:
				break;
		}
		for(i=0;i<10;i++)
		{
			SEG=CHAR[i];
			Loop_Delay(500);		
		}	
	}
}

/* main entrance */
int main()
{
	unsigned char temp;
	unsigned int i,j;
	Osc_Init_Parameter_t Osc_Init_Parameter;
	
	EA=0; // global interrupt off
	WDTCN=0xDE;
	WDTCN=0xAD; // Watch Dog Timer off
	
	// Config the OSC:
	Osc_Init_Parameter.Source_Select=EXTERNAL_OSC;
	Osc_Init_Parameter.External_Mode=CRYSTAL;
	Osc_Init_Parameter.Frequency=24000000;
	Osc_Init(&Osc_Init_Parameter);
	Loop_Delay(100);
	
	// Enable cross-bar
	XBR2=Reg_Field_Set(XBR2, BIT6);
	
	// Config Timer0 and make it start to run
	// Timer0: 8bit auto-reload
	TL0=(256-200);
	TH0=(256-200);
	
	ET0=1;
	EA=1;
	
	TMOD=0x02;// 0000 0010, Timer0: 8bit auto-reload
	TR0=1; // start Timer0
	
	myTest();
	
	while(1);
	
	return 0;
}// end of main()

void t0_isr(void) interrupt 1 //100us
{
	static unsigned int cnt=0;
	static unsigned int state=0;
	
	if(cnt==1000)//every 0.1s
	{
		cnt=0;
		//P4=~P4;
		switch(state)
		{
			case 0:
				P4=~0x01;
				state=1;
				break;
			case 1:
				P4=~0x02;
				state=2;
				break;
			case 2:
				P4=~0x04;
				state=3;
				break;
			case 3:
				P4=~0x08;
				state=0;
				break;	
			default:
				break;
		}
	}
	
	cnt++;
}