use super::error_log::ErrorLog;
use super::logger_receiver::LoggerReceiver;
use super::logger_sender::LoggerSender;
use super::nivel::Nivel;

use std::path::Path;
use std::sync::mpsc;

pub(crate) type MensajeLog = (Nivel, String);

/// Crea la dupla Sender-Receiver de mensajes de Logs
pub fn inicializar_logger(archivo_loggear: &Path) -> Result<(LoggerSender, LoggerReceiver), ErrorLog> {
    let (sender, receiver) = mpsc::channel::<MensajeLog>();

    let logger_sender = LoggerSender::new(sender);
    let logger_receiver = LoggerReceiver::new(archivo_loggear, receiver)?;

    Ok((logger_sender, logger_receiver))
}
