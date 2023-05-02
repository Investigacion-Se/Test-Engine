#[derive(Debug)]
pub enum Nivel {
    FATAL,
    ERROR,
    ADVERTENCIA,
    INFO,
    DEBUG,
    RASTRO,
}

impl std::fmt::Display for Nivel {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
            Nivel::FATAL => write!(f, "FATAL"),
            Nivel::ERROR => write!(f, "ERROR"),
            Nivel::ADVERTENCIA => write!(f, "ADVERTENCIA"),
            Nivel::INFO => write!(f, "INFO"),
            Nivel::DEBUG => write!(f, "DEBUG"),
            Nivel::RASTRO => write!(f, "RASTRO"),
        }
    }
}
