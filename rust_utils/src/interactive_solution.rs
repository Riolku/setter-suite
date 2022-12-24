use super::reader::{BufferedAsciiStream, ErrorHandler, Reader, Tokenizer};
use super::tied_streams::TiedStandardStreams;
use super::validating_reader;
use std::io;

pub fn entry() -> TiedStandardStreams<Reader<impl Tokenizer, impl ErrorHandler>, impl io::Write> {
    TiedStandardStreams::new(
        validating_reader::new(BufferedAsciiStream::new(io::stdin().lock(), 4096)),
        io::BufWriter::with_capacity(4096, io::stdout().lock()),
    )
}
