use super::reader::{BufferedAsciiStream, ErrorHandler, Reader, Tokenizer};
use super::validating_reader;

use std::cell::Cell;
use std::env;
use std::fs::File;
use std::rc::Rc;

pub mod codes {
    pub const AC: u8 = 0;
    pub const WA: u8 = 1;
    pub const PE: u8 = 2;
    pub const IE: u8 = 3;
    pub const PARTIAL: u8 = 7;
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

pub fn partial(num: i32, denom: i32) -> u8 {
    eprintln!("partial {num}/{denom}\n");
    print!("{num}/{denom} points");
    return codes::PARTIAL;
}

impl<F> Checker<F>
where
    F: FnOnce() -> Option<u8>,
{
    pub fn new(pre_error: F) -> Self {
        Self {
            pre_error: Rc::new(Cell::new(Some(pre_error))),
        }
    }

    pub fn assert_or_code(&self, cond: bool, code: u8) {
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

    pub fn exit(&self, code: u8) -> ! {
        let pre_error_code = (self.pre_error.take().unwrap())();
        std::process::exit(i32::from(pre_error_code.unwrap_or(code)));
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
    Constructor:
        FnOnce(BufferedAsciiStream<File>, Checker<ErrorFunc>) -> Reader<TokenizerType, HandlerType>,
    ErrorFunc: FnOnce() -> Option<u8>,
    HandlerType: ErrorHandler,
    TokenizerType: Tokenizer,
{
    let check = Checker::new(f);
    let mut arg_iter = env::args();
    arg_iter.next();
    let mut next_file_as_stream =
        || BufferedAsciiStream::new(File::open(arg_iter.next().unwrap()).unwrap(), 1048576);
    (
        check.clone(),
        validating_reader::new(next_file_as_stream()),
        reader_constructor(next_file_as_stream(), check),
        validating_reader::new(next_file_as_stream()),
    )
}

#[macro_export]
macro_rules! read_maybe_array {
    ($rd:expr, $size:expr, $type:ty, $range:expr) => {{
        let rd_ref = &mut $rd;
        let cap = $size;
        assert!(cap > 0);

        let first_token = rd_ref.read_token();
        let ret = if first_token.get() == "-1" {
            None
        } else {
            let range = $range;
            let mut ret = Vec::with_capacity(cap);

            let first: $type = first_token.parse();
            rd_ref.check_range(&first, &range);
            ret.push(first);

            for _ in 1..cap {
                rd_ref.expect_space();
                let item: $type = rd_ref.read_token().parse();
                rd_ref.check_range(&item, &range);
                ret.push(item);
            }
            Some(ret)
        };
        rd_ref.expect_newline();
        ret
    }};
}

#[macro_export]
macro_rules! read_maybe_single {
    ($rd:expr, $type:ty, $range:expr) => {{
        let rd_ref = &mut $rd;

        let tk = rd_ref.read_token();
        let ret = if tk.get() == "-1" {
            None
        } else {
            let ret: $type = tk.parse();
            rd_ref.check_range(&ret, &$range);
            Some(ret)
        };
        rd_ref.expect_newline();
        ret
    }};
}

#[cfg(test)]
mod tests;
