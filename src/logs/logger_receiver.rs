use super::{
    error_log::ErrorLog,
    logger::MensajeLog,
    nivel::Nivel,
};

use std::{
    io::Write,
    sync::mpsc::Receiver,
};

/// Se encarga de manejar los mensajes de logs que se tenga que enviar para registrar las operaciones
/// 
/// ### Errores
///  * `Error::ErrorNoSePuedeEncontrarElArchivo`: Este error va a aparecer cuando el archivo pasado no se exista
///  * `Error::ErrorNoSePudoAgregarTextoAlArchivo`: Este error va a aparece cuando no se puede agregar más lineas al archivo dado
pub struct LoggerReceiver<W: Write> {
    receiver: Receiver<MensajeLog>,
    archivo: W,
}

impl<W: Write> LoggerReceiver<W> {
    /// Crea el receiver a partir del path de un archivo y un receiver de channel
    /// 
    /// ### Errores
    ///  * `Error::ErrorNoSePuedeEncontrarElArchivo`: Este error va a aparecer cuando el archivo pasado no se exista
    pub(crate) fn new(archivo: W, receiver: Receiver<MensajeLog>) -> Self {
        LoggerReceiver { receiver, archivo }
    }

    /// La acción de recibir los mensajes mandados por `LoggerSender`
    /// 
    /// ### Errores
    ///  * `Error::ErrorNoSePudoAgregarTextoAlArchivo`: Este error va a aparece cuando no se puede agregar más lineas al archivo dado
    pub fn recibir_logs(self) -> Result<(), ErrorLog> {
        let mut archivo = self.archivo;

        for (nivel, mensaje) in self.receiver {
            let texto = Self::format_mensaje(nivel, mensaje);

            if archivo.write(texto.as_bytes()).is_err() {
                return Err(ErrorLog::ErrorNoSePudoAgregarTextoAlArchivo);
            }

            print!("{texto}");
        }

        Ok(())
    }

    /// Formatear el mensaje para que aparezca en los logs
    fn format_mensaje(nivel: Nivel, mensaje: String) -> String {
        format!("[{nivel}] {mensaje}\n")
    }
}
