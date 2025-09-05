use std::str::Bytes;
use age::secrecy::SecretSlice;

pub trait EncoderBase {
  fn encode(&mut self, value: &std::vec::Vec<u8>) -> Result<std::vec::Vec<u8>, age::EncryptError>;
  fn decode(&self, encoded_value: &std::vec::Vec<u8>) -> Result<std::vec::Vec<u8>, age::DecryptError>;
}

pub struct Encoder {
  key: age::secrecy::SecretString
}

impl Default for Encoder {
  fn default() -> Self {
    Self {
      key: age::secrecy::SecretString::from(std::env::var("SECRET_KEY").unwrap())
    }
  }
}

impl Encoder {
  pub fn new(key: age::secrecy::SecretString) -> Self {
    Self { key }
  }
}

impl EncoderBase for Encoder {
  fn encode(&mut self, value: &std::vec::Vec<u8>) -> Result<std::vec::Vec<u8>, age::EncryptError> {
    age::encrypt(&age::scrypt::Recipient::new(self.key.clone()), value)
  }

  fn decode(&self, encoded_value: &std::vec::Vec<u8>) -> Result<std::vec::Vec<u8>, age::DecryptError> {
    age::decrypt(&age::scrypt::Identity::new(self.key.clone()), encoded_value)
  }
}

mod tests {
  use super::{Encoder, EncoderBase};
  use dotenv::dotenv;

  #[test]
  fn test_encoder() {
    dotenv().ok();
    let mut encoder = super::Encoder::default();
    let value = vec![1, 2, 3, 4];
    let encoded_value = encoder.encode(&value).unwrap();
    let decoded_value = encoder.decode(&encoded_value).unwrap();
    assert_eq!(value, decoded_value);
  }
}
