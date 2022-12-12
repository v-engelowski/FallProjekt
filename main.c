/*
 * schwerkraftMessen.c
 *
 * Created: 11/22/2022 8:00:17 AM
 * Author : ForZa
 */ 


#include <util/delay.h>
#include <avr/io.h>
#include "../../../myLib/lcd-routines.h"


#define FALLPORT	PORTA
#define FALLPIN		PINA
#define FALLDDR		DDRA

#define KUGEL		PORTA0
#define PLATTE		PORTA1

#define FALLDISTANZ 0.312f

char str[24];
int timer = 0;


//void float_to_string(float _f) 
//{
	//memset(str, '\0', sizeof(str));
	//sprintf(str ,"%d.%d", (int) _f, (int) ((_f - (int) _f) * 1000));
//}


void init()
{
	// Vorwiderstand einschalten
	FALLPORT  |= (1 << KUGEL);
	//FALLPORT  |= (1 << PLATTE); //nur wenn testen mit knöpfen
	
}

void warteAufFall() {
	lcd_setcursor(0,1);
	lcd_string("Warte auf Fall");
	
	while((FALLPIN & (1 << KUGEL)) == 0);	// Warte bis kein Kontakt mit der Kugel
	//lcd_clear();
	//lcd_string("Zeit läuft");
	
}

void warteAufAufprall() {	

	
	while(!(FALLPIN & (1 << PLATTE))) {	// Warte bis Platte nicht mehr 0V hat
		timer++;	
		_delay_ms(1);
	}
}

void warteAufKugelKontakt() {
	lcd_setcursor(0,1);
	lcd_string("Kugel nach oben bitte");
	
	while((FALLPIN & (1 << KUGEL)) != 0);

	lcd_setcursor(0,1);
	lcd_string("Kugel wieder oben");
	_delay_ms(2000);
	lcd_clear();
}

float berechneErdanziehung(float _t) {
	_t = _t / 1000.0f;			//ms -> s
	
	//return FALLDISTANZ / pow(_t, 2);
	return 2 * FALLDISTANZ / pow(_t, 2);
}


int main(void)
{
	//char text[100];
	

	init();
	lcd_init();
	//lcd_setcursor(0,1);
	//lcd_string("Let's go");
	//
	//_delay_ms(100);
	
    for(;;)
    {
		timer = 0;
		warteAufFall();
		warteAufAufprall();
		
		float anz = berechneErdanziehung(timer);
		
		//float_to_string(anz);	// Erdanziehung
		//float_to_string(timer);	// Fallzeit
		
		lcd_clear();
		lcd_string(itoa(timer, str, 10));	// Int to ASCII
		
		//for(;;);
		
		_delay_ms(1000);
		
		lcd_clear();
		
		warteAufKugelKontakt();
    }
}

