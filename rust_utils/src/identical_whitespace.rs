use super::reader::{
    self, AsciiStream, ExactWhitespaceTokenizer, Tokenizer, TokenizerResult, WRONG_WHITESPACE,
};
use std::io::BufRead;

pub struct Handler<S: AsciiStream> {
    src: S,
}
pub fn new(src: impl BufRead) -> Handler<impl AsciiStream> {
    Handler {
        src: reader::to_ascii_stream(src),
    }
}

impl<S> Tokenizer for Handler<S>
where
    S: AsciiStream,
{
    fn init(&mut self) -> TokenizerResult<()> {
        Ok(())
    }
    fn expect_space(&mut self) -> TokenizerResult<()> {
        match self.src.next() {
            Some(' ') => Ok(()),
            _ => Err(WRONG_WHITESPACE),
        }
    }
    fn expect_newline(&mut self) -> TokenizerResult<()> {
        match self.src.next() {
            Some('\n') => Ok(()),
            _ => Err(WRONG_WHITESPACE),
        }
    }
    fn expect_eof(&mut self) -> TokenizerResult<()> {
        match self.src.next() {
            None => Ok(()),
            _ => Err(WRONG_WHITESPACE),
        }
    }
    fn read_token(&mut self) -> TokenizerResult<String> {
        let mut token = String::new();
        while self.src.peek().map_or(false, |c| !c.is_ascii_whitespace()) {
            token.push(self.src.next().unwrap());
        }
        if token.len() == 0 {
            Err(WRONG_WHITESPACE)
        } else {
            Ok(token)
        }
    }
}

impl<S> ExactWhitespaceTokenizer for Handler<S>
where
    S: AsciiStream,
{
    fn read_line(&mut self) -> TokenizerResult<String> {
        let mut line = String::new();
        while self.src.peek().map_or(false, |c| *c != '\n') {
            line.push(self.src.next().unwrap());
        }
        if self.src.next().is_none() {
            Err(WRONG_WHITESPACE)
        } else {
            Ok(line)
        }
    }
}
