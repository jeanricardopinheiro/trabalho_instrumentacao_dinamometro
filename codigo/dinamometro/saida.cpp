#include "saida.h"
#include "ajuste.h"
#include "filtro.h"
#include <Arduino.h>

// Único estado global deste módulo: flag de "hora de imprimir"
static volatile bool imprimir = false;

void inicializarSerial() {
    Serial.begin(9600);
}

void sinalizarSaida() {
    imprimir = true;
}

void enviarDadosSerial() {
    if (!imprimir) return;
    imprimir = false;

    float tensao = tratamentoTensao(lerMediaFiltrada());
    float forca  = tratamentoForca(tensao);

    Serial.print("Forca lida: ");
    Serial.print(forca, 5);
    Serial.print(" N | Tensao ADC: ");
    Serial.print(tensao, 5);
    Serial.println(" V");
}