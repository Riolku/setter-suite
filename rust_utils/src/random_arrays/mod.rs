pub trait CollectIntoVec {
    type Item;
    fn collect_vec(self) -> Vec<Self::Item>;
}
impl<I> CollectIntoVec for I where I: Iterator {
    type Item = I::Item;
    fn collect_vec(self) -> Vec<Self::Item> {
        self.collect()
    }
}

pub trait RandomArrayExtension {
    fn binary_array(&mut self, n: usize, ones: usize) -> Vec<bool>;
    fn array_with_sum(&mut self, n: usize, sum: usize) -> Vec<usize>;
}

use rand::Rng;
use rand::distributions::Uniform;
use rand::seq::IteratorRandom;
impl<R> RandomArrayExtension for R where R: Rng {
    fn binary_array(&mut self, n: usize, ones: usize) -> Vec<bool> {
        assert!(ones <= n);
        let mut ans = vec![false; n];
        for i in (0..n).choose_multiple(self, ones) {
            ans[i] = true;
        }
        debug_assert!(ans.iter().filter(|x| **x).count() == ones);
        ans
    }
    fn array_with_sum(&mut self, n: usize, sum: usize) -> Vec<usize> {
        assert!(n > 0);
        let mut ans = Vec::with_capacity(n + 1);
        ans.push(sum);
        let distro = Uniform::new_inclusive(0, sum);
        ans.extend(self.sample_iter(distro).take(n - 1));
        ans[1..n].sort_unstable_by(|x, y| Ord::cmp(y, x));
        ans.push(0);
        for i in 0..n {
            ans[i] -= ans[i + 1];
        }
        ans.pop();
        debug_assert!(ans.iter().sum::<usize>() == sum);
        ans
    }
}

#[cfg(test)]
mod tests;
