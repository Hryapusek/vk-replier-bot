use log::{debug, error, info, log_enabled, Level};
use std::collections::HashMap;

mod json_constants {
    pub const PAIRS_KEY: &str = "pairs";
}

trait PairCoderBase {
    fn decode(&self, key: &u64) -> Option<String>;
    fn encode(&mut self, value: &str) -> u64;
    fn get(&self, key: &u64) -> Option<&str>;
    fn get_key_by_value(&self, value: &str) -> Option<u64>;
    fn load_from_file(&mut self, path: &str) -> Result<(), String>;
    fn save_to_file(&self, path: &str) -> Result<(), String>;
}

#[derive(Default)]
struct PairCoder {
    map: HashMap<u64, String>,
}

impl PairCoderBase for PairCoder {
    fn decode(&self, key: &u64) -> Option<String> {
        self.map.get(key).cloned()
    }

    fn encode(&mut self, value: &str) -> u64 {
        if let Some(key) = self.get_key_by_value(value) {
            return key;
        }

        let mut key = rand::random::<u64>();
        while let Some(_) = self.map.get(&key) {
            key = rand::random::<u64>();
        }
        self.map.insert(key, value.to_owned());
        key
    }

    fn get(&self, key: &u64) -> Option<&str> {
        self.map.get(key).map(|v| v.as_str())
    }

    fn get_key_by_value(&self, value: &str) -> Option<u64> {
        for (key, val) in &self.map {
            if val == value {
                return Some(*key);
            }
        }
        None
    }

    fn load_from_file(&mut self, json_path: &str) -> Result<(), String> {
        let json_data = std::fs::read_to_string(json_path).map_err(|e| e.to_string())?;
        let json: serde_json::Value =
            serde_json::from_str(&json_data).map_err(|e| e.to_string())?;
        let json_pairs = json[json_constants::PAIRS_KEY].as_object();

        if json_pairs.is_none() {
            return Err("No pairs found in JSON".to_string());
        }

        for (key, value) in json_pairs.unwrap() {
            let key = key.parse::<u64>().map_err(|e| e.to_string())?;
            let value = value.as_str().ok_or("Value is not a string")?.to_string();
            self.map.insert(key, value);
        }

        Ok(())
    }

    fn save_to_file(&self, path: &str) -> Result<(), String> {
        let json = serde_json::json!({
            json_constants::PAIRS_KEY: self.map
        });

        let json_data = serde_json::to_string_pretty(&json).map_err(|e| e.to_string())?;
        std::fs::write(path, json_data).map_err(|e| e.to_string())?;
        Ok(())
    }
}

impl PairCoder {
    fn from_json_path(json_path: &str) -> PairCoder {
        let mut result = PairCoder::default();
        if let Err(e) = result.load_from_file(json_path) {
            error!("Failed to load from JSON: {}. Continuing with empty map", e);
        }

        result
    }
}

mod tests {
    use super::*;

    #[test]
    fn default_test() {
        let example_name = "s0m4b0dY";
        let another_name = "hryapusek";

        {
            let mut pair_coder = super::PairCoder::default();
            assert!(pair_coder.map.is_empty());

            let code = pair_coder.encode(&example_name);

            println!("Randomly generated code(just to check): {}", code);

            let decoded = pair_coder.decode(&code);
            assert_eq!(decoded, Some(example_name.to_string()));
        }

        let new_code: u64;

        {
            let mut pair_coder = super::PairCoder::from_json_path("non_existing.json");
            assert!(pair_coder.map.is_empty());

            pair_coder.load_from_file("test_files/key.json").unwrap();
            assert_eq!(pair_coder.decode(&(1 as u64)).unwrap(), example_name);

            new_code = pair_coder.encode(&another_name);
            println!("This pair should be in test_files/output_key.json with all other pairs -> \"{}\": \"{}\"", new_code, another_name);
            pair_coder.save_to_file("test_files/output_key.json").unwrap();
        }

        {
            let pair_coder = super::PairCoder::from_json_path("test_files/output_key.json");
            assert_eq!(pair_coder.decode(&new_code).unwrap(), another_name);
        }
    }
}
