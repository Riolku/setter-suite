use super::reader::{self, wrong_whitespace, AsciiStream, TokenizerResult};

pub struct Tokenizer<S> {
    src: S,
}
pub fn new<S: AsciiStream>(src: S) -> Tokenizer<S> {
    Tokenizer { src }
}

impl<S> reader::Tokenizer for Tokenizer<S>
where
    S: AsciiStream,
{
    fn expect_space(&mut self) -> TokenizerResult<()> {
        match self.src.next() {
            Some(b' ') => Ok(()),
            _ => wrong_whitespace(),
        }
    }
    fn expect_newline(&mut self) -> TokenizerResult<()> {
        match self.src.next() {
            Some(b'\n') => Ok(()),
            _ => wrong_whitespace(),
        }
    }
    fn expect_eof(&mut self) -> TokenizerResult<()> {
        match self.src.next() {
            None => Ok(()),
            _ => wrong_whitespace(),
        }
    }
    fn read_token(&mut self) -> TokenizerResult<&[u8]> {
        let bytes = self.src.read_while(u8::is_ascii_graphic);
        if bytes.len() == 0 {
            wrong_whitespace()
        } else {
            Ok(bytes)
        }
    }
}
#[cfg(test)]
mod tests;
