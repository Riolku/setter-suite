use super::reader::{self, wrong_whitespace, AsciiStream, TokenizerResult};

pub struct Tokenizer<S> {
    src: S,
    token: String,
}
pub fn new<S: AsciiStream>(src: S) -> Tokenizer<S> {
    Tokenizer { src, token: String::new() }
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
    fn read_token(&mut self) -> TokenizerResult<&str> {
        match self.src.next() {
            Some(mut c) if c.is_ascii_graphic() => {
                self.token.clear();
                loop {
                    self.token.push(c);
                    match self.src.next_if(char::is_ascii_graphic) {
                        Some(next_char) => c = next_char,
                        None => break Ok(&*self.token),
                    }
                }
            }
            _ => wrong_whitespace(),
        }
    }
}
#[cfg(test)]
mod tests;
