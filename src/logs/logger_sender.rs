use std::sync::mpsc::Sender;

use super::nivel::Nivel;
use super::errores::Error;

#[derive(Debug, Clone)]
pub struct LoggerSender {
    sender: Sender<String>,
}

impl LoggerSender {

    pub(crate) fn new(sender: Sender<String>) -> Self {
        LoggerSender { sender }
    }

    pub(crate) fn loggear(&self, nivel: Nivel, mensaje: String) -> Result<(), Error> {
        let texto_completo = format!("{nivel} {mensaje}\n");
        self.sender.send(texto_completo)?;
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