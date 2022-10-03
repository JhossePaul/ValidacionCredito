#include <Rcpp.h>

using namespace Rcpp;

double pago_en_capital(double sal_teorico, double excedente, double iva_t, double intereses_t){
  double output;

  if(excedente > iva_t + intereses_t) {
    output = excedente - (iva_t + intereses_t);
  } else {
    output = 0;
  }

  return(output);
}

//' @title
//' Saldos Reales
//'
//' @description
//' Calcula el saldo real en todos los periodos por medio de la aplicación de
//' pagos registrados.
//'
//' @param DT Data.table que contiene el desglose de los saldos teóricos y
//' las aplpicaciónes de pagos.
//'
//' @export
// [[Rcpp::export]]
NumericVector saldo_en_t(DataFrame DT) {
  NumericVector cuotas_pagadas = DT["cuotas_pagadas"];
  NumericVector intereses = DT["interes_sin_iva"];
  NumericVector iva = DT["iva"];
  NumericVector saldo_teorico = DT["saldo"];
  NumericVector pagos_reales_acumulados = DT["total_acumulado"];
  NumericVector pago_esperado_acum = DT["cuotas_acum_esperadas"];

  int table_size = intereses.size();
  NumericVector output(table_size);

  for(int i = 0; i < table_size; ++i){
    int pagos_completos = cuotas_pagadas[i];

    if(pagos_completos >= table_size) {
      output[i] = 0;

    } else {
      double pago_cap_t = pago_en_capital(
        saldo_teorico[pagos_completos],
        pagos_reales_acumulados[i] - pago_esperado_acum[i],
        iva[i],
        intereses[i]
      );

      output[i] = saldo_teorico[pagos_completos] - pago_cap_t;
    }
  }

  return output;
}
