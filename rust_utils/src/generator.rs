#[macro_export]
macro_rules! count_exprs {
    () => { 0 };
    ($odd:expr $(, $a:expr, $b:expr)*) => { (count_exprs!($($a)*) << 1) | 1 };
    ($($a:expr, $even:expr),*) => { count_exprs!($($a)*) << 1 };
}

#[macro_export]
macro_rules! generator_main {
    ($([$($case:expr,)*],)*) => {
        use std::process::ExitCode;
        fn main() -> ExitCode {
            use std::env;
            let mut args = env::args();
            let _exec = args.next();
            let first = args.next().unwrap();
            if first == "cc" {
                use std::io::{stdout, Write};

                print_many!(stdout().lock(),
                    $(
                        count_exprs!($($case),*)
                    ),*
                );
                ExitCode::SUCCESS
            }
            else {
                let suite: usize = first.parse().unwrap();
                let case_num: usize = args.next().unwrap().parse().unwrap();

                let mut running_suite = 0;
                $(
                    running_suite += 1;
                    if running_suite == suite {
                        let mut running_case = 0;
                        $(
                            running_case += 1;
                            if running_case == case_num {
                                $case.generate();
                                return ExitCode::SUCCESS;
                            }
                        )*

                        eprintln!("No case {} in suite {}", case_num, suite);
                        return ExitCode::FAILURE;
                    }
                )*
                eprintln!("No suite {}", suite);
                ExitCode::FAILURE
            }
        }
    };
}