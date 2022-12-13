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
fn panics_if_no_whitespace_before_eof() {
    let mut reader = test_reader("2");
    assert_eq!(reader.read_token(), "2");
    reader.expect_newline();
    reader.read_token();
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

use super::super::{panic_error_handler, reader::Reader};
use super::*;
fn test_reader(
    contents: &str,
) -> Reader<Tokenizer<impl AsciiStream + '_>, panic_error_handler::Handler> {
    reader::new(
        new(reader::FullAsciiStream::new(contents.as_bytes())),
        panic_error_handler::Handler::new(),
    )
}
