from ._closable import Closable

__all__ = ["MediaBuffer", "MediaBuffer2D"]

class MediaBuffer(Closable[MediaBuffer]):
    current_length: int
    @property
    def max_length(self) -> int: ...
    def get_bytes(self) -> bytes: ...

class MediaBuffer2D(MediaBuffer):
    def get_bytes_2d(self) -> bytes: ...
