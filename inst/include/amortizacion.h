#ifndef AMORTIZACION_H
#define AMORTIZACION_H

#include <Rcpp.h>

using namespace Rcpp;

int plazos_(double monto, double tasa, double pago);
double cuota_(double monto, double tasa, int plazos);
IntegerVector plazos(NumericVector monto, NumericVector tasa, NumericVector pago);
NumericVector cuota(NumericVector monto, NumericVector tasa, IntegerVector pagos);
double cuota_(double monto, double tasa, int pagos);
Date ceiling_quincenal(Date fecha);
DataFrame amortizacion(double monto, double tasa, int plazos, Date fecha_inicial, int pagos_mensuales);

#endif
