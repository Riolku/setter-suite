use super::reader::{
    self, AsciiStream, StandardWhitespace, Tokenizer, TokenizerResult, WRONG_WHITESPACE,
};
use std::io::BufRead;

#[derive(Eq, PartialEq)]
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
        flag: WhitespaceFlag::All,
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
    fn skip_non_line_whitespace(&mut self) {
        while matches!(self.src.peek(), Some(c) if Self::is_non_line_whitespace(*c)) {
            self.src.next();
        }
    }
    fn raw_read_token(&mut self) -> String {
        let mut token = String::new();
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
                    Some(c) if Self::is_non_line_whitespace(c) => {}
                    Some(c) => {
                        assert!(
                            c.is_ascii_whitespace(),
                            "Found non-whitespace right after token!"
                        );
                        return Err(WRONG_WHITESPACE);
                    }
                    None => return Err(WRONG_WHITESPACE),
                }

                self.skip_non_line_whitespace();
            }
            WhitespaceFlag::Newline => {
                match self.src.peek() {
                    Some(c) if c.is_ascii_whitespace() => {}
                    _ => {
                        panic!("Found non-whitespace right after token!");
                    }
                }

                if !Self::is_line_whitespace(self.src.next().unwrap())
                    & !self.has_line_before_next_token()
                {
                    return Err(WRONG_WHITESPACE);
                }
            }
            WhitespaceFlag::All => {
                while matches!(self.src.peek(), Some(c) if c.is_ascii_whitespace()) {
                    self.src.next();
                }
            }
        };
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

impl<S> Tokenizer for Handler<S>
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

impl<S> StandardWhitespace for Handler<S>
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
mod tests {
    #[test]
    fn basic() {
        let mut reader = test_reader("a \tb \n cdef\n");
        assert_eq!(reader.read_token(), "a");
        reader.expect_space();
        assert_eq!(reader.read_token(), "b");
        reader.expect_newline();

        assert_eq!(reader.read_token(), "cdef");

        drop(reader);
    }

    #[test]
    fn skips_all_initial_whitespace() {
        let mut reader = test_reader("\n\n\t k");
        assert_eq!(reader.read_token(), "k");

        drop(reader);
    }

    #[test]
    fn skips_all_trailing_whitespace() {
        let mut reader = test_reader("k\n\n \t\n  ");
        assert_eq!(reader.read_token(), "k");

        drop(reader);
    }

    #[test]
    fn skips_all_whitespace_on_newline() {
        let mut reader = test_reader("a\n \n\n\r\nb");
        assert_eq!(reader.read_token(), "a");
        reader.expect_newline();
        assert_eq!(reader.read_token(), "b");

        drop(reader);
    }

    #[test]
    #[should_panic(expected = "wrong whitespace")]
    fn panics_if_no_newline() {
        let mut reader = test_reader("a b");
        assert_eq!(reader.read_token(), "a");
        reader.expect_newline();
        assert_eq!(reader.read_token(), "b");
        // necessary because otherwise reader's destructor will panic, passing the test.
        panic!("Should have died");
    }

    #[test]
    #[should_panic(expected = "wrong whitespace")]
    fn panics_if_no_whitespace_before_newline() {
        let mut reader = test_reader("a\nb");
        assert_eq!(reader.read_token(), "a");
        reader.expect_space();
        assert_eq!(reader.read_token(), "b");
        // necessary because otherwise reader's destructor will panic, passing the test.
        panic!("Should have died");
    }

    #[test]
    #[should_panic(expected = "Do not call two whitespace methods in a row")]
    fn panics_if_two_whitespace_methods_are_called() {
        let mut reader = test_reader("a \n");
        assert_eq!(reader.read_token(), "a");
        reader.expect_space();
        reader.expect_newline();
    }

    #[test]
    fn does_not_panic_calling_newline_before_end() {
        let mut reader = test_reader("a\n");
        assert_eq!(reader.read_token(), "a");
        reader.expect_newline();
        drop(reader);
    }

    #[test]
    fn can_get_all_tokens_on_line() {
        let mut reader = test_reader("1  2 3 4 \n");
        assert_eq!(reader.read_token(), "1");
        reader.expect_space();
        assert_eq!(reader.read_token(), "2");
        assert_eq!(reader.next_token_on_line(), Some("3".to_string()));
        assert_eq!(reader.next_token_on_line(), Some("4".to_string()));
        assert_eq!(reader.next_token_on_line(), None);
    }

    use super::*;
    use crate::{panic_error_handler, reader::ErrorHandler as _, reader::Reader};
    fn test_reader(
        contents: &str,
    ) -> Reader<Handler<impl AsciiStream + '_>, panic_error_handler::Handler> {
        reader::new(
            new(contents.as_bytes()),
            panic_error_handler::Handler::new(),
        )
    }
}
