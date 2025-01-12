#![cfg_attr(not(debug_assertions), windows_subsystem = "windows")] // hide console window on Windows in release

mod debts_reader;
mod default_types;
mod pair_coder;
mod encoder;
mod settings;
mod gui;

use eframe::egui;
use log::info;
use dotenv::dotenv;
use gui::MyApp;
use encoder::{EncoderBase, Encoder};

fn main() -> eframe::Result {
    info!("Starting application");
    dotenv().ok();
    env_logger::init(); // Log to stderr (if you run with `RUST_LOG=debug`).
    let options = eframe::NativeOptions {
        viewport: egui::ViewportBuilder::default().with_inner_size([600.0, 300.0]),
        ..Default::default()
    };
    eframe::run_native(
        "My egui App",
        options,
        Box::new(|cc| {
            // This gives us image support:
            egui_extras::install_image_loaders(&cc.egui_ctx);

            cc.egui_ctx.style_mut(|style| style.visuals = egui::Visuals::light());
            cc.egui_ctx.set_pixels_per_point(1.4_f32);

            Ok(Box::<MyApp>::default())
        }),
    )
}
