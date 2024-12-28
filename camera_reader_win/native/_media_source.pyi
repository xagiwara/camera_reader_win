from typing import Literal
from ._stream_description import StreamDescriptorByIndex
from ._closable import Closable
from ._source_reader import SourceReader, SourceReaderCallback

__all__ = ["MediaSource"]

class MediaSource(Closable):
    def __init__(
        self,
        device_name: Literal["audio", "video"],
        endpoint_id_or_symbolic_link: str | None = None,
    ) -> None: ...
    def __enter__(self) -> MediaSource: ...
    def __exit__(
        self,
        exc_type: any,
        exc_value: any,
        traceback: any,
    ) -> None: ...
    def close(self) -> None: ...
    @property
    def characteristics(self) -> set[
        Literal[
            "live",
            "seekable",
            "pausable",
            "slow_seek",
            "multiple_presentation",
            "skip_forwardable",
            "skip_backwardable",
            "no_network",
        ]
    ]: ...
    def __len__(self) -> int: ...
    def __getitem__(self, index: int) -> StreamDescriptorByIndex: ...
    def create_source_reader(
        self,
        low_latency: bool | None = None,
        readwrite_disable_converters: bool | None = None,
        readwrite_enable_hardware_transforms: bool | None = None,
        source_reader_async_callback: SourceReaderCallback | None = None,
        source_reader_d3d_manager: None = None,
        source_reader_disable_dxva: bool | None = None,
        source_reader_disconnect_mediasource_on_shutdown: bool | None = None,
        source_reader_enable_advanced_video_processing: bool | None = None,
        source_reader_enable_video_processing: bool | None = None,
        source_reader_mediasource_config: None = None,
        source_reader_passthrough_mode: bool | None = None,
        fieldofuse_ulock_attribute: None = None,
    ) -> SourceReader: ...
