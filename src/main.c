#define F_CPU 4800000UL
#include <avr/interrupt.h>
#include <util/delay.h>

#define ADC_CH (1)
#define ADC_IN (PB2)

#define GREEN_LED (PB3)
#define RED_LED (PB4)

#define turn_on(pin) PORTB |= (1 << (pin))
#define turn_off(pin) PORTB &= ~((1 << (pin)))
#define blink(pin)      \
    do {                \
        turn_on(pin);   \
        _delay_ms(128); \
        turn_off(pin);  \
        _delay_ms(128); \
    } while (1)

ISR(ADC_vect)
{
    if (ADCH >= 195) // Ubat >= 3.9V
        turn_on(GREEN_LED);
    else if (ADCH >= 185) // Ubat >= 3.7V
        blink(GREEN_LED);
    else if (ADCH >= 175) // Ubat >= 3.5V
        turn_on(RED_LED);
    else // Ubat < 3.5V
        blink(RED_LED);
}

int main(void)
{
    DDRB = (1 << RED_LED) | (1 << GREEN_LED);
    PORTB = 0;
    DIDR0 |= (1 << ADC_IN);

    ADMUX = 0b01100000 | ADC_CH;
    ADCSRA = 0b11101111;

    sei();
    while (1)
        ;
}
