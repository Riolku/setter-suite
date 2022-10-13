use std::fmt::Write;
use std::iter;

#[test]
fn test_print_many() {
    let mut buf = String::new();

    print_many!(buf, "ab", 1, 2);
    print_many!(buf);

    assert_eq!(buf, "ab 1 2\n\n");
}

#[test]
fn test_print_iter() {
    let mut buf = String::new();

    print_iter!(buf, vec![1, 2, 3]);
    print_iter!(buf, Vec::<i32>::new());
    print_iter!(buf, iter::once(4).chain(iter::once(8)));

    assert_eq!(buf, "1 2 3\n\n4 8\n");
}
