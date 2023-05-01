use test_engine::logs::logger;


fn main() {
    logger::log_fatal(format!("Mensaje de prueba: {}", 3.14));
    logger::log_error(format!("Mensaje de prueba: {}", 3.14));
    logger::log_advertencia(format!("Mensaje de prueba: {}", 3.14));
    logger::log_info(format!("Mensaje de prueba: {}", 3.14));
    logger::log_debug(format!("Mensaje de prueba: {}", 3.14));
    logger::log_rastro(format!("Mensaje de prueba: {}", 3.14));
}