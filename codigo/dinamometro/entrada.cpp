#include "entrada.h"
#include "filtro.h"
#include "saida.h"

#include <avr/interrupt.h>

// Fs = 1000 Hz. 100 amostras => 10 linhas/s na serial (use 1000 para 1 linha/s)
#define AMOSTRAS_POR_LINHA 100

ISR(ADC_vect){
    static uint16_t contador = 0;   // local à ISR, não é global

    filtrar(ADC);

    if (++contador >= AMOSTRAS_POR_LINHA) {
        contador = 0;
        sinalizarSaida();
    }

    // Pulsa PB5 para verificar Fs/2 no osciloscópio
    PORTB ^= (1 << 5);

    // Limpa o flag do Timer0 (necessário para o próximo auto-trigger do ADC)
    TIFR0 = (1 << OCF0A);
}

void inicializarADC() {

    cli();

    DIDR0 = (1 << ADC0D);
    ADMUX = (1 << REFS0);
    ADCSRA = (1 << ADEN) | (1 << ADATE) | (1 << ADIE) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    ADCSRB = (1 << ADTS1) | (1 << ADTS0);

    // Timer0 em CTC, prescaler 64: 250 kHz / (249+1) = 1000 Hz
    TCCR0A = (1 << WGM01);
    TCCR0B = (1 << CS01) | (1 << CS00);
    OCR0A = 249;
    TIMSK0 = 0;
    TIFR0 = (1 << OCF0A);

    pinMode(13, OUTPUT);

    sei();
}