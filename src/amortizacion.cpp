#include <Rcpp.h>
#include <misc.h>
#include <fechas.h>

using namespace Rcpp;
using namespace fechas;

int plazos_(double monto, double tasa, double pago) {
  return round((log(pago) - log(pago - tasa * monto)) / log(tasa + 1));
}

double cuota_(double monto, double tasa, int plazos) {
  if (tasa == 0)
    return monto / plazos;
  else
    return monto * (tasa + (tasa / (std::pow(1 + tasa, plazos) - 1)));
}

//' @export
// [[Rcpp::export]]
IntegerVector plazos(NumericVector monto, NumericVector tasa, NumericVector pago) {
  int n = monto.size();
  IntegerVector output(n);

  for (int i = 0; i < n; i++) {
    output[i] = plazos_(monto[i], tasa[i], pago[i]);
  }

  return output;
}

//' @title
//' Cuota de pago mensual
//'
//' @description
//' Calcula la cuota de pago mensual siguiendo la fórmula:
//'
//' \deqn{A = P (\frac{i (1 + i)^{n}}{(1 + i)^{n} - 1}))}
//'
//' para i > 0. Si i = 0, entonces simplemente:
//'
//' \deqn{A = P/n}
//'
//' @param monto  [double(1)]\cr Monto del prestamo
//' @param tasa   [double(1)]\cr Tasa de interés del préstamo
//' @param plazos [int(1)]\cr Plazo del crédito
//'
//' @export
// [[Rcpp::export]]
NumericVector cuota(NumericVector monto, NumericVector tasa, IntegerVector plazos) {
  int n = monto.size();
  NumericVector output(n);

  for (int i = 0; i < n; i++) {
    output[i] = cuota_(monto[i], tasa[i], plazos[i]);
  }

  return output;
}

//' @title
//' Ceiling quincenal
//'
//' @description
//' Devuelve la fecha de corte
//'
//' @param fecha  [Date(1): required]\cr Fecha de la cual calcular la quincena
//'
//' @return
//' [Date(1)] Próxima quincena
//'
//' @export
// [[Rcpp::export]]
DateVector ceiling_fecha(DateVector fechas, NumericVector pagos_mensuales) {
  int n = fechas.size();
  DateVector output(n);

  for (int i = 0; i < n; i++) {
    bdate bfecha = Date_to_date(fechas[i]);
    bdate fecha_quincena = ceiling_date(bfecha, pagos_mensuales[i]);
    output[i] = date_to_Date(fecha_quincena);
  }

  return output;
}

// @title
// Tabla de amortización
//
// @description
// Calcula la tabla de amortización lo más cercano posible a como lo hacen
// algunas instituciones de crédito. Este método es un método iterativo que ocupa varios
// redondeos en múltiples fases del proceso. Para entender exactamente en que
// momentos y en que valores se hacen los redondeos, se recomienda leer el
// código fuente.
//
// @inheritParams         cuota
// @param fecha_inicial   [Date(1)]\cr Fecha de vencimiento de la primera cuota
// @param pagos_mensuales [int(1)]\cr Número de cuotas mensuales
//
// @export
// [[Rcpp::export]]
DataFrame amortizacion(
    double monto,
    double tasa,
    int plazos,
    Date fecha_inicial,
    int pagos_mensuales
  ) {
  double cuota_mensual = round_away_(cuota_(monto, tasa, plazos), 2);

  IntegerVector numero_de_cuota = seq(1, plazos);
  IntegerVector numero_de_cuotas_pendientes = plazos - numero_de_cuota;

  NumericVector pago(plazos, cuota_mensual);
  DateVector fechas = fechas_de_pago(fecha_inicial, plazos, pagos_mensuales);

  IntegerVector pagos_mensuales_vector(plazos, pagos_mensuales);
  NumericVector saldo(plazos);
  NumericVector interes_con_iva(plazos);
  NumericVector capital(plazos);
  NumericVector interes_sin_iva(plazos);
  NumericVector iva(plazos);

  saldo[0] = monto;
  interes_con_iva[0] = round_away_(round_away_(monto * tasa, 6), 2);
  interes_sin_iva[0] = round_away_(interes_con_iva[0] / 1.16, 2);
  capital[0] = round_away_(cuota_mensual - interes_con_iva[0], 3);
  iva[0] = interes_con_iva[0] - interes_sin_iva[0];

  for (int i = 1; i < plazos; i++) {
    saldo[i] = saldo[i - 1] - capital[i - 1];
    interes_con_iva[i] = round_away_(saldo[i] * tasa, 2);
    interes_sin_iva[i] = round_away_(interes_con_iva[i] / 1.16, 2);
    capital[i] = round_away_(cuota_mensual - interes_con_iva[i], 3);
    iva[i] = interes_con_iva[i] - interes_sin_iva[i];
  }

  double diferencia = monto - sum(capital);

  capital[plazos - 1] = round_away_(capital[plazos - 1] + diferencia, 2);
  pago[plazos - 1] = capital[plazos - 1] + interes_con_iva[plazos - 1];

  return DataFrame::create(
    _["fecha"] = fechas,
    _["numero_de_cuota"] = numero_de_cuota,
    _["numero_de_cuotas_pendientes"] = numero_de_cuotas_pendientes,
    _["pago"] = pago,
    _["saldo"] = saldo,
    _["interes_con_iva"] = interes_con_iva,
    _["interes_sin_iva"] = interes_sin_iva,
    _["iva"] = iva,
    _["capital"] = capital
  );
}
