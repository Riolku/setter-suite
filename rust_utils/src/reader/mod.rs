use std::fmt::Debug;
use std::io::Read;
use std::ops::RangeBounds;
use std::str::FromStr;

#[derive(Debug)]
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

pub trait AsciiStream {
    fn next(&mut self) -> Option<u8>;
    fn peek(&mut self) -> Option<u8>;
    fn next_if<F>(&mut self, f: F) -> Option<u8> where
        F: FnOnce(&u8) -> bool;
    fn read_while<F>(&mut self, f: F) -> &[u8]
    where
        F: FnMut(&u8) -> bool;
}

#[derive(Debug)]
pub struct BufferedAsciiStream<R> {
    src: R,
    buf: Vec<u8>,
    buf_pos: usize,
    buf_size: usize,
    token: Vec<u8>,
}

impl<R: Read> BufferedAsciiStream<R> {
    pub fn new(src: R, buf_size: usize) -> Self {
        Self {
            src,
            buf: Vec::new(),
            buf_pos: 0,
            buf_size,
            token: Vec::new(),
        }
    }
    #[cold]
    fn reset_buf(&mut self) -> Option<()> {
        self.buf_pos = 0;
        self.buf.resize(self.buf_size, 0u8);
        let buf_len = self.src.read(&mut *self.buf).unwrap();
        self.buf.truncate(buf_len);
        if buf_len == 0 {
            None
        } else {
            Some(())
        }
    }
    fn fill_buf(&mut self) -> Option<()> {
        if self.buf_pos == self.buf.len() {
            self.reset_buf()
        } else {
            Some(())
        }
    }

    #[cold]
    fn read_while_no_buf<F>(&mut self, start: usize, mut f: F) -> &[u8]
    where
        F: FnMut(&u8) -> bool,
    {
        self.token.clear();
        self.token.extend_from_slice(&self.buf[start..]);
        loop {
            match self.next() {
                Some(b) => {
                    if !f(&b) {
                        self.buf_pos -= 1;
                        break &self.token[..];
                    }
                    self.token.push(b);
                }
                None => break &self.token[..],
            }
        }
    }
}

impl<R: Read> AsciiStream for BufferedAsciiStream<R> {
    fn next(&mut self) -> Option<u8> {
        self.fill_buf()?;
        self.buf_pos += 1;
        Some(self.buf[self.buf_pos - 1])
    }
    fn peek(&mut self) -> Option<u8> {
        self.fill_buf()?;
        Some(self.buf[self.buf_pos])
    }
    fn next_if<F>(&mut self, f: F) -> Option<u8> where
        F: FnOnce(&u8) -> bool {
        match self.next() {
            Some(b) => {
                if ! f(&b) {
                    self.buf_pos -= 1;
                    None
                }
                else {
                    Some(b)
                }
            }
            None => None,
        }
    }
    fn read_while<F>(&mut self, mut f: F) -> &[u8]
    where
        F: FnMut(&u8) -> bool,
    {
        let start = self.buf_pos;
        while self.buf_pos < self.buf.len() {
            if !f(&self.buf[self.buf_pos]) {
                return &self.buf[start..self.buf_pos];
            }
            self.buf_pos += 1;
        }
        self.read_while_no_buf(start, f)
    }
}

#[derive(Debug)]
pub struct FullAsciiStream {
    buf: Vec<u8>,
    buf_pos: usize,
}
impl FullAsciiStream {
    pub fn new(mut src: impl Read) -> Self {
        let mut buf = Vec::new();
        src.read_to_end(&mut buf).unwrap();
        Self { buf, buf_pos: 0 }
    }
}
impl AsciiStream for FullAsciiStream {
    fn next(&mut self) -> Option<u8> {
        if self.buf_pos == self.buf.len() {
            None
        } else {
            self.buf_pos += 1;
            Some(self.buf[self.buf_pos - 1])
        }
    }
    fn peek(&mut self) -> Option<u8> {
        if self.buf_pos == self.buf.len() {
            None
        } else {
            Some(self.buf[self.buf_pos])
        }
    }
    fn next_if<F>(&mut self, f: F) -> Option<u8> where
        F: FnOnce(&u8) -> bool {
        match self.next() {
            Some(b) => {
                if ! f(&b) {
                    self.buf_pos -= 1;
                    None
                }
                else {
                    Some(b)
                }
            }
            None => None,
        }
    }
    fn read_while<F>(&mut self, mut f: F) -> &[u8]
    where
        F: FnMut(&u8) -> bool,
    {
        let start = self.buf_pos;
        while self.buf_pos < self.buf.len() {
            if !f(&self.buf[self.buf_pos]) {
                break;
            }
            self.buf_pos += 1;
        }
        &self.buf[start..self.buf_pos]
    }
}

#[derive(Debug)]
pub enum TokenizerError {
    WhitespaceError,
}
pub type TokenizerResult<T> = Result<T, TokenizerError>;
#[cold]
pub fn wrong_whitespace<T>() -> TokenizerResult<T> {
    Err(TokenizerError::WhitespaceError)
}

pub trait Tokenizer {
    fn expect_space(&mut self) -> TokenizerResult<()>;
    fn expect_newline(&mut self) -> TokenizerResult<()>;
    fn expect_eof(&mut self) -> TokenizerResult<()>;
    fn read_token(&mut self) -> TokenizerResult<&[u8]>;
}

pub trait StandardWhitespace {
    fn next_token_on_line(&mut self) -> Option<&[u8]>;
    fn has_token_in_stream(&mut self) -> TokenizerResult<bool>;
}

pub trait ErrorHandler {
    fn out_of_range(&self) -> !;
    fn parse_error(&self) -> !;
    fn wrong_whitespace(&self) -> !;
    fn non_ascii(&self) -> !;
}

impl<TK, EH> Reader<TK, EH>
where
    TK: Tokenizer,
    EH: ErrorHandler,
{
    pub fn expect_space(&mut self) {
        let res = self.tokenizer.expect_space();
        Self::from_tk_result(&self.handler, res)
    }
    pub fn expect_newline(&mut self) {
        let res = self.tokenizer.expect_newline();
        Self::from_tk_result(&self.handler, res)
    }
    pub fn read_token(&mut self) -> BorrowedToken<'_, EH> {
        let res = self.tokenizer.read_token();
        let borrowed_bytes = Self::from_tk_result(&self.handler, res);
        Self::to_borrowed_token(&self.handler, borrowed_bytes)
    }
    pub fn check_range<T>(&self, val: &T, range: &impl RangeBounds<T>)
    where
        T: PartialOrd,
    {
        if !range.contains(&val) {
            self.handler.out_of_range();
        }
    }
    fn from_tk_result<T>(handler: &EH, res: TokenizerResult<T>) -> T {
        res.unwrap_or_else(|_| handler.wrong_whitespace())
    }
    fn to_borrowed_token<'a>(handler: &'a EH, borrowed_bytes: &'a [u8]) -> BorrowedToken<'a, EH> {
        if ! borrowed_bytes.is_ascii() {
            handler.non_ascii();
        }

        // Since the string is ascii, it's also valid UTF8.
        let borrowed_token = unsafe { std::str::from_utf8_unchecked(borrowed_bytes) };
        BorrowedToken::new(borrowed_token, handler)
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
            Self::from_tk_result(&self.handler, res)
        }
    }
}

impl<TK, EH> Reader<TK, EH>
where
    TK: Tokenizer + StandardWhitespace,
    EH: ErrorHandler,
{
    pub fn next_token_on_line(&mut self) -> Option<BorrowedToken<'_, EH>> {
        match self.tokenizer.next_token_on_line() {
            Some(token_bytes) => Some(Self::to_borrowed_token(&self.handler, token_bytes)),
            None => None,
        }
    }
    pub fn has_token_in_stream(&mut self) -> bool {
        let res = self.tokenizer.has_token_in_stream();
        Self::from_tk_result(&self.handler, res)
    }
}

#[derive(Debug)]
pub struct BorrowedToken<'a, EH> {
    token: &'a str,
    handler: &'a EH,
}

impl<'a, EH> BorrowedToken<'a, EH>
where
    EH: ErrorHandler,
{
    pub fn new(token: &'a str, handler: &'a EH) -> Self {
        Self { token, handler }
    }
    pub fn get(&self) -> &str {
        self.token
    }
    pub fn parse<T>(self) -> T
    where
        T: FromStr,
        T::Err: Debug,
    {
        self.token
            .parse()
            .unwrap_or_else(|_| self.handler.parse_error())
    }
}
impl<'a, EH> PartialEq<&str> for BorrowedToken<'a, EH> {
    fn eq(&self, other: &&str) -> bool {
        self.token == *other
    }
}

#[macro_export]
macro_rules! read_sep {
    ($rd:expr, $range:expr, $first:ty $(,$rest:ty)*) => {
        {
            let rd_ref = &mut $rd;
            let ret = (
                {
                    let ret: $first = rd_ref.read_token().parse();
                    rd_ref.check_range(&ret, &$range);
                    ret
                },
                $({
                    rd_ref.expect_space();
                    let ret: $rest = rd_ref.read_token().parse();
                    rd_ref.check_range(&ret, &$range);
                    ret
                }),*
            );
            rd_ref.expect_newline();
            ret
        }
    };
}

#[macro_export]
macro_rules! read_sep_without_range {
    ($rd:expr, $first:ty $(,$rest:ty)*) => {
        {
            let rd_ref = &mut $rd;
            let ret = (
                {
                    let ret: $first = rd_ref.read_token().parse();
                    ret
                },
                $({
                    rd_ref.expect_space();
                    let ret: $rest = rd_ref.read_token().parse();
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
    ($rd:expr, $size:expr, $type:ty, $range:expr) => {{
        let rd_ref = &mut $rd;
        let cap = $size;
        let range = $range;

        let mut ret = Vec::with_capacity(cap);
        for i in 0..cap {
            if i != 0 {
                rd_ref.expect_space();
            }
            let item: $type = rd_ref.read_token().parse();
            rd_ref.check_range(&item, &range);
            ret.push(item);
        }
        rd_ref.expect_newline();
        ret
    }};
}

#[cfg(test)]
mod tests;
