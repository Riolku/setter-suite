pub struct ErrorHandler;

use super::checker::{codes, exit};

impl super::reader::ErrorHandler for ErrorHandler {
    fn new() -> Self {
        Self {}
    }
    fn wrong_whitespace(&self) -> ! {
        exit(codes::PE);
    }
    fn out_of_range(&self) -> ! {
        exit(codes::WA);
    }
    fn parse_error(&self) -> ! {
        exit(codes::WA);
    }
}

use super::identical_whitespace;
use super::reader::{self, AsciiStream, ErrorHandler as _, Reader};
use std::io::BufRead;

pub fn new(
    src: impl BufRead,
) -> Reader<identical_whitespace::Handler<impl AsciiStream>, ErrorHandler> {
    reader::new(identical_whitespace::new(src), ErrorHandler::new())
}
