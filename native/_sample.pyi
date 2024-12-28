from ._media_buffer import MediaBuffer

__all__ = ["Sample"]

class Sample:
    duration: int
    sample_time: int
    @property
    def total_length(self) -> int: ...
    @property
    def buffer_count(self) -> int: ...
    def __len__(self) -> int:
        """Sugar syntax for buffer_count(self)"""
        ...

    def get_buffer_by_index(self, index: int) -> MediaBuffer: ...
    def convert_to_contiguous_buffer(self) -> MediaBuffer: ...
