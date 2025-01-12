use std::{cell::RefCell, io::Write, path::PathBuf, rc::Rc};

use num_format::{Buffer, CustomFormat, Grouping};

use crate::{
    debts_reader::{CSVDebtsFileParser, DebtsFileParser},
    default_types::DebtorBase,
    egui,
    encoder::*,
    pair_coder::{PairCoder, PairCoderBase},
};
use eframe::egui::Visuals;
use log::{error, info};
use rfd::FileDialog;

use crate::settings::{Settings, SettingsBase};

pub struct MyApp {
    settings: Box<dyn SettingsBase>,
    pair_coder: Box<dyn PairCoderBase>,
    encoder: Encoder,
    debts_reader: CSVDebtsFileParser,
    error_text: String,
    search_text: String,
    number_format: CustomFormat,
    debug: bool,
}

impl MyApp {
    fn get_or_create_settings() -> Box<dyn SettingsBase> {
        let settings: Settings;
        if let Ok(created_settings) = Settings::new("test_files/settings.json") {
            settings = created_settings;
        } else {
            let mut file = std::fs::File::create("settings.json").unwrap();
            file.write(b"{}").unwrap();
            settings = Settings::new("settings.json").unwrap();
        }

        Box::new(settings)
    }

    fn create_pair_coder() -> Box<dyn PairCoderBase> {
        Box::new(PairCoder::default())
    }
}

impl Default for MyApp {
    fn default() -> Self {
        let number_format = CustomFormat::builder()
            .grouping(Grouping::Standard)
            .separator("'");
        Self {
            settings: Self::get_or_create_settings(),
            pair_coder: Self::create_pair_coder(),
            encoder: Encoder::default(),
            debts_reader: CSVDebtsFileParser::default(),
            error_text: String::new(),
            search_text: String::new(),
            number_format: number_format.build().unwrap(),
            debug: false,
        }
    }
}

impl MyApp {
    fn encode_and_save_debtors(&mut self, debtors: Vec<Rc<RefCell<dyn DebtorBase>>>) {
        let file_path = rfd::FileDialog::new()
            .set_title("Выберите место для сохранения")
            .add_filter("csv", &["csv"])
            .save_file();
        if let None = file_path {
            info!("No file selected. Canceling process");
            return;
        }
        let output_file = std::fs::File::create(file_path.unwrap()).unwrap();
        let mut writer = csv::WriterBuilder::new()
            .delimiter(self.debts_reader.delimiter())
            .from_writer(output_file);
        for debtor in debtors {
            let encoded = self.pair_coder.encode(&debtor.borrow().name());
            writer
                .write_record(&[encoded.to_string(), debtor.borrow().amount().to_string()])
                .unwrap();
        }
    }

    fn process_file(&mut self, file_path: &PathBuf) -> Result<(), String> {
        info!("Processing file {:?}", file_path);
        let debtors = self.debts_reader.parse_file(file_path);
        if let Ok(debtors) = debtors {
            info!("Parsed {} debtors", debtors.len());
            self.encode_and_save_debtors(debtors);
            Ok(())
        } else {
            error!("Failed to parse file: {:?}", debtors.as_ref().unwrap_err());
            Err(format!("Не удалось обработать файл. Данные загружены в неправильном формате. Ошибка для разработчика: {:?}", debtors.as_ref().unwrap_err()))
        }
    }
}

impl eframe::App for MyApp {
    fn update(&mut self, ctx: &egui::Context, _frame: &mut eframe::Frame) {
        egui::CentralPanel::default().show(ctx, |ui| {
            ui.heading("Приложение для кодирования долгов");
            ui.label(format!("Людей загружено: {}", self.pair_coder.keys_count()));
            if self.pair_coder.keys_count() == 0 {
                ui.label(format!(
                    "Чтобы начать использовать приложение - выберите файл с долгами"
                ));
            }
            if ui.button("Выбрать файл с долгами").clicked() {
                let file = FileDialog::new()
                    .add_filter("csv", &["csv"])
                    .set_directory(std::env::current_dir().unwrap())
                    .pick_file();
                if let Some(file) = file.as_ref() {
                    match self.process_file(&file) {
                        Ok(()) => {
                            info!("File processed successfully");
                            self.error_text = String::new();
                        }
                        Err(e) => {
                            self.error_text = e;
                        }
                    }
                } else {
                    info!("No file selected. Canceling process");
                }
            }

            ui.label("");
            ui.label("Введите ФИО человека в поле ниже чтобы узнать его код");
            ui.text_edit_singleline(&mut self.search_text).highlight();

            if self.search_text.len() > 0 {
                match self.pair_coder.get_key_by_value(&self.search_text) {
                    Some(key) => {
                        let mut buf = Buffer::new();
                        buf.write_formatted(&key, &self.number_format);
                        ui.label(format!("Код: {}", buf.as_str()));
                        if ui.button("Скопировать код").clicked() {
                            ui.output_mut(|o| o.copied_text = key.to_string());
                        }
                    }
                    None => {
                        ui.label(format!("Должник не наиден"));
                    }
                }
            }

            if self.error_text.len() > 0 {
                ui.label("При обработке файла произошла ошибка: ".to_string() + &self.error_text);
            }

            if self.debug {
                ctx.show_viewport_immediate(
                    egui::ViewportId::from_hash_of("debug_viewport"),
                    egui::ViewportBuilder::default()
                        .with_title("Debug Viewport")
                        .with_inner_size([200.0, 100.0]),
                    |ctx, class| {
                        assert!(
                            class == egui::ViewportClass::Immediate,
                            "This egui backend doesn't support multiple viewports"
                        );
                    },
                );
            }
        });
    }
}
