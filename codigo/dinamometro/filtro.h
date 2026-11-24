#ifndef FILTRO_H
#define FILTRO_H

#include <stdint.h>

void inicializarFiltro();

// Chamada dentro da ISR: insere uma amostra (contagens do ADC) na média móvel.
void filtrar(uint16_t contagens);

// Chamada fora da ISR: devolve a média móvel atual em contagens do ADC.
float lerMediaFiltrada();

#endif