use super::reader::ErrorHandler;

#[derive(Debug)]
pub struct Handler;

impl Handler {
    pub fn new() -> Self {
        Self {}
    }
}

impl ErrorHandler for Handler {
    fn out_of_range(&self) -> ! {
        panic!("out of range")
    }
    fn parse_error(&self) -> ! {
        panic!("parse error")
    }
    fn wrong_whitespace(&self) -> ! {
        panic!("wrong whitespace")
    }
    fn non_ascii(&self) -> ! {
        panic!("non ascii");
    }
}
