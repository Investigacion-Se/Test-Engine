use super::logger::MensajeLog;
use std::sync::mpsc::Sender;

use super::error_log::ErrorLog;
use super::nivel::Nivel;

/// Se encarga de mandar los mensajes de logs, este se puede clonar para tener varios senders
///
/// ### Errores
///  * `Error::ErrorNoSeEncuentraReceiver`: Este error puede aparecer cuando no existe un receiver
#[derive(Debug, Clone)]
pub struct LoggerSender {
    sender: Sender<MensajeLog>,
}

impl LoggerSender {
    pub(crate) fn new(sender: Sender<MensajeLog>) -> Self {
        LoggerSender { sender }
    }

    /// Envia el mensaje deseado con su nivel de prioridad
    /// ### Errores
    ///  * `Error::ErrorNoSeEncuentraReceiver`: Este error puede aparecer cuando no existe un receiver
    pub(crate) fn loggear(&self, nivel: Nivel, mensaje: String) {
        if self.sender.send((nivel, mensaje)).is_err() {
            println!("No se puede usar este sender");
        }
    }

    /// Envia el mensaje deseado con el nivel `Nivel::FATAL`
    ///
    /// ### Errores
    ///  * `Error::ErrorNoSeEncuentraReceiver`: Este error puede aparecer cuando no existe un receiver
    pub fn log_fatal(&self, mensaje: String) {
        self.loggear(Nivel::FATAL, mensaje)
    }

    /// Envia el mensaje deseado con el nivel `Nivel::ERROR`
    ///
    /// ### Errores
    ///  * `Error::ErrorNoSeEncuentraReceiver`: Este error puede aparecer cuando no existe un receiver
    pub fn log_error(&self, mensaje: String) {
        self.loggear(Nivel::ERROR, mensaje)
    }

    /// Envia el mensaje deseado con el nivel `Nivel::ADVERTENCIA`
    ///
    /// ### Errores
    ///  * `Error::ErrorNoSeEncuentraReceiver`: Este error puede aparecer cuando no existe un receiver
    pub fn log_advertencia(&self, mensaje: String) {
        self.loggear(Nivel::ADVERTENCIA, mensaje)
    }

    /// Envia el mensaje deseado con el nivel `Nivel::INFO`
    ///
    /// ### Errores
    ///  * `Error::ErrorNoSeEncuentraReceiver`: Este error puede aparecer cuando no existe un receiver
    pub fn log_info(&self, mensaje: String) {
        self.loggear(Nivel::INFO, mensaje)
    }

    /// Envia el mensaje deseado con el nivel `Nivel::DEBUG`
    ///
    /// ### Errores
    ///  * `Error::ErrorNoSeEncuentraReceiver`: Este error puede aparecer cuando no existe un receiver
    pub fn log_debug(&self, mensaje: String) {
        self.loggear(Nivel::DEBUG, mensaje)
    }

    /// Envia el mensaje deseado con el nivel `Nivel::RASTRO`
    ///
    /// ### Errores
    ///  * `Error::ErrorNoSeEncuentraReceiver`: Este error puede aparecer cuando no existe un receiver
    pub fn log_rastro(&self, mensaje: String) {
        self.loggear(Nivel::RASTRO, mensaje)
    }
}
