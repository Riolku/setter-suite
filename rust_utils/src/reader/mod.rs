use std::fmt::Debug;
use std::io::BufRead;
use std::iter::Peekable;
use std::str::FromStr;

pub struct Reader<TK, EH>
where
    TK: Tokenizer,
    EH: ErrorHandler,
{
    tokenizer: TK,
    handler: EH,
}
pub fn new<TK, EH>(tokenizer: TK, handler: EH) -> Reader<TK, EH>
where
    TK: Tokenizer,
    EH: ErrorHandler,
{
    Reader {
        tokenizer: tokenizer,
        handler: handler,
    }
}

pub trait AsciiStream: Iterator<Item = char> {
    fn peek(&mut self) -> Option<&char>;
}
impl<I> AsciiStream for Peekable<I>
where
    I: Iterator<Item = char>,
{
    fn peek(&mut self) -> Option<&char> {
        self.peek()
    }
}
pub fn to_ascii_stream(src: impl BufRead) -> impl AsciiStream {
    src.bytes().map(|res| res.unwrap() as char).peekable()
}

pub enum TokenizerError {
    WhitespaceError,
}
pub const WRONG_WHITESPACE: TokenizerError = TokenizerError::WhitespaceError;
pub type TokenizerResult<T> = Result<T, TokenizerError>;

pub trait Tokenizer {
    fn expect_space(&mut self) -> TokenizerResult<()>;
    fn expect_newline(&mut self) -> TokenizerResult<()>;
    fn expect_eof(&mut self) -> TokenizerResult<()>;
    fn read_token(&mut self) -> TokenizerResult<String>;
}

pub trait ExactWhitespaceTokenizer {
    fn read_line(&mut self) -> TokenizerResult<String>;
    fn peek(&mut self) -> Option<&char>;
    fn next(&mut self) -> Option<char>;
}

pub trait StandardWhitespace {
    fn next_token_on_line(&mut self) -> Option<String>;
    fn has_token_in_stream(&mut self) -> TokenizerResult<bool>;
}

pub trait ErrorHandler {
    fn out_of_range(&self) -> !;
    fn parse_error(&self) -> !;
    fn wrong_whitespace(&self) -> !;
}

impl<TK, EH> Reader<TK, EH>
where
    TK: Tokenizer,
    EH: ErrorHandler,
{
    pub fn expect_space(&mut self) {
        let res = self.tokenizer.expect_space();
        self.from_tk_result(res)
    }
    pub fn expect_newline(&mut self) {
        let res = self.tokenizer.expect_newline();
        self.from_tk_result(res)
    }
    pub fn read_token(&mut self) -> String {
        let res = self.tokenizer.read_token();
        self.from_tk_result(res)
    }
    pub fn parse_token<T>(&mut self) -> T
    where
        T: FromStr,
        T::Err: Debug,
    {
        self.read_token().parse().unwrap()
    }
    fn from_tk_result<T>(&self, res: TokenizerResult<T>) -> T {
        res.unwrap_or_else(|_| self.handler.wrong_whitespace())
    }
}

impl<TK, EH> Drop for Reader<TK, EH>
where
    TK: Tokenizer,
    EH: ErrorHandler,
{
    fn drop(&mut self) {
        // If the thread is panicking, let it panic. We only run EOF checks when we aren't panicking.
        if !std::thread::panicking() {
            let res = self.tokenizer.expect_eof();
            self.from_tk_result(res)
        }
    }
}

impl<TK, EH> Reader<TK, EH>
where
    TK: Tokenizer + ExactWhitespaceTokenizer,
    EH: ErrorHandler,
{
    pub fn read_line(&mut self) -> String {
        let res = self.tokenizer.read_line();
        self.from_tk_result(res)
    }
}

impl<TK, EH> Reader<TK, EH>
where
    TK: Tokenizer + StandardWhitespace,
    EH: ErrorHandler,
{
    pub fn next_token_on_line(&mut self) -> Option<String> {
        self.tokenizer.next_token_on_line()
    }
    pub fn has_token_in_stream(&mut self) -> bool {
        let res = self.tokenizer.has_token_in_stream();
        self.from_tk_result(res)
    }
}

#[macro_export]
macro_rules! read_sep {
    ($rd:expr, $first:ty $(,$rest:ty)*) => {
        {
            let rd_ref = &mut $rd;
            let ret = (
                {
                    let ret: $first = rd_ref.parse_token();
                    ret
                },
                $({
                    rd_ref.expect_space();
                    let ret: $rest = rd_ref.parse_token();
                    ret
                }),*
            );
            rd_ref.expect_newline();
            ret
        }
    };
}

#[macro_export]
macro_rules! read_array {
    ($rd:expr, $size:expr, $type:ty) => {{
        let cap = $size;
        let mut ret = Vec::with_capacity(cap);
        for i in 0..cap {
            if i != 0 {
                $rd.expect_space();
            }

            let item: $type = $rd.parse_token();
            ret.push(item);
        }
        $rd.expect_newline();
        ret
    }};
}

#[cfg(test)]
mod tests;
