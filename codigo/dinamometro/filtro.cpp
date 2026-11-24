#include "filtro.h"
#include <Arduino.h>

// Filtro em 2 estágios (tudo em inteiros, sem erro acumulado):
//
// Estágio 1: média de blocos de 50 amostras (50 ms a 1 kHz) -> decimação p/ 20 Hz
//            zeros em 20, 40, 60... Hz  => elimina 60 Hz e os aliases em 20 Hz
// Estágio 2: média móvel de 40 blocos (2 s) a 20 Hz
//            zeros em 0,5 Hz e múltiplos => elimina 1, 2 e 2,5 Hz
//
// Resposta equivalente a uma média de 2 s (2000 amostras) usando só 40 posições.
#define BLOCO   50
#define JANELA  40

static uint16_t acumBloco = 0;      // máx. 50*1023 = 51.150 (cabe em 16 bits)
static uint8_t  contBloco = 0;
static uint16_t buffer[JANELA];     // 80 bytes
static uint8_t  indice = 0;
static volatile uint8_t  preenchidos = 0;  // blocos válidos (evita rampa no início)
static volatile uint32_t soma = 0;         // máx. 40*51.150 = 2.046.000

void inicializarFiltro() {
    acumBloco = 0;
    contBloco = 0;
    indice = 0;
    preenchidos = 0;
    soma = 0;
    for (uint8_t i = 0; i < JANELA; i++) {
        buffer[i] = 0;
    }
}

// Roda na ISR a 1 kHz: na maioria das vezes é só uma soma de 16 bits
void filtrar(uint16_t contagens) {
    acumBloco += contagens;

    if (++contBloco < BLOCO) return;

    // Fim do bloco (a cada 50 ms): atualiza a média móvel
    soma -= buffer[indice];
    buffer[indice] = acumBloco;
    soma += acumBloco;

    acumBloco = 0;
    contBloco = 0;

    if (++indice >= JANELA) indice = 0;
    if (preenchidos < JANELA) preenchidos++;
}

// Roda no loop(): devolve a média em contagens do ADC
float lerMediaFiltrada() {
    uint32_t s;
    uint8_t n;

    noInterrupts();
    s = soma;
    n = preenchidos;
    interrupts();

    if (n == 0) return 0.0;     // ainda sem nenhum bloco completo

    return (float)s / ((float)BLOCO * n);
}
