use super::super::{identical_whitespace, panic_error_handler};
use super::*;

#[test]
fn test_read_sep() {
    let mut reader = new_test_reader("a 1 2 3 4.5 bcde\n-1 -5\n6\n");

    let (a, one, two, three, four_point_five, bcde) =
        read_sep_without_range!(reader, String, usize, u64, i32, f32, String);
    assert_eq!(a, "a");
    assert_eq!(one, 1);
    assert_eq!(two, 2);
    assert_eq!(three, 3);
    assert!(f32::abs(four_point_five - 4.5) < 1e-9);
    assert_eq!(bcde, "bcde");

    let (negative_one, negative_five) = read_sep!(reader, -6..0, i8, isize);
    assert_eq!(negative_one, -1);
    assert_eq!(negative_five, -5);

    let _ = read_sep!(reader, 6..=6, u32);
}

#[test]
#[should_panic(expected = "parse error")]
fn test_read_sep_parse() {
    let mut reader = new_test_reader("1 a\n");
    let _ = read_sep!(reader, 1..=2, usize, usize);

    // necessary because otherwise reader's destructor will panic, passing the test.
    panic!("Should have died");
}

#[test]
#[should_panic(expected = "out of range")]
fn test_read_sep_range() {
    let mut reader = new_test_reader("1 2 3\n");
    let _ = read_sep!(reader, 1..=2, usize, i32, u8);

    // necessary because otherwise reader's destructor will panic, passing the test.
    panic!("Should have died");
}

#[test]
fn test_read_array() {
    let mut reader = new_test_reader("1 2 3 4\n1\n\n");
    let v = read_array!(reader, 4, usize, 1..=5);
    assert_eq!(v, vec![1, 2, 3, 4]);
    let v = read_array!(reader, 1, usize, 1..=5);
    assert_eq!(v, vec![1]);
    let v = read_array!(reader, 0, usize, 1..=5);
    assert_eq!(v, vec![]);
}

#[test]
#[should_panic(expected = "out of range")]
fn test_read_array_range() {
    let mut reader = new_test_reader("1 2 3\n");
    let _ = read_array!(reader, 3, usize, 1..=2);

    // necessary because otherwise reader's destructor will panic, passing the test.
    panic!("Should have died");
}

fn new_test_reader(
    contents: &str,
) -> Reader<identical_whitespace::Tokenizer<impl AsciiStream + '_>, panic_error_handler::Handler> {
    new(
        identical_whitespace::new(FullAsciiStream::new(contents.as_bytes())),
        panic_error_handler::Handler::new(),
    )
}
