use crate::default_types::{Debtor, DebtorBase};
use std::{cell::RefCell, num::ParseFloatError, path::Path, rc::Rc};

pub trait DebtsFileParser<E> {
    fn parse_file(&self, path: &Path) -> Result<Vec<Rc<RefCell<dyn DebtorBase>>>, E>;
    fn delimiter(&self) -> u8;
}

#[derive(Debug)]
pub enum CSVDebtsFileParserError {
    FileNotFound(String),
    PathIsNotFile(String),
    IncorrectFileExtension(String),
    BadFileData(String),
    BadRecordData(String, String),
}

pub struct CSVDebtsFileParser
{
    delimiter: u8
}

impl CSVDebtsFileParser {
    pub fn new(delimiter: u8) -> CSVDebtsFileParser {
        CSVDebtsFileParser { delimiter }
    }
}

impl Default for CSVDebtsFileParser {
    fn default() -> Self {
        CSVDebtsFileParser::new(b';')
    }
}

impl DebtsFileParser<CSVDebtsFileParserError> for CSVDebtsFileParser {
    fn delimiter(&self) -> u8 {
        self.delimiter
    }
    
    fn parse_file(
        &self,
        path: &Path,
    ) -> Result<Vec<Rc<RefCell<dyn DebtorBase>>>, CSVDebtsFileParserError> {
        if !path.exists() {
            return Err(CSVDebtsFileParserError::FileNotFound(
                path.to_str().unwrap().to_owned(),
            ));
        }

        if !path.is_file() {
            return Err(CSVDebtsFileParserError::PathIsNotFile(
                path.to_str().unwrap().to_owned(),
            ));
        }

        if !path.extension().unwrap().eq("csv") {
            return Err(CSVDebtsFileParserError::IncorrectFileExtension(
                path.to_str().unwrap().to_owned(),
            ));
        }

        let mut builder = csv::ReaderBuilder::new();
        builder.delimiter(self.delimiter);
        builder.has_headers(false);
        let mut reader = builder.from_path(path).map_err(|_| {
            CSVDebtsFileParserError::FileNotFound(path.to_str().unwrap().to_owned())
        })?;

        let mut result: Vec<Rc<RefCell<dyn DebtorBase>>> = vec![];

        for record in reader.records() {
            let record = record.map_err(|_| {
                CSVDebtsFileParserError::BadFileData(path.to_str().unwrap().to_owned())
            })?;

            if record.len() != 2 {
                return Err(CSVDebtsFileParserError::BadRecordData(
                    path.to_str().unwrap().to_owned(),
                    record.as_slice().to_string() + format!(" - Expected 2 elements, found {}", record.len()).as_str(),
                ));
            }

            let name = record[0].to_string();
            let amount: f64 = record[1].parse().map_err(|err: ParseFloatError| {
                CSVDebtsFileParserError::BadRecordData(
                    path.to_str().unwrap().to_owned(),
                    err.to_string() + record.as_slice(),
                )
            })?;

            result.push(Rc::new(RefCell::new(Debtor::new(name, amount))));
        }

        Ok(result)
    }
}

mod tests {
    use std::path::Path;
    use super::*;

    #[test]
    fn parse_default_input_file() {
        let file_path = Path::new("test_files/input.csv");

        let parser = super::CSVDebtsFileParser::default();
        let vector = parser.parse_file(file_path).unwrap();

        assert_eq!(vector.len(), 4, "Expected 4 records. Records are {:?}", vector);

        assert_eq!(vector[0].borrow().name(), "Абраамян Александр Манвелович");
        assert_eq!(vector[0].borrow().amount(), 5395.2);

        assert_eq!(vector[1].borrow().name(), "Два Александр Манвелович");
        assert_eq!(vector[1].borrow().amount(), 2395.2);

        assert_eq!(vector[2].borrow().name(), "Три Александр Манвелович");
        assert_eq!(vector[2].borrow().amount(), 3395.2);

        assert_eq!(vector[3].borrow().name(), "Четыре Александр Манвелович");
        assert_eq!(vector[3].borrow().amount(), 4395.2);
    }
}
