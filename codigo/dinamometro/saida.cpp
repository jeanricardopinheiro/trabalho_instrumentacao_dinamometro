#include "saida.h"
#include "ajuste.h"
#include <Arduino.h>

#define FIFO_SIZE 128
#define SYNC_FLAG 0xFF
volatile double fifo_buffer[FIFO_SIZE];
volatile uint8_t fifo_head = 0;
volatile uint8_t fifo_tail = 0;

void inicializarSerial(){
    // 6.4. Configuração da UART (Serial @ 115.200 bps)
    // Versão Arduino: Serial.begin() configura UCSR0A/B/C e UBRR0.
    // (No código bare-metal original: UBRR0 = 8 sem U2X0, que na prática dá
    //  ~111 kbps; Serial.begin(115200) usa U2X0 com UBRR0 = 16, erro de ~2%.)
    Serial.begin(115200);
}


void enviarDadosFila(double saidaADC){

    uint8_t next = (fifo_head + 1) & (FIFO_SIZE - 1);

    if (next != fifo_tail) {
        fifo_buffer[fifo_head] = saidaADC;
        fifo_head = next;
    }
}

void enviarDadosSerial(){

    double valorTensao, valorForca, leitura;
    static uint8_t contador = 0;

    if (fifo_head != fifo_tail) {

        noInterrupts();
        leitura = fifo_buffer[fifo_tail];
        fifo_tail = (fifo_tail + 1) & (FIFO_SIZE - 1);
        interrupts();

        if (++contador < 50) return;   // 1000 Hz / 50 = 20 linhas/s
        contador = 0;

        valorTensao = tratamentoTensao(leitura);
        valorForca = tratamentoForca(valorTensao);

        Serial.print("Forca lida: ");
        Serial.print(valorForca, 2);

        Serial.print(" kg/f | Tensao ADC: ");
        Serial.print(valorTensao, 2);

        Serial.println(" V");
    }

}
