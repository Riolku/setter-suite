use super::super::{reader, identical_whitespace, panic_error_handler};
use super::*;

#[test]
fn test_read_maybe_array() {
    let mut reader = new_test_reader("1 2 3 4\n-1\n-1\n");

    assert_eq!(read_maybe_array!(reader, 4, usize, 1..=4), Some(vec![1, 2, 3, 4]));
    assert_eq!(read_maybe_array!(reader, 100, usize, 1..100), None);
    assert_eq!(read_maybe_array!(reader, 1, usize, 1..100), None);
}

#[test]
fn test_read_maybe_single() {
    let mut reader = new_test_reader("1\n-1\n");

    assert_eq!(read_maybe_single!(reader, usize, 1..=1), Some(1));
    assert_eq!(read_maybe_single!(reader, usize, 1..=1), None);
}

fn new_test_reader(
    contents: &str,
) -> Reader<identical_whitespace::Tokenizer<impl reader::AsciiStream + '_>, panic_error_handler::Handler> {
    reader::new(
        identical_whitespace::new(contents.as_bytes()),
        panic_error_handler::Handler::new(),
    )
}
