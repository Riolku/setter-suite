use super::reader::{self, AsciiStream, ErrorHandler, Reader};
use super::{identical_whitespace, panic_error_handler};
use std::io::BufRead;

pub fn new(
    src: impl BufRead,
) -> Reader<identical_whitespace::Handler<impl AsciiStream>, panic_error_handler::Handler> {
    reader::new(
        identical_whitespace::new(src),
        panic_error_handler::Handler::new(),
    )
}
