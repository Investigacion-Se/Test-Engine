use test_engine::logs::{logger, errores};
use std::path::Path;
use std::thread;
use std::time::Duration;

fn main() -> Result<(), errores::Error>{

    let archivo = Path::new("algo.txt");
    let (sender, receiver) = logger::inicializar_logger(archivo)?;

    let handle = thread::spawn(move || {
        for mensaje in receiver {
            print!("{mensaje}");
        }
    });

    let sender_copia = sender.clone();
    thread::spawn(move || {
        sender_copia.log_fatal(format!("Mensaje de prueba: {}", 3.14)).unwrap();
        thread::sleep(Duration::from_millis(500));
    });

    thread::spawn(move || {
        sender.log_error(format!("Mensaje de prueba: {}", 3.14)).unwrap();
        thread::sleep(Duration::from_millis(1000));
    });

    handle.join().unwrap();

    Ok(())
}