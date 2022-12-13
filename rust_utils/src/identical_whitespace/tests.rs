#[test]
fn basic() {
    let mut reader = new_test_reader("a  b \n cdef\n abc\n");
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
    reader.expect_newline();

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

use super::super::{panic_error_handler, reader::Reader};
use super::*;
fn new_test_reader(
    contents: &str,
) -> Reader<Tokenizer<impl AsciiStream + '_>, panic_error_handler::Handler> {
    reader::new(
        new(reader::FullAsciiStream::new(contents.as_bytes())),
        panic_error_handler::Handler::new(),
    )
}
