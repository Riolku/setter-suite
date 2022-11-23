#[macro_export]
macro_rules! print_many {
    ($wr:expr, $first:expr $(,$rest:expr)*) => {
        write!($wr, "{}", $first).unwrap();

        $(
            write!($wr, " {}", $rest).unwrap();
        )*

        write!($wr, "\n").unwrap();
    };
    ($wr:expr) => {
        write!($wr, "\n").unwrap();
    };
}

#[macro_export]
macro_rules! print_iter {
    ($wr:expr, $iter:expr) => {
        let mut iter = $iter.into_iter();
        if let Some(first) = iter.next() {
            write!($wr, "{}", first).unwrap();
        }

        for element in iter {
            write!($wr, " {}", element).unwrap();
        }

        write!($wr, "\n").unwrap();
    };
}

#[cfg(test)]
mod tests;
