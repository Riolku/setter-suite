#[macro_export]
macro_rules! count_exprs {
    () => { 0 };
    ($odd:expr $(, $a:expr, $b:expr)*) => { (count_exprs!($($a),*) << 1) | 1 };
    ($($a:expr, $even:expr),*) => { count_exprs!($($a),*) << 1 };
}

#[macro_export]
macro_rules! generator_main {
    ($([$($case:expr),* $(,)?]),* $(,)?) => {
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
                                use rand::SeedableRng;
                                let rng = ConcreteRng::seed_from_u64(
                                    ((suite << 32) | case_num) as u64
                                );

                                use std::io::{BufWriter, stdout, stderr};
                                let input_stream = BufWriter::with_capacity(1048576, stdout().lock());
                                let output_stream = BufWriter::with_capacity(1048576, stderr().lock());

                                $case.generate(rng, input_stream, output_stream);
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

#[macro_export]
macro_rules! bind_solution_test {
    ($solve:expr, $input_type:ty, $output_type:ty) => {
        struct SolutionTest;

        use dmoj_rust_utils::test_cases::AbstractSolutionTest;
        impl SolutionTest {
            fn new<G>(
                generator: G,
            ) -> AbstractSolutionTest<G, impl FnOnce($input_type) -> $output_type> {
                AbstractSolutionTest::new(generator, $solve)
            }
        }
    };
}

#[macro_export]
macro_rules! bind_many_solution_test {
    ($solve:ident, $input_type:ty, $output_type:ty) => {
        type ManyInput = Vec<$input_type>;
        type ManyOutput = Vec<$output_type>;

        impl InputStreamWriteable for ManyInput {
            fn write_to(&self, stream: &mut impl Write) {
                print_many!(stream, self.len());
                for case in self {
                    InputStreamWriteable::write_to(case, stream);
                }
            }
        }

        impl OutputStreamWriteable for ManyOutput {
            fn write_to(&self, stream: &mut impl Write) {
                for case in self {
                    OutputStreamWriteable::write_to(case, stream);
                }
            }
        }

        fn solve_many(input: ManyInput) -> ManyOutput {
            input.into_iter().map($solve).collect()
        }

        bind_solution_test!(solve_many, ManyInput, ManyOutput);
    };
}

#[macro_export]
macro_rules! chain_all_iters {
    ($only:expr) => {
        $only
    };
    ($($first:expr, $second:expr),+) => {
        chain_all_iters!(
            $(
                $first.chain($second)
            ),*
        )
    };
    ($first:expr, $($a:expr, $b:expr),+) => {
        chain_all_iters!(
            $first,
            $(
                $a.chain($b)
            ),*
        )
    };
}

#[macro_export]
macro_rules! many_cases {
    () => {
        compile_error!("Why are you calling `many_cases` with zero arguments?")
    };
    ($($e:expr),+ $(,)?) => {
        ManyCases::new(
            chain_all_iters!(
                $(
                    dmoj_rust_utils::test_cases::to_generator_ref_iter($e)
                ),*
            )
        )
    }
}

pub mod prelude {
    pub use super::super::test_cases::{
        ConcreteRng, InputGenerator, InputGeneratorRef, InputStreamWriteable, LiteralInput,
        LiteralTest, ManyCases, OutputStreamWriteable, StreamWriteable, TestCase,
    };
    pub use rand::distributions::Uniform;
    pub use rand::prelude::*;
    pub use std::io::Write;
    pub use std::iter;
}
