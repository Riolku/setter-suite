use rand::Rng;
use std::io::Write;

pub trait TestCase {
    fn generate(self, rng: impl Rng, input_stream: impl Write, output_stream: impl Write);
}

pub trait StreamWriteable {
    fn write_to(&self, stream: impl Write);
}

pub trait InputGenerator {
    type Input;
    fn generate(self, rng: impl Rng) -> Self::Input;
}

pub trait AbstractSolver {
    type Input;
    type Output;
    fn solve(self, input: Self::Input) -> Self::Output;
}

pub struct SolutionTest<G, S> {
    generator: G,
    solver: S,
}

impl<G, S> SolutionTest<G, S>
{
    pub fn new(generator: G, solver: S) -> Self {
        Self { generator, solver }
    }
}

impl<G, S, I, O> TestCase for SolutionTest<G, S>
where
    G: InputGenerator<Input = I>,
    S: AbstractSolver<Input = I, Output = O>,
    I: StreamWriteable,
    O: StreamWriteable,
{
    fn generate(self, rng: impl Rng, input_stream: impl Write, output_stream: impl Write) {
        let input = self.generator.generate(rng);
        input.write_to(input_stream);
        let output = self.solver.solve(input);
        output.write_to(output_stream);
    }
}
