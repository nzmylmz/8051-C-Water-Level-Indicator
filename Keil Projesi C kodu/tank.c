/*K�t�phaneler*/
#include <stdio>
#include <reg51.h>
/*Pin Tanimlari*/
sbit rs=P2^5;//LCD RS pini
sbit rw=P2^6;//LCD RW pini
sbit e=P2^7; //LCD EN pini
sbit quat=P1^3; //tankin �eyrek(quarter) seviye pini
sbit half=P1^2; //tankin yarim(half) seviye pini
sbit quat_3=P1^1; //tankin 3/4 seviye pini
sbit full=P1^0; //tankin dolu(full) seviye pini
sbit spkr_on=P1^4; //Buzzer pini 
sbit pump=P1^5; // pompa tetikleme pini

/*Fonksiyonlar*/
void delay(int k) //Bekleme Fonksiyonu
{
int i,j;
for(i=0;i<k;i++)
  for(j=0;j<1275;j++);
}//delay

void write(int j) //LCD'ye veri yazma fonk.
{
rs=1;  //LCD'veri g�nderme modu
rw=0;  //LCD'ye yazilacak
P3=j;  //veriyi LCD'data pinlerine y�kle
e=1;  //Enable pini lojik 1'e �ekilir 
delay(1); //Bekle
e=0; //Enable pini lojik 0'a �ek ve veri ekrana g�r�l�r.
return;
}//write

void cmd(int j)  //LCD'ye komut g�nderme fonk.
{
P3=j;  //Komutu LCD'data pinlerine y�kle
rs=0;  //G�nderilen degerin bir komut oldugunu bildir
rw=0;  //LCD'ye yaz
e=1;  //EN lojik 1 yap
delay(1); //Bekle
e=0; //LCD'deki degisikligi uygula
return;
}//cmd

void puts(char *a) //Karakter katarini LCD'ye yazma fonk.
{
unsigned int p=0;
for(;a[p]!=0;p++)//Gelen string boyunca d�n
write(a[p]); //Her karakteri write fonk.'a g�nder
}//puts

void lcd_init(void) // LCD kullanima hazirlama fonk.
{
cmd(0x38); //8bit 2 satir 16 s�tun ayarlamasi
delay(1);
cmd(0x0e); //imleci g�ster
delay(1);     
cmd(0x01); //ekrani temizle
cmd(0x80); //LCD'nin 1.satir 1.s�tununa imleci yerlestir
}//lcd_init
//Fonksiyonlarin Sonu

/*Ana Program*/
void main()
{
	quat=half=quat_3=full=1; //Pinler giris(input)olarak tanimlaniyor
	quat=half=quat_3=full=0; //Pinler d�sen kenara �ekiliyor
	spkr_on=pump=0;    // baslangi�ta buzzer ve pompa pasif yapiliyor
	lcd_init();        // initialising LCD
	puts("Welcome To Water");
	cmd(0xC0);							//Ikinci satir ilk s�tun
	puts("Level Indicator");
	delay(300);
	while(1)
	{
		while(quat==0&&half==0&&quat_3==0&&full==0)//Tankin bos olma sarti 
		{
			pump=1; //Bossa pompa �alisssin
			spkr_on=0; //Bossa buzzer dursun
			lcd_init();   
			puts("VACANT"); //Ekrana VACANT yazdir
			while(quat==0); //Bos oldugu m�ddet�e bekle
		}
		while(quat==1&&half==0&&quat_3==0&&full==0)//Tankin ilk �eyregi doldugu an
		{
			pump=1; //pump �alismaya devam etsin
			spkr_on=0;
			lcd_init();
			puts("QUATER");      //Ekrana QUARTER yaz
			while(half==0 && quat==1); //�eyrek durum sonlanana kadar bekle
		}
		while(quat==1&&half==1&&quat_3==0&&full==0)//tankin yarim oldugu durum
		{
			pump=1;
			spkr_on=0;
			lcd_init();     
			puts("HALF");      //Ekrana HALF yazdir
			while(quat_3==0 && half==1); //Yarim durum sonlanan kadar bekle
		}
		while(quat==1&&half==1&&quat_3==1&&full==0)//tankin 3/4'� dolu oldugu durum
		{
			pump=1;
			spkr_on=0;
			lcd_init();
			puts("3/4 FULL");     //Ekrana 3/4 Full yazdir
			while(full==0 && quat_3==1); //3/4 durum sonlanana kadar bekle
		}
		while(quat==1&&half==1&&quat_3==1&&full==1)//tankin dolu oldugu durum
		{
			pump=0;//tank doldugunda pompayi durdur
			spkr_on=1;// Buzzeri �alistir
			lcd_init();
			puts("FULL;CLOSE TAP");     //Ekrana Full;Close TAP yazdir
			while(full==1);//Dolu oldugu m�ddet�e bekle
		}
	}
}

