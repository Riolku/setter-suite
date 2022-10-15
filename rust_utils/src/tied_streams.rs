use std::io::Write;

pub struct TiedStreams<R, W> {
    r: R,
    w: W,
}

impl<R, W: Write> TiedStreams<R, W> {
    pub fn new(r: R, w: W) -> Self {
        Self { r, w }
    }
    pub fn rd(&mut self) -> &mut R {
        self.w.flush().unwrap();
        &mut self.r
    }
    pub fn wr(&mut self) -> &mut W {
        &mut self.w
    }
}
