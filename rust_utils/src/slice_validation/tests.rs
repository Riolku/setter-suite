use super::*;

#[test]
fn test_is_permutation() {
    assert!([1, 2, 3].is_permutation());
    assert!(![1, 2, 2].is_permutation());
    assert!(![1, 2, 6].is_permutation());
    assert!(![0, 1, 2].is_permutation());
}

#[test]
fn test_is_adjacent_distinct() {
    assert!([1, 3, 1].is_adjacent_distinct());
    assert!([Some(3), Some(2), None].is_adjacent_distinct());
    assert!(["abc", "bcd"].is_adjacent_distinct());
    assert!(!["abc", "abc"].is_adjacent_distinct());
    assert!(![1, 1, 3].is_adjacent_distinct());
}

#[test]
fn test_is_distinct() {
    assert!(![1, 3, 1].is_distinct());
    assert!([Some(3), Some(2), None].is_distinct());
    assert!(["abc", "bcd"].is_distinct());
    assert!(!["abc", "abc"].is_distinct());
    assert!(![1, 1, 3].is_distinct());
}
