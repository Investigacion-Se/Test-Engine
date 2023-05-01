use super::errores::Error;
use std::{sync::mpsc::Receiver, path::Path};

pub struct LoggerReceiver {
    receiver: Receiver<String>
}

impl LoggerReceiver {

    pub(crate) fn new(archivo_log: &Path, receiver: Receiver<String>) -> Result<Self, Error> {

        // TODO: Crear informacion sobre el archivo

        Ok(LoggerReceiver { receiver })
    }

}

impl Iterator for LoggerReceiver {
    type Item = String;

    fn next(&mut self) -> Option<Self::Item> {
        self.receiver.recv().ok()
    }
}