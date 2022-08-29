mod codes {
    pub const AC: i32 = 0;
    pub const WA: i32 = 1;
    pub const PE: i32 = 2;
    pub const IE: i32 = 3;
    pub const PARTIAL: i32 = 7;
}

use std::process;

pub fn assert_or_code(cond: bool, code: i32) {
    if !cond {
        exit(code);
    }
}

pub fn assert_or_wa(cond: bool) {
    assert_or_code(cond, codes::WA);
}

pub fn assert_or_pe(cond: bool) {
    assert_or_code(cond, codes::PE);
}

pub fn exit(code: i32) -> ! {
    super::pre_error();
    process::exit(code);
}
