/// Representa los errores posibles para los logs
///
/// ### Errores
///  * `Errorlog::ErrorNoSeEncuentraReceiver`: Este error ocurre cuando se droppea el receiver por
///  lo que el sender no podria mandar el mensaje
///  * `Errorlog::ErrorNoSePudoAgregarTextoAlArchivo`: Este error ocurre cuando la variable con
///  trait Write falla al escribirse
#[derive(Debug, Copy, Clone)]
pub enum ErrorLog {
    /// Este error puede aparecer cuando no existe un receiver
    ErrorNoSeEncuentraReceiver,
    /// Este error va a aparece cuando no se puede agregar más lineas al archivo dado
    ErrorNoSePudoAgregarTextoAlArchivo,
}
