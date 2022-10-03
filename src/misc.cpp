#include <Rcpp.h>
#include <fechas.h>

using namespace fechas;

double direct_round_away(double x, int n = 0) {
  return std::round(x * pow(10, n)) / pow(10, n);
}

//' @export
// [[Rcpp::export]]
double round_away_(double x, int n = 0) {
  double z = x  * std::pow(10, n);

  z = z + 0.5 + 1e-6;
  z = std::trunc(z);

  return z / std::pow(10, n);
}

// @title
// Saldos Reales
//
// @description
// Calcula el saldo real en todos los periodos por medio de la aplicación de
// pagos registrados.
//
// @param DT Data.table que contiene el desglose de los saldos teóricos y
// las aplicaciones de pagos.
//
// @export
// [[Rcpp::export]]
NumericVector round_away(NumericVector x, int n = 0) {
  int s = x.size();
  NumericVector output(s);

  for (int i = 0; i < s; i++) {
    output[i] = round_away_(x[i], n);
  }

  return output;
}


//' @export
// [[Rcpp::export]]
DateVector fechas_de_pago(Date fecha_inicial, int plazos, int pagos_mensuales) {
  bdate fecha_boost = Date_to_date(fecha_inicial);
  bdate_vector fechas = serie_fechas(fecha_boost, plazos, pagos_mensuales);

  return dates_to_Dates(fechas);
}


//' @export
// [[Rcpp::export]]
DateVector sequence_fechas(Date fecha_inicial, Date fecha_final, int pagos_mensuales) {
  bdate bfecha_inicial = Date_to_date(fecha_inicial);
  bdate bfecha_final = Date_to_date(fecha_final);
  dd delta (1);

  bdate_vector fechas(1, bfecha_inicial);

  while (fechas.back() < bfecha_final) {
    fechas.push_back(ceiling_date(fechas.back() + delta, pagos_mensuales));
  }

  return dates_to_Dates(fechas);
}
