use log::{debug, error, info, log_enabled, Level};
use serde::{ser::SerializeMap, Deserialize};
use std::collections::HashMap;

mod json_constants {
    pub const PAIRS_KEY: &str = "pairs";
    pub const CONFIG_NAME: &str = "pair_coder_keys.json";
}

#[derive(serde::Serialize, serde::Deserialize, Clone, Debug)]
pub struct UserData {
    pub name: String,
    pub amount: Option<f64>,
    /// epoch
    pub last_updated: u128,
}

pub trait PairCoderBase {
    fn decode(&self, key: &u64) -> Option<&UserData>;
    fn encode(&mut self, value: &str, amount: Option<f64>) -> u64;
    fn get_key_by_value(&self, value: &str) -> Option<u64>;
    fn load_from_file(&mut self, path: &str) -> Result<(), String>;
    fn save_to_file(&self, path: &str) -> Result<(), String>;
    fn keys_count(&self) -> usize;
}

pub struct PairCoder {
    map: HashMap<u64, UserData>,
}

impl PairCoderBase for PairCoder {
    fn decode(&self, key: &u64) -> Option<&UserData> {
        self.map.get(key)
    }

    fn encode(&mut self, value: &str, amount: Option<f64>) -> u64 {
        let lowercase_value = value.trim().to_lowercase();
        if let Some(key) = self.get_key_by_value(lowercase_value.as_str()) {
            info!("Found key for value: {} - {}", value, key);
            return key;
        }

        info!("No key found for value: {}", lowercase_value);

        let mut key = rand::random::<u64>();
        while let Some(_) = self.map.get(&key) {
            key = rand::random::<u64>();
        }
        self.map.insert(
            key,
            UserData {
                name: lowercase_value.to_owned(),
                amount,
                last_updated: std::time::SystemTime::now()
                    .duration_since(std::time::SystemTime::UNIX_EPOCH)
                    .unwrap()
                    .as_secs() as u128,
            },
        );
        if let Err(e) = self.save_to_file(json_constants::CONFIG_NAME) {
            error!("Failed to save to JSON: {}", e);
        }
        key
    }

    fn get_key_by_value(&self, value: &str) -> Option<u64> {
        for (key, val) in &self.map {
            if &val.name == &value.trim().to_lowercase() {
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
            let user_data: UserData =
                UserData::deserialize(value.as_object().ok_or("Value is not an object")?)
                    .map_err(|e| e.to_string())?;
            self.map.insert(key, user_data);
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

    fn keys_count(&self) -> usize {
        self.map.len()
    }
}

impl Default for PairCoder {
    fn default() -> PairCoder {
        let mut result = PairCoder::new();
        if let Err(e) = result.load_from_file("pair_coder_keys.json") {
            error!("Failed to load from JSON: {}. Continuing with empty map", e);
        } else {
            info!("Loaded {} pairs from JSON", result.keys_count());
        }

        result
    }
}

impl PairCoder {
    pub fn new() -> PairCoder {
        PairCoder {
            map: HashMap::new(),
        }
    }
}

mod tests {
    use super::*;

    #[test]
    fn default_test() {
        let example_name = "s0m4b0dY".to_lowercase();
        let another_name = "hryapusek".to_lowercase();

        {
            let mut pair_coder = super::PairCoder::new();
            assert!(pair_coder.map.is_empty());

            let code = pair_coder.encode(&example_name, None);

            println!("Randomly generated code(just to check): {}", code);

            let decoded = pair_coder.decode(&code);
            assert_eq!(decoded.unwrap().name, example_name.to_string());
        }

        let new_code: u64;

        {
            let mut pair_coder = super::PairCoder::new();
            pair_coder.load_from_file("non_existing.json");
            assert!(pair_coder.map.is_empty());

            const EXAMPLE_AMOUNT: f64 = 1000.0;

            if let Err(e) = pair_coder.load_from_file("test_files/key.json") {
                println!("Failed to load from JSON: {}", e);
                assert!(false);
            }
            assert_eq!(pair_coder.decode(&(1 as u64)).unwrap().name, example_name);
            assert_eq!(pair_coder.decode(&(2 as u64)).unwrap().name, example_name);
            assert_eq!(pair_coder.decode(&(2 as u64)).unwrap().amount, Some(EXAMPLE_AMOUNT));

            new_code = pair_coder.encode(&another_name, None);
            println!("This pair should be in test_files/output_key.json with all other pairs -> \"{}\": \"{}\"", new_code, another_name);
            pair_coder
                .save_to_file("test_files/output_key.json")
                .unwrap();
        }

        {
            let mut pair_coder = super::PairCoder::new();
            pair_coder.load_from_file("test_files/output_key.json");
            assert_eq!(pair_coder.decode(&new_code).unwrap().name, another_name);
        }
    }
}
