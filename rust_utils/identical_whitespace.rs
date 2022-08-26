mod identical_whitespace {
    use super::reader;

    use std::io::BufRead;
    pub struct Handler<SourceIter: reader::ReaderIter>;

    impl<SourceIter: reader::ReaderIter> Handler<SourceIter> {
        fn match_exactly(&mut self, src: &mut Peekable<SourceIter>, char c) -> reader::EmptyResult {
            src.peek().filter(|c| c == ' ')
        }
    }

    impl<SourceIter: reader::ReaderIter> reader::WhitespaceHandler<SourceIter> for Handler<SourceIter> {
        fn read_space(&mut self, src: &mut Peekable<SourceIter>) -> reader::EmptyResult {
            self.peek().unwrap_or(Err(()))?.
        }
    }
}
