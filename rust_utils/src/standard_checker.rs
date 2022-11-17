use super::checker::{self, codes, Checker};
use super::reader::{self, AsciiStream, Reader, Tokenizer};
use super::standard_whitespace;
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
        self.checker.exit(codes::WA);
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
) -> Reader<standard_whitespace::Tokenizer<impl AsciiStream>, ErrorHandler<F>> {
    reader::new(standard_whitespace::new(src), ErrorHandler::new(checker))
}

pub fn entry<F: FnOnce()>(
    f: F,
) -> (
    Checker<F>,
    Reader<impl Tokenizer, impl reader::ErrorHandler>,
    Reader<standard_whitespace::Tokenizer<impl AsciiStream>, ErrorHandler<F>>,
    Reader<impl Tokenizer, impl reader::ErrorHandler>,
) {
    checker::entry(f, new_checker)
}
