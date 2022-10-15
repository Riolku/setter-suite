use super::reader::{ErrorHandler, Reader, Tokenizer};
use super::validating_reader;
use std::io;

pub fn entry() -> (Reader<impl Tokenizer, impl ErrorHandler>, impl io::Write) {
    (
        validating_reader::new(io::BufReader::with_capacity(1048576, io::stdin().lock())),
        io::BufWriter::with_capacity(1048576, io::stdout().lock()),
    )
}
