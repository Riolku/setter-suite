pub trait CollectIntoVec {
    type Item;
    fn collect_vec(self) -> Vec<Self::Item>;
}
impl<I> CollectIntoVec for I
where
    I: Iterator,
{
    type Item = I::Item;
    fn collect_vec(self) -> Vec<Self::Item> {
        self.collect()
    }
}

pub trait RandomArrayExtension {
    fn binary_array(&mut self, n: usize, ones: usize) -> Vec<bool>;
    fn array_with_sum(&mut self, n: usize, sum: usize) -> Vec<usize>;
    fn build_array<I, T>(&mut self, builder: I) -> Vec<T>
    where
        I: IntoIterator<Item = (usize, T, T)>,
        T: SampleUniform;
    // Get n distinct numbers in [0, upper)
    fn choose_multiple_large_range(&mut self, n: usize, upper: usize) -> Vec<usize>;
}

use rand::distributions::uniform::SampleUniform;
use rand::distributions::Uniform;
use rand::seq::IteratorRandom;
use rand::Rng;
impl<R> RandomArrayExtension for R
where
    R: Rng,
{
    fn binary_array(&mut self, n: usize, ones: usize) -> Vec<bool> {
        assert!(ones <= n);
        let mut ans = vec![false; n];
        for i in (0..n).choose_multiple(self, ones) {
            ans[i] = true;
        }
        debug_assert!(ans.iter().filter(|&x| *x).count() == ones);
        ans
    }
    fn array_with_sum(&mut self, n: usize, sum: usize) -> Vec<usize> {
        if n == 0 {
            assert!(sum == 0);
            return vec![];
        }
        let mut ans = Vec::with_capacity(n + 1);
        ans.push(sum);
        let distro = Uniform::new_inclusive(0, sum);
        ans.extend(self.sample_iter(distro).take(n - 1));
        ans[1..n].sort_unstable_by(|x, y| Ord::cmp(y, x));
        for i in 0..n - 1 {
            ans[i] -= ans[i + 1];
        }
        debug_assert!(ans.iter().sum::<usize>() == sum);
        ans
    }
    fn build_array<I, T>(&mut self, builder: I) -> Vec<T>
    where
        I: IntoIterator<Item = (usize, T, T)>,
        T: SampleUniform,
    {
        let mut ans = Vec::new();
        for (c, l, r) in builder {
            let distro = Uniform::new_inclusive(l, r);
            ans.extend(self.sample_iter(distro).take(c));
        }
        ans
    }
    fn choose_multiple_large_range(&mut self, n: usize, upper: usize) -> Vec<usize> {
        rand::seq::index::sample(self, upper, n).into_vec()
    }
}

#[cfg(test)]
mod tests;
