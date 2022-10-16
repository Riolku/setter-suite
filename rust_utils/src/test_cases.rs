extern crate rand;
use self::rand::Rng;
use std::io::Write;

pub trait TestCase {
    fn generate(self, rng: impl Rng, input_stream: impl Write, output_stream: impl Write);
}

pub struct LiteralTest {
    input: &'static str,
    output: &'static str,
}

impl LiteralTest {
    pub fn new(input: &'static str, output: &'static str) -> Self {
        Self { input, output }
    }
}

impl TestCase for LiteralTest {
    fn generate(self, _: impl Rng, mut input_stream: impl Write, mut output_stream: impl Write) {
        input_stream.write_all(&self.input.as_bytes()).unwrap();
        output_stream.write_all(&self.output.as_bytes()).unwrap();
        input_stream.flush().unwrap();
        output_stream.flush().unwrap();
    }
}

pub trait StreamWriteable {
    fn write_to(&self, stream: &mut impl Write);
}

pub trait InputGenerator {
    type Input;
    fn generate(self, rng: impl Rng) -> Self::Input;
}

pub struct SolutionTestFactory<S> {
    solver: S,
}

impl<S> SolutionTestFactory<S> {
    pub const fn new(solver: S) -> Self {
        Self { solver }
    }

    pub fn with<G>(self, generator: G) -> SolutionTest<G, S> {
        SolutionTest::new(generator, self.solver)
    }
}

pub struct SolutionTest<G, S> {
    generator: G,
    solver: S,
}

impl<G, S> SolutionTest<G, S> {
    pub fn new(generator: G, solver: S) -> Self {
        Self { generator, solver }
    }
}

impl<G, S, I, O> TestCase for SolutionTest<G, S>
where
    G: InputGenerator<Input = I>,
    S: FnOnce(I) -> O,
    I: StreamWriteable,
    O: StreamWriteable,
{
    fn generate(self, rng: impl Rng, mut input_stream: impl Write, mut output_stream: impl Write) {
        let input = self.generator.generate(rng);
        input.write_to(&mut input_stream);
        let output = (self.solver)(input);
        output.write_to(&mut output_stream);
        input_stream.flush().unwrap();
        output_stream.flush().unwrap();
    }
}
