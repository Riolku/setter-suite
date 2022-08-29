use super::reader::ErrorHandler;

pub struct Handler;

impl ErrorHandler for Handler {
    fn new() -> Self {
        Self {}
    }
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
