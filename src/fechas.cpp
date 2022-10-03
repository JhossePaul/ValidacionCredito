#include <Rcpp.h>
#include <fechas.h>
using namespace Rcpp;

// [[Rcpp::plugins(cpp17)]]
// [[Rcpp::depends(BH)]]

#include <boost/date_time.hpp>

namespace fechas {
  bdate Date_to_date(Date fecha) {
    return bdate(fecha.getYear(), fecha.getMonth(), fecha.getDay());
  }

  Date date_to_Date (bdate fecha) {
    return Date(fecha.month(), fecha.day(), fecha.year());
  }

  bdate date_ceiling_quincenal(bdate fecha) {
    return fecha.day() < 16 ?
      bdate(fecha.year(), fecha.month(), 15) :
      fecha.end_of_month();
  }

  bdate ceiling_date(bdate fecha, int pagos) {
    return pagos == 1 ? fecha.end_of_month() : date_ceiling_quincenal(fecha);
  }

  DateVector dates_to_Dates (bdate_vector fechas) {
    int n = fechas.size();
    DateVector output(n);

    for (int i = 0; i < n; i++) {
      output[i] = date_to_Date(fechas[i]);
    }

    return output;
  }

  bdate_vector serie_mensual(bdate fecha_inicial, int plazos) {
    bdate_vector output(plazos);
    month_iterator iterator(fecha_inicial);

    int i = 0;
    while (i < plazos) {
      output[i] = *iterator;
      ++iterator; ++i;
    }

    return(output);
  }

  bdate_vector serie_quincenal(bdate fecha_inicial, int plazos) {
    dd incremento(1);
    bdate_vector output(plazos, date_ceiling_quincenal(fecha_inicial));

    for (int i = 1; i < plazos; i++) {
      output[i] = date_ceiling_quincenal(output[i - 1] + incremento);
    }

    return output;
  }

  bdate_vector serie_fechas(bdate fecha_inicial, int plazos, int pagos_mensuales) {
    return pagos_mensuales  == 1 ?
      serie_mensual(fecha_inicial, plazos) :
      serie_quincenal(fecha_inicial, plazos);
  }
}
