# include <at89x52.h>
# include <8051_LCD_8_BIT.H>
# include <string.h>

bit Engine=0;
sbit motor=P1^3;
idata unsigned char mob_no[]={"+919029398900"};
idata unsigned char mob_no_2[]={"+919029398900"};// Mobile Number To Send Sms.
# include <8051_Serial.H>
# include <8051_GSM.H>

int Speed_sensor_value=0;
idata unsigned char gps_info_lat[18];
idata unsigned char gps_info_log[18];

bit Gas = 0;
bit Alcohol = 0;
bit Accident = 0;
bit Fire = 0;
bit Emergency = 0;

sbit red=P1^1;
sbit green=P1^2;
sbit buzzer=P1^0;

sbit Voice_message_1=P0^0;//speed 
sbit Voice_message_2=P0^1;//gas
sbit Voice_message_3=P0^2;//alcohol
sbit Voice_message_4=P0^3;//fire
sbit Voice_message_5=P0^4;//accident
sbit Voice_message_6=P0^5;//emergency
sbit Voice_message_7=P0^6;//normal
sbit Voice_message_8=P0^7;//

sbit D0=P1^7;
sbit D1=P1^6;
sbit D2=P1^5;
sbit VT=P1^4;
int d=0;
bit sms_bit=1;
int car_speed_limit=0;
int car_speed=0;

void init();
void init_display();
void area_road_speed_limit();
void send_message();
void sensor_status_update();
void gps_data_update();
void sensor_status_show();
void warning();
	
void main()
{ 
	red=1;
	green=1; 
	buzzer=0;
	motor=0;    
	init();
	
	init_Serial();
	D0=1;
	D1=1;
	D2=1;
	VT=1;
	send_string("SM_ARU...");
	IE=0x90;
	EA=1;
	
	while(1)
	{	
		lcd_cmd(0x01);
		if(update_data==1)
		{
			update_data=0;		
			sensor_status_update();
			sensor_status_show();
			gps_data_update();
		}
	  area_road_speed_limit();
//		if(Engine==1)
//		{motor=1;}
//		if(Engine==0)
//		{motor=0;}
	}
}

void init()
{
	lcd_init();
	init_display();
}

void init_display()
	{
		lcd_cmd(0x80);
		lcd_string(" SM_ARU Vehicle With ");
		lcd_cmd(0xc0);
		lcd_string("  Advanced Security ");
		lcd_cmd(0x94);
		lcd_string("      System");
		delay(5000);
		lcd_cmd(0x01);
	}


void area_road_speed_limit()
{
	if(VT==1 && D0==1 && D1==0 && D2==0)
	{
		lcd_cmd(0x80);
		lcd_string("Speed Limit 60 Kmph ");
		car_speed_limit=60;
	}
	
	
	else if(VT==1 && D0==0 && D1==1 && D2==0)
	{
		lcd_cmd(0x80);
		lcd_string("Speed Limit 80 Kmph ");
		car_speed_limit=80;
	}
	
	else if(VT==1 && D0==0 && D1==0 && D2==1)
	{
		lcd_cmd(0x80);
		lcd_string("Speed Limit 140 Kmph");
		car_speed_limit=140;
	}
	
	else
	{
		lcd_cmd(0x80);
		lcd_string("No Speed Limit Area ");
		car_speed_limit=1000;
	}
		delay(1000);
}


void send_message()
{			
	send_string("AT+CMGF=1\r\n");
	delay(2000);
	send_string("AT&W\r\n");
	delay(2000);
	lcd_cmd(0xC0);
	lcd_string("mob_no:");
	lcd_cmd(0x94);
	lcd_string(mob_no);
	number_Enter();
	send_string("I am in trouble help me...\r\n");
	delay(1000);
	send_string("My Location...\r\n");
	delay(1000);
	send_string(gps_info_lat);
	delay(1000);
	send_string(gps_info_log);
	
	if(Gas==1){send_string("Gas Leackage Detect\r\n");}
	if(Alcohol==1){send_string("Alcohol Detect\r\n");}
	if(Fire==1){send_string("Fire Leackage Detect\r\n");}
	if(Emergency==1){send_string("Emergency Mode\r\n");}
	
	send_string("http://www.geoplaner.com data in form[dd.mm.mmm]\r\n\r\n");
	delay(2000);
	Tx_char(0x1a);
	delay(4000);
	
	lcd_cmd(0x01);
	lcd_string("Sending Sms...");
	send_string("AT+CMGF=1\r\n");
	delay(2000);
	send_string("AT&W\r\n");
	delay(2000);
	lcd_cmd(0xC0);
	lcd_string("mob_no 2:");
	lcd_cmd(0x94);
	lcd_string(mob_no_2);
	number_Enter_2();
	send_string("I am in trouble help me...\r\n");
	delay(1000);
	send_string("My Location...\r\n");
	delay(1000);
	send_string(gps_info_lat);
	delay(1000);
	send_string(gps_info_log);
	
	if(Gas==1){send_string("Gas Leackage Detect\r\n");}
	if(Alcohol==1){send_string("Alcohol Detect\r\n");}
	if(Fire==1){send_string("Fire Leackage Detect\r\n");}
	if(Emergency==1){send_string("Emergency Mode\r\n");}
	
	send_string("http://www.geoplaner.com data in form[dd.mm.mmm]\r\n\r\n");
	delay(2000);
	Tx_char(0x1a);
	delay(4000);
}

void sensor_status_update()
{
	
	car_speed=(sensor_info[7]-0x30)*20;
	lcd_cmd(0x80);	
	lcd_data(sensor_info[1]);
	
	lcd_data(sensor_info[2]);
	
	lcd_data(sensor_info[3]);
	
	lcd_data(sensor_info[4]);
	
	lcd_data(sensor_info[5]);
	
	lcd_data(sensor_info[6]);
	
	//lcd_data(sensor_info[7]);
	
	lcd_data(sensor_info[8]);
	lcd_data(sensor_info[9]);
	delay(1000);
//	d=car_speed;
//	lcd_data(((d/100)%10)+0x30);
//	lcd_data(((d/10)%10)+0x30);
//	lcd_data((d%10)+0x30);
	
	
	if(sensor_info[1]=='G')Gas=1;
	if(sensor_info[1]=='g')Gas=0;
	
	if(sensor_info[2]=='F')Fire=1;
	if(sensor_info[2]=='f')Fire=0;
	
	if(sensor_info[3]=='D')Alcohol=1;
	if(sensor_info[3]=='d')Alcohol=0;
	
	if(sensor_info[4]=='A')Accident=1;
	if(sensor_info[4]=='a')Accident=0;
	
	if(sensor_info[5]=='E')Emergency=1;
	if(sensor_info[5]=='e')Emergency=0;
	
	if(sensor_info[6]=='S')Engine=1;
	if(sensor_info[6]=='s')Engine=0;
	
	
}


void gps_data_update()
{
	int i=1,j=0;
	
	while(i<15)
	gps_info_lat[j++]=gps_info[i++];
	
	gps_info_lat[j++]=('\0');
	j=0;
	
	while(i<29)
	gps_info_log[j++]=gps_info[i++];
	
	gps_info_log[j++]=('\0');
	
	lcd_cmd(0xc0);
		lcd_string(gps_info_lat);
		lcd_cmd(0x94);
		lcd_string(gps_info_log);
//		lcd_cmd(0xD4);
//		//lcd_string(gps_info);

//		lcd_cmd(0xD4);
//		lcd_string("SM_ARU...");
		delay(2000);
//	
}


void sensor_status_show()
{
	lcd_cmd(0xD4);
	lcd_string("Car Speed:");
	d=car_speed;
	lcd_data(((d/10)%10)+0x30);
	lcd_data((d%10)+0x30);
	lcd_string("Kmph    ");
	delay(1000);
	if(car_speed>car_speed_limit){lcd_cmd(0xD4);lcd_string("Above Speed limit   ");warning();}
	if(Gas==1){lcd_cmd(0xD4);lcd_string("Gas Leackage Detect ");warning();}
	if(Alcohol==1){lcd_cmd(0xD4);lcd_string("Alcohol Detect      ");warning();}
	if(Fire==1){lcd_cmd(0xD4);lcd_string("Fire Leackage Detect ");warning();}
	if(Accident==1){lcd_cmd(0xD4);lcd_string("Accident Detect ");warning();}
	if(Emergency==1){lcd_cmd(0xD4);lcd_string("Emergency Detect   ");warning();}
	if(Engine==1){lcd_cmd(0xD4);lcd_string("Engine On          ");delay(1000);}
	if(Engine==0){lcd_cmd(0xD4);lcd_string("Engine Off        ");delay(1000);}
	
	if((Gas==1 || Alcohol==1 || Fire==1 || Emergency==1|| Accident==1) && sms_bit==1)
	{
		lcd_cmd(0x01);
		lcd_string("Sending Sms...");
		sms_bit=0;
		send_message();
	}
	if(Gas==0 && Alcohol==0 && Fire==0 && Emergency==0)
	{red=1;green=0;buzzer=0;sms_bit=1;lcd_cmd(0xD4);lcd_string("System Normal       ");sms_bit=1;}
}

void warning()
{
	red=0;green=1;buzzer=1;Voice_message_8=0;delay(1000);Voice_message_8=1;
	red=0;green=1;buzzer=0;
}