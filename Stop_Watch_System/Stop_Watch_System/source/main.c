#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


char time[]={0,0,0,0,0,0};

void Timer1_CTCA_init(){
	SREG|=(1<<7);
	TIMSK|=(1<<OCIE1A);
	TCCR1A|=(1<<FOC1A)|(1<<FOC1B);
	TCCR1B|=(1<<WGM12)|(1<<CS10)|(1<<CS11);
	TCNT1=0;
	OCR1A=15625;
}

ISR(TIMER1_COMPA_vect){
	time[0]++;
}

void INT_init(){
	DDRD&=~(1<<PD2);
	PORTD|=(1<<PD2);


	DDRD&=~(1<<PD3);

	DDRB&=~(1<<PB2);
	PORTB|=(1<<PB2);

	GICR|=0xE0;
	MCUCR|=0x0B;
}

ISR(INT0_vect){
	for (int i = 0; i < 6; ++i) {
		time[i]=0;
	}

}
ISR(INT1_vect){
	TIMSK&=~(1<<OCIE1A);

}
ISR(INT2_vect){
	TIMSK|=(1<<OCIE1A);

}


int main(int argc, char **argv) {
	DDRC|=0x0F;
	PORTC&=0xF0;
	DDRA|=0x3F;
	PORTA&=0xC0;

	INT_init();
	Timer1_CTCA_init();

	while(1){

		for (int var = 0; var < 6; ++var) {
			PORTA&=0xC0;
			PORTA|=(1<<var);
			PORTC=(PORTC&0xF0)|(time[var]&0x0F);
			_delay_ms(5);
		}
		if(time[0]==10){
			time[0]=0;
			time[1]++;
			if(time[1]==6){
				time[1]=0;
				time[2]++;
				if(time[2]==10){
					time[2]=0;
					time[3]++;
					if(time[3]==6){
						time[3]=0;
						time[4]++;
						if(time[4]==10){
							time[4]=0;
							time[5]++;
						}
						if((time[4]==4)&&(time[5]==2)){
							for (int i = 0; i < 6; ++i) {
									time[i]=0;
								}
						}
					}

				}

			}
		}

	}

}
