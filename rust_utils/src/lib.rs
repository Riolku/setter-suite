#[cfg(feature = "checker")]
pub mod checker;

#[cfg(feature = "reader")]
pub mod reader;

#[cfg(feature = "identical_checker")]
pub mod identical_checker;

#[cfg(feature = "identical_whitespace")]
pub mod identical_whitespace;

#[cfg(feature = "standard_checker")]
pub mod standard_checker;

#[cfg(feature = "standard_whitespace")]
pub mod standard_whitespace;

#[cfg(feature = "output")]
pub mod output;

#[cfg(feature = "panic_error_handler")]
pub mod panic_error_handler;

#[cfg(feature = "validating_reader")]
pub mod validating_reader;
