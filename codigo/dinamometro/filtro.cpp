#include "filtro.h"

#include <Arduino.h>


static_assert(sizeof(double) == 4, "Este sketch requer AVR (double de 4 bytes).");

// --- 1. Estrutura do Filtro BiQuad ---
typedef struct {
    double B[3];       // Coeficientes do numerador (b0, b1, b2)
    double A[2];       // Coeficientes do denominador (a1, a2) - (a0 é 1)
    double wz[2];      // Estados internos (atrasos) wz[0]=w[n-1], wz[1]=w[n-2]
    double scaleValue; // Ganho (Scale Value) desta seção
} BiQuadC;


BiQuadC bq1; //bq2
BiQuadC *prtbq1 = &bq1;
//BiQuadC *prtbq2 = &bq2;

// --- 2. Função de Inicialização do Filtro ---
void BiQuad_initWithCoefficients(BiQuadC *bq, double b0, double b1, double b2,
                                    double a1, double a2, double scaleValue) {
    bq->B[0] = b0; bq->B[1] = b1; bq->B[2] = b2;
    bq->A[0] = a1; bq->A[1] = a2;
    bq->wz[0] = 0;
    bq->wz[1] = 0;
    bq->scaleValue = scaleValue;
                                }
//Função de Inicialização do setup(declarar no .h)
void inicializarFiltro(){
    // 6.3. Coeficientes do Filtro (Gerados pelo MATLAB) Ex. abaixo: Passa baixa, Butterworth, Fs = 1000 Hz, frequencia de corte = 4 Hz e frequencia de rejeição = 40 Hz (-40dB)
    BiQuad_initWithCoefficients(prtbq1, 1.00000000e+00, 2.00000000e+00, 1.00000000e+00, -1.96427336e+00,9.64900457e-01, 1.567731501764e-04 );

    //===== verifiar se nao vai dar BO com uma freq 2hz
}

// --- 3. Função de Processamento de Amostra ---
//sai do arquivo então declarar no .h
double BiQuad_signalSample(BiQuadC *bq, double x) {
    double y;
    y = bq->B[0] * x + bq->wz[0];
    bq->wz[0] = bq->B[1] * x - bq->A[0] * y + bq->wz[1];
    bq->wz[1] = bq->B[2] * x - bq->A[1] * y;
    y *= bq->scaleValue;
    return y;
}

double filtrar(float contagens){
    // 5.0 Variáveis locais
    double y; //y2 para mais uma seção.

    y = BiQuad_signalSample(prtbq1, contagens);
    //y2 = BiQuad_signalSample(prtbq2, y);
    // y3 = BiQuad_signalSample(prtbq3, y2);
    return y;
}

