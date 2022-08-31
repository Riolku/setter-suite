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
        while matches!(self.src.peek(), Some(c) if !c.is_ascii_whitespace()) {
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
        while matches!(self.src.peek(), Some(c) if *c != '\n') {
            line.push(self.src.next().unwrap());
        }
        if self.src.next().is_none() {
            Err(WRONG_WHITESPACE)
        } else {
            Ok(line)
        }
    }
    fn peek(&mut self) -> Option<&char> {
        self.src.peek()
    }
    fn next(&mut self) -> Option<char> {
        self.src.next()
    }
}

#[cfg(test)]
mod tests {
    #[test]
    fn basic() {
        let mut reader = new_test_reader("a  b \n cdef\n abc l m n o p\n");
        assert_eq!(reader.read_token(), "a");
        reader.expect_space();
        reader.expect_space();
        assert_eq!(reader.read_token(), "b");
        reader.expect_space();
        reader.expect_newline();

        reader.expect_space();
        assert_eq!(reader.read_token(), "cdef");
        reader.expect_newline();

        reader.expect_space();
        assert_eq!(reader.read_token(), "abc");
        reader.expect_space();
        assert_eq!(reader.read_line(), "l m n o p");

        drop(reader);
    }

    #[test]
    #[should_panic(expected = "wrong whitespace")]
    fn panics_when_has_unread_data() {
        let reader = new_test_reader(" ");
        drop(reader);
    }

    #[test]
    #[should_panic(expected = "wrong whitespace")]
    fn panics_when_whitespace_is_wrong() {
        let mut reader = new_test_reader("\n");
        reader.expect_space();

        // necessary because otherwise reader's destructor might panic, passing the test.
        panic!("Should have died");
    }

    #[test]
    #[should_panic(expected = "wrong whitespace")]
    fn panics_when_extra_whitespace_before_token() {
        let mut reader = new_test_reader(" a");
        reader.read_token();

        // necessary because otherwise reader's destructor might panic, passing the test.
        panic!("Should have died");
    }

    #[test]
    #[should_panic(expected = "wrong whitespace")]
    fn panics_when_no_newline_on_readline() {
        let mut reader = new_test_reader("abc woo");
        reader.read_line();

        // necessary because otherwise reader's destructor might panic, passing the test.
        panic!("Should have died");
    }

    use super::*;
    use crate::{panic_error_handler, reader::ErrorHandler as _, reader::Reader};
    fn new_test_reader(
        contents: &str,
    ) -> Reader<Handler<impl AsciiStream + '_>, panic_error_handler::Handler> {
        reader::new(
            new(contents.as_bytes()),
            panic_error_handler::Handler::new(),
        )
    }
}
