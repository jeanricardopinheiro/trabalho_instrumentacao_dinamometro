#ifndef SAIDA_H
#define SAIDA_H

void inicializarSerial();

// Chamada dentro da ISR: avisa o loop() quando é hora de imprimir.
void sinalizarSaida();

// Chamada no loop(): imprime se houver sinalização pendente.
void enviarDadosSerial();

#endif