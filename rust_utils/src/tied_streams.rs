use std::io::Write;

pub struct TiedStandardStreams<R, W> {
    r: R,
    w: W,
}

impl<R, W: Write> TiedStandardStreams<R, W> {
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

impl<R, W> Drop for TiedStandardStreams<R, W> {
    fn drop(&mut self) {
        // Close stdin.
        // This is kind of dumb, but basically this struct is only used for interactive grading,
        // and in those cases, we need to tell the other side of the pipe that we are done.
        unsafe { libc::close(0) };
    }
}
