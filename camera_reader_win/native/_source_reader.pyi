from typing import Callable
from ._media_event import MediaEvent
from ._sample import Sample
from ._media_type import MediaType
from ._closable import Closable

__all__ = [
    "SourceReader",
    "SourceReaderCallback",
]

# MF_SOURCE_READER_FIRST_VIDEO_STREAM = 0xFFFFFFFC
# MF_SOURCE_READER_FIRST_AUDIO_STREAM = 0xFFFFFFFD
# MF_SOURCE_READER_ANY_STREAM = 0xFFFFFFFE
# MF_SOURCE_READER_MEDIASOURCE = 0xFFFFFFFF

class SourceReader(Closable["SourceReader"]):
    def get_current_media_type(self, stream_index: int) -> MediaType: ...
    def set_current_media_type(
        self, stream_index: int, media_type: MediaType
    ) -> None: ...
    def read_sample(
        self, stream_index: int, flags: int = 0
    ) -> tuple[int, int, int, Sample | None]: ...
    def read_sample_async(self, stream_index: int, flags: int = 0) -> None: ...

class SourceReaderCallback:
    on_event: Callable[[int, MediaEvent], None] | None
    on_flush: Callable[[int], None] | None
    on_read_sample: Callable[[int, int, int, int, Sample], None] | None
