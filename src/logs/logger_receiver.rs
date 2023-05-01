use super::errores::Error;
use super::logger::MensajeLog;
use super::nivel::Nivel;
use std::{
    fs::{File, OpenOptions},
    io::Write,
    path::Path,
    sync::mpsc::Receiver,
};

pub struct LoggerReceiver {
    receiver: Receiver<MensajeLog>,
    archivo: File,
}

impl LoggerReceiver {
    pub(crate) fn new(archivo_log: &Path, receiver: Receiver<MensajeLog>) -> Result<Self, Error> {
        let mut resultado_archivo = OpenOptions::new().append(true).open(archivo_log);

        if resultado_archivo.is_err() {
            resultado_archivo = File::create(archivo_log);
        }

        let archivo = match resultado_archivo {
            Ok(archivo) => archivo,
            _ => {
                return Err(Error::ErrorNoSePudoCrearArchivo);
            }
        };

        Ok(LoggerReceiver { receiver, archivo })
    }

    pub fn recibir_logs(self) -> Result<(), Error> {
        let mut archivo = self.archivo;

        for (nivel, mensaje) in self.receiver {
            let texto = Self::format_mensaje(nivel, mensaje);

            if archivo.write(texto.as_bytes()).is_err() {
                return Err(Error::ErrorNoSePudoAgregarTextoAlArchivo);
            }

            print!("{texto}");
        }

        Ok(())
    }

    fn format_mensaje(nivel: Nivel, mensaje: String) -> String {
        format!("{nivel} {mensaje}\n")
    }
}
