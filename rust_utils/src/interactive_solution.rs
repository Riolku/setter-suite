use super::reader::{BufferedAsciiStream, ErrorHandler, Reader, Tokenizer};
use super::tied_streams::TiedStreams;
use super::validating_reader;
use std::io;

pub fn entry() -> TiedStreams<Reader<impl Tokenizer, impl ErrorHandler>, impl io::Write> {
    TiedStreams::new(
        validating_reader::new(BufferedAsciiStream::new(io::stdin().lock(), 4096)),
        io::BufWriter::with_capacity(4096, io::stdout().lock()),
    )
}
