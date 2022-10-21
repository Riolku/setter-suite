#[cfg(feature = "checker")]
pub mod checker;

#[cfg(feature = "generator")]
pub mod generator;

#[cfg(feature = "identical_checker")]
pub mod identical_checker;

#[cfg(feature = "identical_whitespace")]
pub mod identical_whitespace;

#[cfg(feature = "interactive_solution")]
pub mod interactive_solution;

#[cfg(feature = "non_interactive_solution")]
pub mod non_interactive_solution;

#[cfg(feature = "output")]
pub mod output;

#[cfg(feature = "panic_error_handler")]
pub mod panic_error_handler;

#[cfg(feature = "reader")]
pub mod reader;

#[cfg(feature = "splay")]
pub mod splay;

#[cfg(feature = "standard_checker")]
pub mod standard_checker;

#[cfg(feature = "standard_whitespace")]
pub mod standard_whitespace;

#[cfg(feature = "test_cases")]
pub mod test_cases;

#[cfg(feature = "tied_streams")]
pub mod tied_streams;

#[cfg(feature = "validating_reader")]
pub mod validating_reader;
