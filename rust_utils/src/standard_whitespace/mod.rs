use super::reader::{self, wrong_whitespace, AsciiStream, StandardWhitespace, TokenizerResult};

#[derive(Debug, Eq, PartialEq)]
enum WhitespaceFlag {
    NoWhitespace,
    Space,
    Newline,
    All,
}

#[derive(Debug)]
pub struct Tokenizer<S: AsciiStream> {
    src: S,
    token: String,
    flag: WhitespaceFlag,
}
pub fn new<S: AsciiStream>(src: S) -> Tokenizer<S> {
    Tokenizer {
        src,
        token: String::new(),
        flag: WhitespaceFlag::All,
    }
}

impl<S> Tokenizer<S>
where
    S: AsciiStream,
{
    fn is_non_line_whitespace(c: char) -> bool {
        !c.is_ascii_graphic() && c != '\n'
    }
    fn skip_non_line_whitespace(&mut self) {
        while self
            .src
            .next_if(|c| Self::is_non_line_whitespace(*c))
            .is_some()
        {}
    }
    fn raw_read_token(&mut self) {
        self.token.clear();
        while let Some(c) = self.src.next_if(char::is_ascii_graphic) {
            self.token.push(c);
        }
    }
    fn has_line_before_next_token(&mut self) -> bool {
        let mut any_line = false;
        while let Some(c) = self.src.next_if(|c| !c.is_ascii_graphic()) {
            if c == '\n' {
                any_line = true;
            }
        }
        any_line
    }

    fn consume_flag(&mut self) -> TokenizerResult<()> {
        match self.flag {
            WhitespaceFlag::NoWhitespace => {}
            WhitespaceFlag::Space => match self.src.next() {
                Some(c) => {
                    assert!(
                        !c.is_ascii_graphic(),
                        "Found non-whitespace right after token!"
                    );
                    if c == '\n' {
                        return wrong_whitespace();
                    }
                    self.skip_non_line_whitespace();
                }
                None => return wrong_whitespace(),
            },
            WhitespaceFlag::Newline => match self.src.peek() {
                Some(c) => {
                    assert!(
                        !c.is_ascii_graphic(),
                        "Found non-whitespace right after token!"
                    );
                    if !self.has_line_before_next_token() {
                        return wrong_whitespace();
                    }
                }
                None => return wrong_whitespace(),
            },
            WhitespaceFlag::All => while self.src.next_if(|c| !c.is_ascii_graphic()).is_some() {},
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
            wrong_whitespace()
        } else {
            Ok(())
        }
    }
    fn read_token(&mut self) -> TokenizerResult<&str> {
        self.consume_flag()?;
        self.raw_read_token();
        if self.token.len() == 0 {
            wrong_whitespace()
        } else {
            Ok(&*self.token)
        }
    }
}

impl<S> StandardWhitespace for Tokenizer<S>
where
    S: AsciiStream,
{
    fn next_token_on_line(&mut self) -> Option<&str> {
        assert!(
            self.flag == WhitespaceFlag::NoWhitespace,
            "You must not call `next_token_on_line` after any whitespace method."
        );

        self.skip_non_line_whitespace();

        self.raw_read_token();
        if self.token.len() == 0 {
            None
        } else {
            Some(&*self.token)
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
                    wrong_whitespace()
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
