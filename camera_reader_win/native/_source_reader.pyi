from abc import ABCMeta, abstractmethod
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

class SourceReaderCallback(metaclass=ABCMeta):
    @abstractmethod
    def on_event(self, stream_index: int, event: MediaEvent) -> None: ...
    @abstractmethod
    def on_flush(self, stream_index: int) -> None: ...
    @abstractmethod
    def on_read_sample(
        self,
        hr_status: int,
        stream_index: int,
        stream_flags: int,
        timestamp: int,
        sample: Sample,
    ) -> None: ...
