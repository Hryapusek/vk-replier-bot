trait SettingsBase {
    fn save_to_file(&mut self);

    fn password(&mut self) -> Option<&str>;
    fn set_password(&mut self, password: Option<&str>);

    fn input_file(&mut self) -> Option<&str>;
    fn set_input_file(&mut self, input_file: Option<&str>);

    fn decode_file(&mut self) -> Option<&str>;
    fn set_decode_file(&mut self, decode_file: Option<&str>);
}

#[derive(serde::Deserialize, serde::Serialize)]
struct PureSettings {
    password: Option<String>,
    input_file: Option<String>,
    decode_file: Option<String>,
}

struct Settings {
    path_to_json: String,
    pure_settings: PureSettings,
}

impl Settings {
    pub fn new(path_to_json: &str) -> Result<Self, String> {
        let pure_settings =
            serde_json::from_reader(std::fs::File::open(path_to_json).map_err(|e| e.to_string())?)
                .map_err(|e| e.to_string())?;
        Ok(Settings {
            path_to_json: path_to_json.to_string(),
            pure_settings,
        })
    }
}

impl SettingsBase for Settings {
    fn password(&mut self) -> Option<&str> {
        self.pure_settings.password.as_ref().map(|s| s.as_str())
    }

    fn set_password(&mut self, password: Option<&str>) {
        if let None = password {
            self.pure_settings.password = None;
        } else if let Some(password) = password {
            self.pure_settings.password = Some(password.to_string());
        }
        self.save_to_file();
    }

    fn input_file(&mut self) -> Option<&str> {
        self.pure_settings.input_file.as_ref().map(|s| s.as_str())
    }

    fn set_input_file(&mut self, input_file: Option<&str>) {
        self.pure_settings.input_file = input_file.map(|s| s.to_string());
        self.save_to_file();
    }

    fn decode_file(&mut self) -> Option<&str> {
        self.pure_settings.decode_file.as_ref().map(|s| s.as_str())
    }

    fn set_decode_file(&mut self, decode_file: Option<&str>) {
        self.pure_settings.decode_file = decode_file.map(|s| s.to_string());
        self.save_to_file();
    }

    fn save_to_file(&mut self) {
        serde_json::to_writer_pretty(
            std::fs::File::create(&self.path_to_json).unwrap(),
            &self.pure_settings,
        )
        .unwrap();
    }
}

mod tests {
    use super::*;
    use dotenv::dotenv;

    #[test]
    fn test_settings() {
        dotenv().ok();
        let mut settings = Settings::new("test_files/settings.json").unwrap();
        settings.set_password(Some("pwd"));
        assert_eq!(settings.password(), Some("pwd"));
        settings.set_input_file(Some("input_file"));
        assert_eq!(settings.input_file(), Some("input_file"));
        settings.set_decode_file(Some("decode_file"));
        assert_eq!(settings.decode_file(), Some("decode_file"));
    }
}
