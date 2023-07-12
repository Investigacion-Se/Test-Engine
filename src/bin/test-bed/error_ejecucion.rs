use super::error_inicializar::ErrorInicializar;

use std::convert::From;

#[derive(Debug, Clone)]
pub enum ErrorEjecucion {
    ArchivoNoEncontrado(String),
}

impl From<ErrorInicializar> for ErrorEjecucion {
    fn from(value: ErrorInicializar) -> Self {
        match value {
            ErrorInicializar::ArchivoDeLogsNoExiste => ErrorEjecucion::ArchivoNoEncontrado("De los logs".to_string()),
        }
    }
}
