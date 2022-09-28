use super::checker::{codes, Checker};
use super::identical_whitespace;
use super::reader::{self, AsciiStream, Reader};
use std::io::BufRead;

pub struct ErrorHandler<'a, F> {
    checker: &'a Checker<F>,
}

impl<'a, F> ErrorHandler<'a, F>
where
    F: FnOnce(),
{
    pub fn new(checker: &'a Checker<F>) -> Self {
        Self { checker }
    }
}

impl<'a, F> reader::ErrorHandler for ErrorHandler<'a, F>
where
    F: FnOnce(),
{
    fn wrong_whitespace(&self) -> ! {
        self.checker.exit(codes::WA);
    }
    fn out_of_range(&self) -> ! {
        self.checker.exit(codes::WA);
    }
    fn parse_error(&self) -> ! {
        self.checker.exit(codes::WA);
    }
}

pub fn new<'a, F: FnOnce()>(
    src: impl BufRead,
    checker: &'a Checker<F>,
) -> Reader<identical_whitespace::Handler<impl AsciiStream>, ErrorHandler<'a, F>> {
    reader::new(identical_whitespace::new(src), ErrorHandler::new(checker))
}
