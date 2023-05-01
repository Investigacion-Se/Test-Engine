use std::path::Path;
use std::thread;
use std::time::Duration;
use test_engine::logs::{errores, logger};

fn main() -> Result<(), errores::Error> {
    let archivo = Path::new("algo.txt");
    let (sender, receiver) = logger::inicializar_logger(archivo)?;

    let handle = thread::spawn(move || receiver.recibir_logs());

    for i in 0..10 {
        let sender_copia = sender.clone();
        thread::spawn(move || {
            sender_copia
                .log_debug(format!("Mensaje de prueba: {}", i))
                .unwrap();
            thread::sleep(Duration::from_millis(500));
        });
    }

    std::mem::drop(sender);
    let _ = handle.join().unwrap();

    Ok(())
}
