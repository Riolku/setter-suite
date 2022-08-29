pub struct ErrorHandler;

use super::checker::{codes, exit};

impl super::reader::ErrorHandler for ErrorHandler {
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

use super::reader::{self, AsciiStream, ErrorHandler as _, Reader};
use super::standard_whitespace;
use std::io::BufRead;

fn new(src: impl BufRead) -> Reader<standard_whitespace::Handler<impl AsciiStream>, ErrorHandler> {
    reader::new(standard_whitespace::new(src), ErrorHandler::new())
}

#[cfg(test)]
mod tests {
    #[test]
    fn basic() {
        let mut reader = test_reader("a \tb \n cdef\n");
        assert_eq!(reader.read_token(), "a");
        reader.expect_space();
        assert_eq!(reader.read_token(), "b");
        reader.expect_newline();

        assert_eq!(reader.read_token(), "cdef");

        drop(reader);
    }

    #[test]
    fn skips_all_initial_whitespace() {
        let mut reader = test_reader("\n\n\t k");
        assert_eq!(reader.read_token(), "k");

        drop(reader);
    }

    #[test]
    fn skips_all_trailing_whitespace() {
        let mut reader = test_reader("k\n\n \t\n  ");
        assert_eq!(reader.read_token(), "k");

        drop(reader);
    }

    #[test]
    fn skips_all_whitespace_on_newline() {
        let mut reader = test_reader("a\n \n\n\r\nb");
        assert_eq!(reader.read_token(), "a");
        reader.expect_newline();
        assert_eq!(reader.read_token(), "b");

        drop(reader);
    }

    #[test]
    #[should_panic]
    fn panics_if_no_newline() {
        let mut reader = test_reader("a b");
        reader.read_token();
        reader.expect_newline();
    }

    #[test]
    #[should_panic]
    fn panics_if_no_whitespace() {
        let mut reader = test_reader("a");
        reader.expect_space();
    }

    #[test]
    #[should_panic]
    fn panics_if_no_whitespace_before_newline() {
        let mut reader = test_reader("a\nb");
        reader.read_token();
        reader.expect_space();
        reader.read_token();
    }

    use super::{reader, standard_whitespace, AsciiStream, Reader};
    use crate::{panic_error_handler, reader::ErrorHandler as _};
    fn test_reader(
        contents: &str,
    ) -> Reader<standard_whitespace::Handler<impl AsciiStream + '_>, panic_error_handler::Handler>
    {
        reader::new(
            standard_whitespace::new(contents.as_bytes()),
            panic_error_handler::Handler::new(),
        )
    }
}
