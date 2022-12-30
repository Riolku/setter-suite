pub trait PermutationValidation {
    fn is_permutation(&self) -> bool;
}
impl PermutationValidation for [usize] {
    fn is_permutation(&self) -> bool {
        let n = self.len();
        let mut seen = vec![false; n];
        for x in self {
            if !(1..=n).contains(x) {
                return false;
            }
            if seen[x - 1] {
                return false;
            }
            seen[x - 1] = true;
        }
        true
    }
}
pub trait SliceValidation {
    fn is_adjacent_distinct(&self) -> bool;
    fn is_distinct(&self) -> bool;
}
impl<T> SliceValidation for [T]
where
    T: Clone + Ord,
{
    fn is_adjacent_distinct(&self) -> bool {
        for i in 1..self.len() {
            if self[i - 1] == self[i] {
                return false;
            }
        }
        return true;
    }
    fn is_distinct(&self) -> bool {
        let mut v = self.to_vec();
        v.sort_unstable();
        v.is_adjacent_distinct()
    }
}

#[cfg(test)]
mod tests;
