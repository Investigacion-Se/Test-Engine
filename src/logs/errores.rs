use std::sync::mpsc::SendError;

#[derive(Debug)]
pub enum Error {
    NoSeEncuentraReceiver,
    NoExisteArchivo,
}

impl std::convert::From<SendError<String>> for Error {
    fn from(value: SendError<String>) -> Self {
        Error::NoSeEncuentraReceiver
    }
}