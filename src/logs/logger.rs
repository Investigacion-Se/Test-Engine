use super::nivel::Nivel;

pub(crate) fn inicializar_logger() -> bool {
    // TODO: create archivo para hacer el log
    true
}

pub(crate) fn finalizar_logger() {
    // TODO: limpear la creacion del archivo
}

pub fn loggear(nivel: Nivel, mensaje: String) {
    let texto_completo = format!("{nivel} {mensaje}");
    println!("{texto_completo}");
}

pub fn log_fatal(mensaje: String) {
    loggear(Nivel::FATAL, mensaje);
}

pub fn log_error(mensaje: String) {
    loggear(Nivel::ERROR, mensaje);
}

pub fn log_advertencia(mensaje: String) {
    loggear(Nivel::ADVERTENCIA, mensaje);
}

pub fn log_info(mensaje: String) {
    loggear(Nivel::INFO, mensaje);
}

pub fn log_debug(mensaje: String) {
    loggear(Nivel::DEBUG, mensaje);
}

pub fn log_rastro(mensaje: String) {
    loggear(Nivel::RASTRO, mensaje);
}