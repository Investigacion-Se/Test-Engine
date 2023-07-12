use super::{error_log::ErrorLog, logger::MensajeLog, nivel::Nivel};

use std::{io::Write, sync::mpsc::Receiver};

/// Se encarga de manejar los mensajes de logs que se tenga que enviar para registrar las operaciones
///
/// ### Errores
///  * `ErrorLog::ErrorNoSePudoAgregarTextoAlArchivo`: Este error va a aparece cuando no se puede agregar más lineas al archivo dado
pub struct LoggerReceiver<W: Write> {
    receiver: Receiver<MensajeLog>,
    archivo: W,
    terminal: bool,
}

impl<W: Write> LoggerReceiver<W> {
    /// Crea el receiver a partir del path de un archivo y un receiver de channel
    pub(crate) fn new(archivo: W, receiver: Receiver<MensajeLog>, terminal: bool) -> Self {
        LoggerReceiver {
            receiver,
            archivo,
            terminal,
        }
    }

    /// La acción de recibir los mensajes mandados por `LoggerSender`
    ///
    /// ### Errores
    ///  * `ErrorLog::ErrorNoSePudoAgregarTextoAlArchivo`: Este error va a aparece cuando no se puede agregar más lineas al archivo dado
    pub fn recibir_logs(self) -> Result<(), ErrorLog> {
        let mut archivo = self.archivo;

        for (nivel, mensaje) in self.receiver {
            let texto = Self::format_mensaje(nivel, mensaje);

            if archivo.write(texto.as_bytes()).is_err() {
                return Err(ErrorLog::ErrorNoSePudoAgregarTextoAlArchivo);
            }

            if self.terminal {
                print!("{texto}");
            }
        }

        Ok(())
    }

    /// Formatear el mensaje para que aparezca en los logs
    fn format_mensaje(nivel: Nivel, mensaje: String) -> String {
        format!("[{nivel}] {mensaje}\n")
    }
}
