use super::logger::MensajeLog;
use std::sync::mpsc::Sender;

use super::errores::Error;
use super::nivel::Nivel;

#[derive(Debug, Clone)]
pub struct LoggerSender {
    sender: Sender<MensajeLog>,
}

impl LoggerSender {
    pub(crate) fn new(sender: Sender<MensajeLog>) -> Self {
        LoggerSender { sender }
    }

    pub(crate) fn loggear(&self, nivel: Nivel, mensaje: String) -> Result<(), Error> {
        if self.sender.send((nivel, mensaje)).is_err() {
            return Err(Error::ErrorNoSeEncuentraReceiver);
        }
        Ok(())
    }

    pub fn log_fatal(&self, mensaje: String) -> Result<(), Error> {
        self.loggear(Nivel::FATAL, mensaje)?;
        Ok(())
    }

    pub fn log_error(&self, mensaje: String) -> Result<(), Error> {
        self.loggear(Nivel::ERROR, mensaje)?;
        Ok(())
    }

    pub fn log_advertencia(&self, mensaje: String) -> Result<(), Error> {
        self.loggear(Nivel::ADVERTENCIA, mensaje)?;
        Ok(())
    }

    pub fn log_info(&self, mensaje: String) -> Result<(), Error> {
        self.loggear(Nivel::INFO, mensaje)?;
        Ok(())
    }

    pub fn log_debug(&self, mensaje: String) -> Result<(), Error> {
        self.loggear(Nivel::DEBUG, mensaje)?;
        Ok(())
    }

    pub fn log_rastro(&self, mensaje: String) -> Result<(), Error> {
        self.loggear(Nivel::RASTRO, mensaje)?;
        Ok(())
    }
}
