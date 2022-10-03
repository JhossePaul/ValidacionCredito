#' @export
calcular_tasa <- function(deuda, monto_mensual, periodos) {
  uniroot(
    function(i, deuda, monto_mensual, periodos) {
      monto_mensual / deuda - monto_mensual / deuda * (i + 1) ^ (-periodos) - i
    },
    interval = c(1e-6, 2),
    periodos = periodos, deuda = deuda, monto_mensual = monto_mensual
  )$root
}
