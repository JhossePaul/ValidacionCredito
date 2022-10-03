#ifndef FECHAS_H
#define FECHAS_H
#define BOOST_NO_AUTO_PTR

#include <Rcpp.h>
#include <types.h>

using namespace Rcpp;

namespace fechas {
  bdate Date_to_date(Date fecha);

  Date date_to_Date (bdate fecha);

  bdate date_ceiling_quincenal(bdate fecha);

  bdate ceiling_date(bdate fecha, int pagos);

  DateVector dates_to_Dates (bdate_vector fechas);

  bdate_vector serie_mensual(bdate fecha_inicial, int plazos);

  bdate_vector serie_quincenal(bdate fecha_inicial, int plazos);

  bdate_vector serie_fechas(bdate fecha_inicial, int plazos, int pagos_mensuales);
}

#endif
