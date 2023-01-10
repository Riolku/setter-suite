use super::reader::{self, wrong_whitespace, AsciiStream, StandardWhitespace, TokenizerResult};

#[derive(Debug, Eq, PartialEq, Ord, PartialOrd)]
enum WhitespaceFlag {
    NoWhitespace,
    Space,
    Newline,
    All,
}

#[derive(Debug)]
pub struct Tokenizer<S: AsciiStream> {
    src: S,
    flag: WhitespaceFlag,
}
pub fn new<S: AsciiStream>(src: S) -> Tokenizer<S> {
    Tokenizer {
        src,
        flag: WhitespaceFlag::All,
    }
}

impl<S> Tokenizer<S>
where
    S: AsciiStream,
{
    fn is_non_line_whitespace(b: &u8) -> bool {
        !b.is_ascii_graphic() && *b != b'\n'
    }
    fn skip_non_line_whitespace(&mut self) {
        while self.src.next_if(Self::is_non_line_whitespace).is_some() {}
    }
    fn has_line_before_next_token(&mut self) -> bool {
        let mut any_line = false;
        while let Some(b) = self.src.next_if(|b| !b.is_ascii_graphic()) {
            if b == b'\n' {
                any_line = true;
            }
        }
        any_line
    }
    fn consume_flag(&mut self) -> TokenizerResult<()> {
        match self.flag {
            WhitespaceFlag::NoWhitespace => {}
            WhitespaceFlag::Space => match self.src.next() {
                Some(b) => {
                    assert!(
                        !b.is_ascii_graphic(),
                        "Found non-whitespace right after token!"
                    );
                    if b == b'\n' {
                        return wrong_whitespace();
                    }
                    self.skip_non_line_whitespace();
                }
                None => return wrong_whitespace(),
            },
            WhitespaceFlag::Newline => match self.src.peek() {
                Some(b) => {
                    assert!(
                        !b.is_ascii_graphic(),
                        "Found non-whitespace {} right after token!",
                        b
                    );
                    if !self.has_line_before_next_token() {
                        return wrong_whitespace();
                    }
                }
                None => return wrong_whitespace(),
            },
            WhitespaceFlag::All => while self.src.next_if(|b| !b.is_ascii_graphic()).is_some() {},
        }
        self.flag = WhitespaceFlag::NoWhitespace;
        Ok(())
    }
    fn poke_flag(&mut self, flag: WhitespaceFlag) {
        if flag > self.flag {
            self.flag = flag;
        }
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
        self.poke_flag(WhitespaceFlag::All);
        self.consume_flag()?;
        if self.src.next().is_some() {
            wrong_whitespace()
        } else {
            Ok(())
        }
    }
    fn read_token(&mut self) -> TokenizerResult<&str> {
        self.consume_flag()?;
        let token_bytes = self.src.read_while(u8::is_ascii_graphic);
        if token_bytes.len() == 0 {
            wrong_whitespace()
        } else {
            // Safety: all the bytes in the token are ascii.
            Ok(unsafe { std::str::from_utf8_unchecked(token_bytes) })
        }
    }
}

impl<S> StandardWhitespace for Tokenizer<S>
where
    S: AsciiStream,
{
    fn next_token_on_line(&mut self) -> Option<&[u8]> {
        assert!(
            self.flag == WhitespaceFlag::NoWhitespace,
            "You must not call `next_token_on_line` after any whitespace method."
        );

        self.skip_non_line_whitespace();

        let token_bytes = self.src.read_while(u8::is_ascii_graphic);
        if token_bytes.len() == 0 {
            None
        } else {
            Some(token_bytes)
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
