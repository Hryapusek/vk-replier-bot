use std::fmt::Debug;


pub trait DebtorBase: Debug {
  fn name(&self) -> &str;
  fn amount(&self) -> f64;
}

#[derive(Debug, Default)]
pub struct Debtor {
  name: String,
  amount: f64,
}

impl Debtor {
  pub fn new(name: String, amount: f64) -> Debtor {
    Debtor { name, amount }
  }
}

impl DebtorBase for Debtor {
  fn name(&self) -> &str {
    self.name.as_str()
  }
  fn amount(&self) -> f64 {
    self.amount
  }
}
