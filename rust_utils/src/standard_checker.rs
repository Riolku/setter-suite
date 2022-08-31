use super::checker::{codes, exit};
use super::reader::{self, AsciiStream, ErrorHandler as _, Reader};
use super::standard_whitespace;
use std::io::BufRead;

pub struct ErrorHandler;

impl reader::ErrorHandler for ErrorHandler {
    fn new() -> Self {
        Self {}
    }
    fn wrong_whitespace(&self) -> ! {
        exit(codes::WA);
    }
    fn out_of_range(&self) -> ! {
        exit(codes::WA);
    }
    fn parse_error(&self) -> ! {
        exit(codes::WA);
    }
}

pub fn new(
    src: impl BufRead,
) -> Reader<standard_whitespace::Handler<impl AsciiStream>, ErrorHandler> {
    reader::new(standard_whitespace::new(src), ErrorHandler::new())
}
