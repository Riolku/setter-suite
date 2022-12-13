use super::reader::{BufferedAsciiStream, ErrorHandler, FullAsciiStream, Reader, Tokenizer};
use super::validating_reader;
use std::io;

pub fn entry() -> (Reader<impl Tokenizer, impl ErrorHandler>, impl io::Write) {
    (
        validating_reader::new(BufferedAsciiStream::new(io::stdin().lock(), 65536)),
        io::BufWriter::with_capacity(65536, io::stdout().lock()),
    )
}
