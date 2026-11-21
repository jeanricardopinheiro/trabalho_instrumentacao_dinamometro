#include "ajuste.h"

static const float VREF = 5.0;
static const float ADC_MAX = 1023.0;

// Parâmetros de calibração (exemplo)
static const float GANHO = 1.0;
static const float OFFSET = 0.0;

static const float SAT_MAX = 5.0;
static const float SAT_MIN = 0.0;

double tratamentoTensao(double saidaFiltrada){

    double tensao;

    tensao = (saidaFiltrada/ADC_MAX) * VREF;

    return tensao;
}

double tratamentoForca(double tensaoReal){

    double forca;

    forca = (GANHO * tensaoReal) + OFFSET;

    return forca;

}
