use super::checker::Checker;
use super::identical_checker_handler::{self, ErrorHandler as IdenticalHandler};
use super::reader::{ErrorHandler, Reader, Tokenizer};
use super::validating_reader;

use std::env;
use std::fs::File;
use std::io;

pub fn entry<F: FnOnce()>(
    f: F,
) -> (
    Checker<F>,
    Reader<impl Tokenizer, impl ErrorHandler>,
    Reader<impl Tokenizer, IdenticalHandler<F>>,
    Reader<impl Tokenizer, impl ErrorHandler>,
) {
    let check = Checker::new(f);
    let mut arg_iter = env::args();
    arg_iter.next();
    let mut next_file_as_buf =
        || io::BufReader::with_capacity(1048576, File::open(arg_iter.next().unwrap()).unwrap());
    (
        check.clone(),
        validating_reader::new(next_file_as_buf()),
        identical_checker_handler::new_checker(next_file_as_buf(), check),
        validating_reader::new(next_file_as_buf()),
    )
}
