use super::reader::ErrorHandler;

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
}
