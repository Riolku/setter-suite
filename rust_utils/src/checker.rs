use super::reader::{ErrorHandler, Reader, Tokenizer};
use super::validating_reader;

use std::cell::Cell;
use std::env;
use std::fs::File;
use std::io::BufReader;
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

pub fn entry<Constructor, ErrorFunc, HandlerType, TokenizerType>(
    f: ErrorFunc,
    reader_constructor: Constructor,
) -> (
    Checker<ErrorFunc>,
    Reader<impl Tokenizer, impl ErrorHandler>,
    Reader<TokenizerType, HandlerType>,
    Reader<impl Tokenizer, impl ErrorHandler>,
)
where
    Constructor: FnOnce(BufReader<File>, Checker<ErrorFunc>) -> Reader<TokenizerType, HandlerType>,
    ErrorFunc: FnOnce(),
    HandlerType: ErrorHandler,
    TokenizerType: Tokenizer,
{
    let check = Checker::new(f);
    let mut arg_iter = env::args();
    arg_iter.next();
    let mut next_file_as_buf =
        || BufReader::with_capacity(1048576, File::open(arg_iter.next().unwrap()).unwrap());
    (
        check.clone(),
        validating_reader::new(next_file_as_buf()),
        reader_constructor(next_file_as_buf(), check),
        validating_reader::new(next_file_as_buf()),
    )
}
