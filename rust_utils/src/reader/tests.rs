use super::*;
use super::super::{identical_whitespace, panic_error_handler};

#[test]
fn test_read_sep() {
    let mut reader = new_test_reader("a 1 2 3 4.5 bcde\n-1 -5\n6\n");

    let (a, one, two, three, four_point_five, bcde) = read_sep!(reader, String, usize, u64, i32, f32, String);
    assert_eq!(a, "a");
    assert_eq!(one, 1);
    assert_eq!(two, 2);
    assert_eq!(three, 3);
    assert!(f32::abs(four_point_five - 4.5) < 1e-9);
    assert_eq!(bcde, "bcde");

    let (negative_one, negative_five) = read_sep!(reader, i8, isize);
    assert_eq!(negative_one, -1);
    assert_eq!(negative_five, -5);

    let (six,) = read_sep!(reader, u32);
    assert_eq!(six, 6);
}

fn new_test_reader(
    contents: &str,
) -> Reader<identical_whitespace::Handler<impl AsciiStream + '_>, panic_error_handler::Handler> {
    new(
        identical_whitespace::new(contents.as_bytes()),
        panic_error_handler::Handler::new(),
    )
}
