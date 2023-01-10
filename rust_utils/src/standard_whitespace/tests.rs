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
    assert!(reader.next_token_on_line().map_or(false, |s| s == "3"));
    assert!(reader.next_token_on_line().map_or(false, |s| s == "4"));
    assert!(reader.next_token_on_line().is_none());
}

#[test]
fn double_newline_ok() {
    let mut reader = test_reader("0\n\n");
    assert_eq!(reader.read_token(), "0");
    reader.expect_newline();
    reader.expect_newline();
}

#[test]
fn multiple_whitespace_means_largest() {
    // If the reference file has a newline and then a space, the user file must have a newline,
    // but need not have a space.
    let mut reader = test_reader("a\nb");
    assert_eq!(reader.read_token(), "a");
    reader.expect_newline();
    reader.expect_space();
    assert_eq!(reader.read_token(), "b");
}

#[test]
#[should_panic(expected = "wrong whitespace")]
fn multiple_whitespace_means_largest_panic() {
    // If the reference file has a newline and then a space, the user file must have a newline.
    let mut reader = test_reader("a b");
    assert_eq!(reader.read_token(), "a");
    reader.expect_newline();
    reader.expect_space();
    assert_eq!(reader.read_token(), "b");
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
