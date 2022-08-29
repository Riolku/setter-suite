use super::reader::{self, AsciiStream, Tokenizer, TokenizerResult, WRONG_WHITESPACE};
use std::cmp::max;
use std::io::BufRead;

#[derive(Clone, Copy, Eq, Ord, PartialEq, PartialOrd)]
enum WhitespaceFlag {
    NoWhitespace,
    Space,
    Newline,
    All,
}

pub struct Handler<S: AsciiStream> {
    src: S,
    flag: WhitespaceFlag,
}
pub fn new(src: impl BufRead) -> Handler<impl AsciiStream> {
    Handler {
        src: reader::to_ascii_stream(src),
        flag: WhitespaceFlag::NoWhitespace,
    }
}

impl<S> Handler<S>
where
    S: AsciiStream,
{
    fn is_non_line_whitespace(c: char) -> bool {
        c.is_ascii_whitespace() && !Self::is_line_whitespace(c)
    }
    fn is_line_whitespace(c: char) -> bool {
        c == '\n' || c == '\r'
    }
    fn consume_flag(&mut self) -> TokenizerResult<()> {
        match self.flag {
            WhitespaceFlag::NoWhitespace => {}
            WhitespaceFlag::Space => {
                if !self.src.next().map_or(false, Self::is_non_line_whitespace) {
                    return Err(WRONG_WHITESPACE);
                }

                while self
                    .src
                    .peek()
                    .map_or(false, |c| Self::is_non_line_whitespace(*c))
                {
                    self.src.next();
                }
            }
            WhitespaceFlag::Newline => {
                if !self.src.peek().map_or(false, char::is_ascii_whitespace) {
                    return Err(WRONG_WHITESPACE);
                }

                let mut any_line = Self::is_line_whitespace(self.src.next().unwrap());
                while self.src.peek().map_or(false, char::is_ascii_whitespace) {
                    if Self::is_line_whitespace(self.src.next().unwrap()) {
                        any_line = true;
                    }
                }

                if !any_line {
                    return Err(WRONG_WHITESPACE);
                }
            }
            WhitespaceFlag::All => {
                while self.src.peek().map_or(false, char::is_ascii_whitespace) {
                    self.src.next();
                }
            }
        };
        self.flag = WhitespaceFlag::NoWhitespace;
        Ok(())
    }
    fn poke_flag(&mut self, flag: WhitespaceFlag) {
        self.flag = max(self.flag, flag);
    }
}

impl<S> Tokenizer for Handler<S>
where
    S: AsciiStream,
{
    fn init(&mut self) -> TokenizerResult<()> {
        self.poke_flag(WhitespaceFlag::All);
        self.consume_flag()
    }
    fn expect_space(&mut self) -> TokenizerResult<()> {
        self.poke_flag(WhitespaceFlag::Space);
        Ok(())
    }
    fn expect_newline(&mut self) -> TokenizerResult<()> {
        self.poke_flag(WhitespaceFlag::Newline);
        Ok(())
    }
    fn expect_eof(&mut self) -> TokenizerResult<()> {
        self.poke_flag(WhitespaceFlag::All);
        self.consume_flag()?;
        if self.src.next().is_some() {
            Err(WRONG_WHITESPACE)
        } else {
            Ok(())
        }
    }
    fn read_token(&mut self) -> TokenizerResult<String> {
        self.consume_flag()?;
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
