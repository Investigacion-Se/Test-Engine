use super::logger_sender::LoggerSender;
use super::logger_receiver::LoggerReceiver;
use super::errores::Error;

use std::sync::mpsc;
use std::path::Path;

pub/*(crete)*/ fn inicializar_logger(archivo_loggear: &Path) -> Result<(LoggerSender, LoggerReceiver), Error> {

    let (sender, receiver) = mpsc::channel::<String>();

    let logger_sender = LoggerSender::new(sender);
    let logger_receiver = LoggerReceiver::new(archivo_loggear, receiver)?;

    Ok((logger_sender, logger_receiver))
}

