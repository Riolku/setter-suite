use rand::seq::SliceRandom;
use rand_xoshiro::Xoshiro256PlusPlus;
use std::io::Write;

pub type ConcreteRng = Xoshiro256PlusPlus;

pub trait TestCase {
    fn generate(self, rng: ConcreteRng, input_stream: impl Write, output_stream: impl Write);
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
    fn generate(self, _: ConcreteRng, mut input_stream: impl Write, mut output_stream: impl Write) {
        input_stream.write_all(&self.input.as_bytes()).unwrap();
        output_stream.write_all(&self.output.as_bytes()).unwrap();
        input_stream.flush().unwrap();
        output_stream.flush().unwrap();
    }
}

pub trait InputGenerator {
    type Input;
    fn generate(self, rng: &mut ConcreteRng) -> Self::Input;
}

pub trait InputGeneratorRef {
    type Input;
    fn generate(&self, rng: &mut ConcreteRng) -> Self::Input;
}

pub struct LiteralInput<I> {
    input: I,
}

impl<I> LiteralInput<I> {
    pub fn new(input: I) -> Self {
        Self { input }
    }
}

impl<I> InputGenerator for LiteralInput<I> {
    type Input = I;
    fn generate(self, _: &mut ConcreteRng) -> I {
        self.input
    }
}

impl<I> InputGeneratorRef for LiteralInput<I>
where
    I: Clone,
{
    type Input = I;
    fn generate(&self, _: &mut ConcreteRng) -> I {
        self.input.clone()
    }
}

pub fn to_generator_ref_iter<'a, C, G, I>(
    items: I,
) -> impl Iterator<Item = &'a dyn InputGeneratorRef<Input = C>>
where
    I: IntoIterator<Item = &'a G>,
    G: 'a + InputGeneratorRef<Input = C>,
{
    items
        .into_iter()
        .map(|r| r as &dyn InputGeneratorRef<Input = C>)
}

pub trait StreamWriteable {
    fn write_to(&self, stream: &mut impl Write);
}

pub trait InputStreamWriteable {
    fn write_to(&self, stream: &mut impl Write);
}

pub trait OutputStreamWriteable {
    fn write_to(&self, stream: &mut impl Write);
}

impl<SW> InputStreamWriteable for SW
where
    SW: StreamWriteable,
{
    fn write_to(&self, stream: &mut impl Write) {
        StreamWriteable::write_to(self, stream)
    }
}

impl<SW> OutputStreamWriteable for SW
where
    SW: StreamWriteable,
{
    fn write_to(&self, stream: &mut impl Write) {
        StreamWriteable::write_to(self, stream)
    }
}

pub struct ManyCases<I> {
    inner: I,
}

impl<'a, I> ManyCases<I>
where
    I: IntoIterator,
{
    pub fn new(inner: I) -> Self {
        Self { inner }
    }
}

impl<'a, C, I> InputGenerator for ManyCases<I>
where
    C: 'a,
    I: IntoIterator<Item = &'a dyn InputGeneratorRef<Input = C>>,
{
    type Input = Vec<C>;
    fn generate(self, rng: &mut ConcreteRng) -> Vec<C> {
        let mut output: Vec<_> = self
            .inner
            .into_iter()
            .map(|case| case.generate(rng))
            .collect();
        output.shuffle(rng);
        output
    }
}

pub struct AbstractSolutionTest<G, S> {
    generator: G,
    solver: S,
}

impl<G, S> AbstractSolutionTest<G, S> {
    pub fn new(generator: G, solver: S) -> Self {
        Self { generator, solver }
    }
}

impl<G, S, I, O> TestCase for AbstractSolutionTest<G, S>
where
    G: InputGenerator<Input = I>,
    S: FnOnce(I) -> O,
    I: InputStreamWriteable,
    O: OutputStreamWriteable,
{
    fn generate(
        self,
        mut rng: ConcreteRng,
        mut input_stream: impl Write,
        mut output_stream: impl Write,
    ) {
        let input = self.generator.generate(&mut rng);
        input.write_to(&mut input_stream);
        let output = (self.solver)(input);
        output.write_to(&mut output_stream);
        input_stream.flush().unwrap();
        output_stream.flush().unwrap();
    }
}
