use super::reader::{self, AsciiStream, ErrorHandler, Reader};
use super::{identical_whitespace, panic_error_handler};
use std::io::BufRead;

fn new(
    src: impl BufRead,
) -> Reader<identical_whitespace::Handler<impl AsciiStream>, panic_error_handler::Handler> {
    reader::new(
        identical_whitespace::new(src),
        panic_error_handler::Handler::new(),
    )
}

#[cfg(test)]
mod tests {
    #[test]
    fn basic() {
        let mut reader = super::new("a  b \n cdef\n abc l m n o p\n".as_bytes());
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
    #[should_panic]
    fn panics_when_has_unread_data() {
        let reader = super::new(" ".as_bytes());
        drop(reader);
    }

    #[test]
    #[should_panic]
    fn panics_when_whitespace_is_wrong() {
        let mut reader = super::new("\n".as_bytes());
        reader.expect_space();
    }

    #[test]
    #[should_panic]
    fn panics_when_extra_whitespace_before_token() {
        let mut reader = super::new(" a".as_bytes());
        reader.read_token();
    }

    #[test]
    #[should_panic]
    fn panics_when_no_newline_on_readline() {
        let mut reader = super::new("abc woo".as_bytes());
        reader.read_line();
    }
}
