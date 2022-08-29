use std::io::BufRead;
use std::iter::Peekable;

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
    let mut reader = Reader {
        tokenizer: tokenizer,
        handler: handler,
    };
    reader.init();
    reader
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
    fn init(&mut self) -> TokenizerResult<()>;
    fn expect_space(&mut self) -> TokenizerResult<()>;
    fn expect_newline(&mut self) -> TokenizerResult<()>;
    fn expect_eof(&mut self) -> TokenizerResult<()>;
    fn read_token(&mut self) -> TokenizerResult<String>;
}

pub trait ExactWhitespaceTokenizer {
    fn read_line(&mut self) -> TokenizerResult<String>;
}

pub trait ErrorHandler {
    fn new() -> Self;
    fn out_of_range(&self) -> !;
    fn parse_error(&self) -> !;
    fn wrong_whitespace(&self) -> !;
}

impl<TK, EH> Reader<TK, EH>
where
    TK: Tokenizer,
    EH: ErrorHandler,
{
    pub fn init(&mut self) {
        let res = self.tokenizer.init();
        self.from_tk_result(res)
    }
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