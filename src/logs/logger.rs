use super::{
    logger_receiver::LoggerReceiver,
    logger_sender::LoggerSender,
    nivel::Nivel,
};

use std::{
    sync::mpsc::channel, 
    io::Write
};

pub(crate) type MensajeLog = (Nivel, String);

/// Crea la dupla Sender-Receiver de mensajes de Logs
pub fn inicializar_logger<W: Write>(archivo: W) -> (LoggerSender, LoggerReceiver<W>) {
    let (sender, receiver) = channel::<MensajeLog>();


    let logger_sender = LoggerSender::new(sender);
    let logger_receiver = LoggerReceiver::new(archivo, receiver);

    (logger_sender, logger_receiver)
}
