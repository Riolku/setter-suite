use std::cell::Cell;

pub mod codes {
    pub const AC: i32 = 0;
    pub const WA: i32 = 1;
    pub const PE: i32 = 2;
    pub const IE: i32 = 3;
    pub const PARTIAL: i32 = 7;
}

pub struct Checker<F> {
    // Realistically, this doesn't _need_ to be a Cell.
    // However, to call the function, we need to take the function.
    // This design is most user-friendly.
    // We could also use mutable receivers, but that's less user friendly.
    pre_error: Cell<Option<F>>,
}

impl<F> Checker<F>
where
    F: FnOnce(),
{
    pub fn new(pre_error: F) -> Self {
        Self {
            pre_error: Cell::new(Some(pre_error)),
        }
    }

    pub fn assert_or_code(&self, cond: bool, code: i32) {
        if !cond {
            self.exit(code);
        }
    }

    pub fn assert_or_wa(&self, cond: bool) {
        self.assert_or_code(cond, codes::WA);
    }

    pub fn assert_or_pe(&self, cond: bool) {
        self.assert_or_code(cond, codes::PE);
    }

    pub fn exit(&self, code: i32) -> ! {
        (self.pre_error.replace(None).unwrap())();
        std::process::exit(code);
    }
}
