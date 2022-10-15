use super::reader::{ErrorHandler, Reader, Tokenizer};
use super::tied_streams::TiedStreams;
use super::validating_reader;
use std::io;

pub fn entry() -> TiedStreams<Reader<impl Tokenizer, impl ErrorHandler>, impl io::Write> {
    TiedStreams::new(
        validating_reader::new(io::BufReader::with_capacity(4096, io::stdin().lock())),
        io::BufWriter::with_capacity(4096, io::stdout().lock()),
    )
}
