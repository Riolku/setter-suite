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
            Some(' ') => Ok(()),
            _ => wrong_whitespace(),
        }
    }
    fn expect_newline(&mut self) -> TokenizerResult<()> {
        match self.src.next() {
            Some('\n') => Ok(()),
            _ => wrong_whitespace(),
        }
    }
    fn expect_eof(&mut self) -> TokenizerResult<()> {
        match self.src.next() {
            None => Ok(()),
            _ => wrong_whitespace(),
        }
    }
    fn read_token(&mut self) -> TokenizerResult<String> {
        match self.src.next() {
            Some(mut c) if c.is_ascii_graphic() => {
                let mut token = String::with_capacity(32);
                loop {
                    token.push(c);
                    if let Some(next_char) = self.src.next_if(char::is_ascii_graphic) {
                        c = next_char;
                    }
                    else {
                        break Ok(token);
                    }
                }
            }
            _ => wrong_whitespace(),
        }
    }
}
#[cfg(test)]
mod tests;
