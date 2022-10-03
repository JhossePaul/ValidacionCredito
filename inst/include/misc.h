#ifndef MISC_H
#define MISC_H

#include <Rcpp.h>

using namespace Rcpp;

double direct_round_away(double x, int n = 0);
NumericVector round_away(NumericVector x, IntegerVector n = 0);
double round_away_(double x, int n = 0);
DateVector fechas_de_pago(Date fecha_inicial, int plazos, int pagos_mensuales);

#endif
