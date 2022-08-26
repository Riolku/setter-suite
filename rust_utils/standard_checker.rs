mod standard_checker {
    use super::{checker, reader};
    pub struct ErrorHandler;

    impl checker::ErrorHandler for ErrorHandler {}
    impl reader::WhitespaceErrorHandler for ErrorHandler {
        fn wrong_whitespace() -> ! {
            checker::exit(checker::codes::WA);
        }
    }
}
