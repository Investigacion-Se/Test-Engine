use super::{logger_receiver::LoggerReceiver, logger_sender::LoggerSender, nivel::Nivel};

use std::{io::Write, sync::mpsc::channel};

pub(crate) type MensajeLog = (Nivel, String);

/// Crea la dupla Sender-Receiver de mensajes de Logs
pub fn inicializar_logger<W: Write>(
    archivo: W,
    show_in_terminal: bool,
) -> (LoggerSender, LoggerReceiver<W>) {
    let (sender, receiver) = channel::<MensajeLog>();

    let logger_sender = LoggerSender::new(sender);
    let logger_receiver = LoggerReceiver::new(archivo, receiver, show_in_terminal);

    (logger_sender, logger_receiver)
}
