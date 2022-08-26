mod reader {
    use std::io;
    use std::iter::Peekable;

    pub type ReaderIter = Iterator<Item = char>;
    pub struct Reader<
        SourceIter: ReaderIter,
        WH: WhitespaceHandler<SourceIter>,
        EH: ValueErrorHandler + WhitespaceErrorHandler,
    > {
        pub src: Peekable<SourceIter>,
        pub wh: WH,
        pub eh: EH,
    }

    impl<
            SourceIter: ReaderIter,
            WH: WhitespaceHandler<SourceIter>,
            EH: ValueErrorHandler + WhitespaceErrorHandler,
        > Reader<SourceIter, WH, EH>
    {
        fn read_space(&mut self) {
            if self.wh.read_space(&mut self.src).is_err() {
                EH::wrong_whitespace()
            }
        }

        fn read_newline(&mut self) {
            if self.wh.read_newline(&mut self.src).is_err() {
                EH::wrong_whitespace()
            }
        }

        fn read_eof(&mut self) {
            if self.wh.read_eof(&mut self.src).is_err() {
                EH::wrong_whitespace()
            }
        }
    }

    pub type EmptyResult = Result<(), ()>;
    pub trait WhitespaceHandler<SourceIter: ReaderIter> {
        fn init(&mut self, src: &mut Peekable<SourceIter>) -> EmptyResult;
        fn read_space(&mut self, src: &mut Peekable<SourceIter>) -> EmptyResult;
        fn read_newline(&mut self, src: &mut Peekable<SourceIter>) -> EmptyResult;
        fn read_eof(&mut self, src: &mut Peekable<SourceIter>) -> EmptyResult;
        fn before_token(&mut self, src: &mut Peekable<SourceIter>) -> EmptyResult;
    }

    pub trait ValueErrorHandler {
        fn out_of_range(token: &str) -> !;
        fn cant_parse(token: &str) -> !;
    }

    pub trait WhitespaceErrorHandler {
        fn wrong_whitespace() -> !;
    }

    pub struct PanicErrorHandler;
    impl ValueErrorHandler for PanicErrorHandler {
        fn out_of_range(token: &str) -> ! {
            panic!("{} is out of range", token);
        }
        fn cant_parse(token: &str) -> ! {
            panic!("can't parse {}", token);
        }
    }

    impl WhitespaceErrorHandler for PanicErrorHandler {
        fn wrong_whitespace() -> ! {
            panic!("wrong whitespace");
        }
    }

    pub fn to_ascii_stream(
        source: impl io::BufRead,
    ) -> Peekable<impl Iterator<Item = char>> {
        source
            .bytes()
            .map(|x| x.unwrap() as char)
            .peekable()
    }
}

fn main() {
    let reader = reader::Reader {

    };
}
