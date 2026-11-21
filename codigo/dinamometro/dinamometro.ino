#include "entrada.h"
#include "filtro.h"
#include "saida.h"

void setup() {
  inicializarFiltro();
  inicializarSerial();
  inicializarADC(); //deixer sempre o ADC por ultimo por causa da interrupção, primeiro configuramos o filtro e o serial
}

void loop() {

  enviarDadosSerial();

}
