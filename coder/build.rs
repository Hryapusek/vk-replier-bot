use std::env;
use std::fs;
use std::path::Path;

fn main() {
    // Notify Cargo to rerun the script if the source files change
    println!("cargo:rerun-if-changed=test_files");

    // Get the target directory (default is `target`)
    let target_dir = env::var("CARGO_TARGET_DIR")
        .unwrap_or_else(|_| "target".to_string());
    let debug_dir = Path::new(&target_dir).join("debug");

    // Destination path for test files
    let dest_path = debug_dir.join("test_files");

    // Ensure the destination directory exists
    if let Err(e) = fs::create_dir_all(&dest_path) {
        println!("cargo:note=Failed to create test_files directory: {}", e);
        return;
    }

    // Copy files from a "test_files" directory in the project root
    match fs::read_dir("test_files") {
        Ok(entries) => {
            for entry in entries {
                match entry {
                    Ok(entry) => {
                        let path = entry.path();
                        if path.is_file() {
                            let file_name = path.file_name().unwrap();
                            let dest_file = dest_path.join(file_name);
                            if let Err(e) = fs::copy(&path, &dest_file) {
                                println!("cargo:note=Failed to copy file {}: {}", path.display(), e);
                            } else {
                                println!("cargo:note=Copied {} to {}", path.display(), dest_file.display());
                            }
                        }
                    }
                    Err(e) => println!("cargo:note=Failed to read directory entry: {}", e),
                }
            }
        }
        Err(e) => println!("cargo:note=Failed to read test_files directory: {}", e),
    }

    println!("cargo:note=Test files copied to {:?}", dest_path);
}
