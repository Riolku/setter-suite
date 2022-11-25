use super::reader::{self, AsciiStream, StandardWhitespace, TokenizerResult, WRONG_WHITESPACE};
use std::io::BufRead;

#[derive(Debug, Eq, PartialEq)]
enum WhitespaceFlag {
    NoWhitespace,
    Space,
    Newline,
    All,
}

pub struct Tokenizer<S: AsciiStream> {
    src: S,
    flag: WhitespaceFlag,
}
pub fn new(src: impl BufRead) -> Tokenizer<impl AsciiStream> {
    Tokenizer {
        src: reader::to_ascii_stream(src),
        flag: WhitespaceFlag::All,
    }
}

impl<S> Tokenizer<S>
where
    S: AsciiStream,
{
    fn is_non_line_whitespace(c: char) -> bool {
        c.is_ascii_whitespace() && !Self::is_line_whitespace(c)
    }
    fn is_line_whitespace(c: char) -> bool {
        c == '\n' || c == '\r'
    }
    fn skip_non_line_whitespace(&mut self) {
        while matches!(self.src.peek(), Some(c) if Self::is_non_line_whitespace(*c)) {
            self.src.next();
        }
    }
    fn raw_read_token(&mut self) -> String {
        let mut token = String::with_capacity(32);
        while matches!(self.src.peek(), Some(c) if !c.is_ascii_whitespace()) {
            token.push(self.src.next().unwrap());
        }
        token
    }
    fn has_line_before_next_token(&mut self) -> bool {
        let mut any_line = false;
        while matches!(self.src.peek(), Some(c) if c.is_ascii_whitespace()) {
            if Self::is_line_whitespace(self.src.next().unwrap()) {
                any_line = true;
            }
        }
        any_line
    }

    fn consume_flag(&mut self) -> TokenizerResult<()> {
        match self.flag {
            WhitespaceFlag::NoWhitespace => {}
            WhitespaceFlag::Space => {
                match self.src.next() {
                    Some(c) => {
                        assert!(
                            c.is_ascii_whitespace(),
                            "Found non-whitespace right after token!"
                        );
                        if Self::is_line_whitespace(c) {
                            return Err(WRONG_WHITESPACE);
                        }
                        self.skip_non_line_whitespace();
                    }
                    None => return Err(WRONG_WHITESPACE),
                }
            }
            WhitespaceFlag::Newline => {
                match self.src.peek() {
                    Some(c) => {
                        assert!(c.is_ascii_whitespace(), "Found non-whitespace right after token!");
                        if !self.has_line_before_next_token() {
                            return Err(WRONG_WHITESPACE);
                        }
                    }
                    None => return Err(WRONG_WHITESPACE),
                }
            }
            WhitespaceFlag::All => {
                while matches!(self.src.peek(), Some(c) if c.is_ascii_whitespace()) {
                    self.src.next();
                }
            }
        }
        self.flag = WhitespaceFlag::NoWhitespace;
        Ok(())
    }
    fn poke_flag(&mut self, flag: WhitespaceFlag) {
        assert!(
            self.flag == WhitespaceFlag::NoWhitespace,
            "Do not call two whitespace methods in a row. \
            This error may also appear because you called a whitespace method immediately after initialization."
        );
        self.flag = flag;
    }
}

impl<S> reader::Tokenizer for Tokenizer<S>
where
    S: AsciiStream,
{
    fn expect_space(&mut self) -> TokenizerResult<()> {
        self.poke_flag(WhitespaceFlag::Space);
        Ok(())
    }
    fn expect_newline(&mut self) -> TokenizerResult<()> {
        self.poke_flag(WhitespaceFlag::Newline);
        Ok(())
    }
    fn expect_eof(&mut self) -> TokenizerResult<()> {
        self.flag = WhitespaceFlag::All;
        self.consume_flag()?;
        if self.src.next().is_some() {
            Err(WRONG_WHITESPACE)
        } else {
            Ok(())
        }
    }
    fn read_token(&mut self) -> TokenizerResult<String> {
        self.consume_flag()?;
        let token = self.raw_read_token();
        if token.len() == 0 {
            Err(WRONG_WHITESPACE)
        } else {
            Ok(token)
        }
    }
}

impl<S> StandardWhitespace for Tokenizer<S>
where
    S: AsciiStream,
{
    fn next_token_on_line(&mut self) -> Option<String> {
        assert!(
            self.flag == WhitespaceFlag::NoWhitespace,
            "You must not call `next_token_on_line` after any whitespace method."
        );

        self.skip_non_line_whitespace();

        let token = self.raw_read_token();
        if token.len() == 0 {
            None
        } else {
            Some(token)
        }
    }
    fn has_token_in_stream(&mut self) -> TokenizerResult<bool> {
        assert!(
            self.flag == WhitespaceFlag::Newline,
            "You must call `expect_newline` before `has_token_in_stream`."
        );

        let any_line = self.has_line_before_next_token();
        match self.src.peek() {
            Some(c) => {
                assert!(
                    !c.is_ascii_whitespace(),
                    "Failed to trim all whitespace from stream"
                );
                // Force a newline to appear if there are still tokens
                if !any_line {
                    Err(WRONG_WHITESPACE)
                } else {
                    Ok(true)
                }
            }
            None => Ok(false),
        }
    }
}

#[cfg(test)]
mod tests;
