#ifndef FILTRO_H
#define FILTRO_H

void inicializarFiltro();
void BiQuad_initWithCoefficients(BiQuadC *bq, double b0, double b1, double b2,
                                 double a1, double a2, double scaleValue);
double BiQuad_signalSample(BiQuadC *bq, double x);
double filtrar(float volts);

#endif
