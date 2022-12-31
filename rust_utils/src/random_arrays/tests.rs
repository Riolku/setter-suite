use super::super::test_cases::ConcreteRng;
use super::*;

fn get_gen() -> ConcreteRng {
    use rand::SeedableRng;
    ConcreteRng::from_entropy()
}

#[test]
fn test_binary_array() {
    let mut gen = get_gen();
    for n in 0..=20 {
        let k = gen.gen_range(0..=n);
        let arr = gen.binary_array(n, k);
        assert!(arr.iter().filter(|&x| *x).count() == k);
    }
}

#[test]
fn test_array_with_sum() {
    let mut gen = get_gen();
    for n in 1..=20 {
        let sum = gen.gen_range(0..=1000);
        let arr = gen.array_with_sum(n, sum);
        assert!(arr.iter().sum::<usize>() == sum);
    }
}

#[test]
fn test_build_array() {
    let mut gen = get_gen();
    let arr = gen.build_array([(1, 1, 2), (5, 3, 5)]);
    assert!(arr[0] >= 1 && arr[0] <= 2);
    assert!(arr[1..].iter().all(|x| (3..=5).contains(x)));

    let arr = gen.build_array(Vec::from([(1, 1, 2), (5, 3, 5)]));
    assert!(arr[0] >= 1 && arr[0] <= 2);
    assert!(arr[1..].iter().all(|x| (3..=5).contains(x)));
}

#[test]
fn test_choose_multiple_large_range() {
    let mut gen = get_gen();
    for n in 1..=20 {
        for upper in [n, n + 1, n + 2, n + 3, 1000, 1_000_000, 1_000_000_000_000_000] {
            let mut arr = gen.choose_multiple_large_range(n, upper);
            assert!(arr.iter().all(|x| (0..upper).contains(x)));
            arr.sort_unstable();
            assert!(std::iter::zip(arr[1..].iter(), arr.iter()).all(|(x, y)| *x != *y));
        }
    }
}
