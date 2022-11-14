use std::cell::Cell;
use std::rc::Rc;

pub mod codes {
    pub const AC: i32 = 0;
    pub const WA: i32 = 1;
    pub const PE: i32 = 2;
    pub const IE: i32 = 3;
    pub const PARTIAL: i32 = 7;
}

pub struct Checker<F> {
    pre_error: Rc<Cell<Option<F>>>,
}

impl<F> Clone for Checker<F> {
    fn clone(&self) -> Self {
        Self {
            pre_error: self.pre_error.clone(),
        }
    }
}

impl<F> Checker<F>
where
    F: FnOnce(),
{
    pub fn new(pre_error: F) -> Self {
        Self {
            pre_error: Rc::new(Cell::new(Some(pre_error))),
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
