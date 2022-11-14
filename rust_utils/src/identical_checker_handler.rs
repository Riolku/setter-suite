use super::checker::{codes, Checker};
use super::identical_whitespace;
use super::reader::{self, AsciiStream, Reader};
use std::io::BufRead;

pub struct ErrorHandler<F> {
    checker: Checker<F>,
}

impl<F> ErrorHandler<F>
where
    F: FnOnce(),
{
    pub fn new(checker: Checker<F>) -> Self {
        Self { checker }
    }
}

impl<F> reader::ErrorHandler for ErrorHandler<F>
where
    F: FnOnce(),
{
    fn wrong_whitespace(&self) -> ! {
        self.checker.exit(codes::PE);
    }
    fn out_of_range(&self) -> ! {
        self.checker.exit(codes::WA);
    }
    fn parse_error(&self) -> ! {
        self.checker.exit(codes::WA);
    }
}

pub fn new_checker<F: FnOnce()>(
    src: impl BufRead,
    checker: Checker<F>,
) -> Reader<identical_whitespace::Handler<impl AsciiStream>, ErrorHandler<F>> {
    reader::new(identical_whitespace::new(src), ErrorHandler::new(checker))
}
