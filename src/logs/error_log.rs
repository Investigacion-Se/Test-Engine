/// Representa los errores posibles para los logs
/// 
/// ### Errores
///  * `ErrorNoSeEncuentraReceiver`
///  * `ErrorNoSePudoEncontrarElArchivo`
///  * `ErrorNoSePudoAgregarTextoAlArchivo`
#[derive(Debug)]
pub enum ErrorLog {
    /// Este error puede aparecer cuando no existe un receiver
    ErrorNoSeEncuentraReceiver,
    /// Este error va a aparecer cuando el archivo pasado no se exista
    ErrorNoSePudoEncontrarElArchivo,
    /// Este error va a aparece cuando no se puede agregar más lineas al archivo dado
    ErrorNoSePudoAgregarTextoAlArchivo,
}
