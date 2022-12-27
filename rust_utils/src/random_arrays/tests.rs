use super::*;
use super::super::test_cases::ConcreteRng;

fn get_gen() -> ConcreteRng {
    use rand::SeedableRng;
    ConcreteRng::from_entropy()
}

#[test]
fn test_binary_array() {
    let mut gen = get_gen();
    for _ in 0..10 {
        let n = gen.gen_range(0..=20);
        let k = gen.gen_range(0..=n);
        let arr = gen.binary_array(n, k);
        assert!(arr.iter().filter(|&x| *x).count() == k);
    }
}

#[test]
fn test_array_with_sum() {
    let mut gen = get_gen();
    for _ in 0..10 {
        let n = gen.gen_range(0..=20);
        let sum = gen.gen_range(0..=1000);
        let arr = gen.array_with_sum(n, sum);
        assert!(arr.iter().sum::<usize>() == sum);
    }
}
