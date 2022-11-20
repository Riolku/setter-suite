use super::checker::{self, codes, Checker};
use super::identical_whitespace;
use super::reader::{self, AsciiStream, Reader, Tokenizer};
use std::io::BufRead;

pub struct ErrorHandler<F> {
    checker: Checker<F>,
}

impl<F> ErrorHandler<F>
where
    F: FnOnce() -> Option<i32>,
{
    pub fn new(checker: Checker<F>) -> Self {
        Self { checker }
    }
}

impl<F> reader::ErrorHandler for ErrorHandler<F>
where
    F: FnOnce() -> Option<i32>,
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

pub fn new_checker<F: FnOnce() -> Option<i32>>(
    src: impl BufRead,
    checker: Checker<F>,
) -> Reader<identical_whitespace::Tokenizer<impl AsciiStream>, ErrorHandler<F>> {
    reader::new(identical_whitespace::new(src), ErrorHandler::new(checker))
}

pub fn entry<F: FnOnce() -> Option<i32>>(
    f: F,
) -> (
    Checker<F>,
    Reader<impl Tokenizer, impl reader::ErrorHandler>,
    Reader<identical_whitespace::Tokenizer<impl AsciiStream>, ErrorHandler<F>>,
    Reader<impl Tokenizer, impl reader::ErrorHandler>,
) {
    checker::entry(f, new_checker)
}
