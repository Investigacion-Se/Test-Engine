mod error_inicializar;
mod error_ejecucion;

use error_inicializar::ErrorInicializar;
use error_ejecucion::ErrorEjecucion;

use test_engine::logs::{logger, logger_receiver::LoggerReceiver, logger_sender::LoggerSender, error_log::ErrorLog};

use std::{
    thread::{self, JoinHandle},
    fs::{File, OpenOptions},
    path::Path,
    io::Write,
};

fn abrir_archivo_logs(archivo_log: &Path) -> Result<File, ErrorInicializar> {
    let log_file = match OpenOptions::new()
        .create(true)
        .write(true)
        .truncate(true)
        .open(archivo_log)
    {
        Ok(archivo_log) => archivo_log,
        _ => return Err(ErrorInicializar::ArchivoDeLogsNoExiste),
    };

    Ok(log_file)
}

fn manejar_receiver<W: Write + Send + 'static>(logger_receiver: LoggerReceiver<W>) -> JoinHandle<Result<(), ErrorLog>> {
    thread::spawn(move || {
        logger_receiver.recibir_logs()
    })
}

fn terminar_ejecucion(logger_sender: LoggerSender) {
    logger_sender.log_debug("Terminando el logger");
}

fn main() -> Result<(), ErrorEjecucion> {
    println!("Inicializando el logger");

    let archivo_log = Path::new("src/bin/test-bed/logs.lg");
    let archivo = abrir_archivo_logs(&archivo_log)?;

    let (logger_sender, logger_receiver) = logger::inicializar_logger(archivo, true);

    let handle_logger = manejar_receiver(logger_receiver);

    terminar_ejecucion(logger_sender);

    match handle_logger.join() {
        Ok(Ok(_)) => println!("Salio todo bien"),
        Ok(Err(_)) => println!("Algo salio mal en el receiver"),
        Err(_) => println!("Paniquio el thread"),
    }

    Ok(())
}
