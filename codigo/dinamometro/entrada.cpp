#include "entrada.h"
#include "filtro.h"
#include "saida.h"

#include <avr/interrupt.h>


ISR(ADC_vect){
    // 5.0 Variáveis locais
    float contagensADC;
    double out; //y3 para mais uma seção.

    // 5.1. Processamento e Filtro, para mais seções inclua aqui também.
    //volts = (ADC / 1023.0) * 5.0;
    contagensADC = ADC;

    out = filtrar(contagensADC);
    enviarDadosFila(out);


    // 5.3. PINO DE DEBUG
    // Pulsa o pino PB5 para verificar a Fs/2 no osciloscópio.
    PORTB ^= (1 << 5);

    // 5.4. Limpa o Flag de Interrupção do Timer0
    // (Obrigatório: o auto-trigger do ADC só dispara novamente numa nova borda
    //  de subida deste flag. Como a interrupção do Timer0 não está habilitada,
    //  ninguém o limpa automaticamente.)
    TIFR0 = (1 << OCF0A);
}

void inicializarADC() {

    //configuração do ADC

    cli(); // Mantém interrupções desligadas até terminar toda a configuração

    // 6.1. Configuração do ADC
    // DIDR0: Desabilita o buffer digital no pino ADC0 (A0) para reduzir ruído.
    DIDR0 = (1 << ADC0D);
    // ADMUX: REFS0=1 -> Ref=AVCC (5V). MUX=0000 -> Canal ADC0.
    ADMUX = (1 << REFS0);
    // ADCSRA:
    // ADEN=1: Liga o ADC.
    // ADATE=1: Habilita Auto-Trigger.
    // ADIE=1: Habilita Interrupção de Conversão Completa.
    // ADPS=111: Prescaler 128. (F_adc_clk = 16MHz/128 = 125kHz).
    ADCSRA = (1 << ADEN) | (1 << ADATE) | (1 << ADIE) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
    // ADCSRB: ADTS=011 -> Define o 'trigger' como "Timer0 Compare Match A".
    ADCSRB = (1 << ADTS1) | (1 << ADTS0);

    // 6.2. Configuração do Timer0 (Fs=1000Hz)
    // (O core do Arduino já configurou o Timer0 para PWM/millis antes do
    //  setup(); as atribuições abaixo sobrescrevem essa configuração.)
    // TCCR0A: WGM01=1 -> Modo CTC (Clear Timer on Compare Match).
    // O timer conta de 0 até OCR0A e zera.
    TCCR0A = (1 << WGM01);
    // TCCR0B: CS0=011 -> Prescaler 64.
    // Clock do Timer = 16.000.000 Hz / 64 = 250.000 Hz
    // (Consulte "Passo 1" do tutorial para valores de 256 ou 1024)
    TCCR0B = (1 << CS01) | (1 << CS00);
    // OCR0A: Define o valor TOP (teto) da contagem (0-255).
    // Fs = Clock_Timer / OCR0A = 250.000 Hz / 250 = 1000 Hz
    // (Consulte "Passo 1" do tutorial para calcular este valor)
    //OCR0A = 250; //################################################Clock_Timer / (OCR0A + 1) o correto seria OCR0A=249
    OCR0A = 249;
    // Desliga a interrupção de overflow do Timer0 que o core usa para millis()
    TIMSK0 = 0;
    // Limpa o flag do timer (boa prática de inicialização)
    TIFR0 = (1 << OCF0A);


    // 6.5. Configuração do Pino de Debug
    // Define PB5 (Pino 13 do Arduino) como SAÍDA
    pinMode(13, OUTPUT);

    sei(); // Habilita Interrupções Globais
}
