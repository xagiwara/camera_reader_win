from . import native
from typing import NamedTuple
import asyncio

__all__ = ["FrameSize", "FrameRate", "VideoDeviceInfo", "VideoDevice"]


class FrameSize(NamedTuple):
    width: int
    height: int

    def __str__(self) -> str:
        return f"{self.width}x{self.height}"

    def __repr__(self):
        return str(self)

    def __le__(self, other: "FrameSize") -> bool:
        return self.width * self.height <= other.width * other.height

    def __lt__(self, other: "FrameSize") -> bool:
        return self.width * self.height < other.width * other.height

    def __ge__(self, other: "FrameSize") -> bool:
        return self.width * self.height >= other.width * other.height

    def __gt__(self, other: "FrameSize") -> bool:
        return self.width * self.height > other.width * other.height


class FrameRate(NamedTuple):
    numerator: int
    denominator: int

    def __str__(self) -> str:
        return (
            f"{self.numerator}/{self.denominator}"
            if self.denominator != 1
            else str(self.numerator)
        )

    def __float__(self) -> float:
        return self.numerator / self.denominator

    def __repr__(self):
        return str(self)

    def __le__(self, other: "FrameRate") -> bool:
        return float(self) <= float(other)

    def __lt__(self, other: "FrameRate") -> bool:
        return float(self) < float(other)

    def __ge__(self, other: "FrameRate") -> bool:
        return float(self) >= float(other)

    def __gt__(self, other: "FrameRate") -> bool:
        return float(self) > float(other)


class VideoDeviceInfo(NamedTuple):
    display_name: str
    """人間が読み取り可能なデバイス名。"""

    name: str
    """デバイスを一意に識別する識別子。"""

    @staticmethod
    def enum() -> list["VideoDeviceInfo"]:
        return [
            VideoDeviceInfo(
                name=device_source.video_symbolic_link,
                display_name=device_source.friendly_name,
            )
            for device_source in native.enum_device_sources("video")
        ]

    def open(
        self, frame_size: tuple[int, int] | None, frame_rate: tuple[int, int] | None
    ) -> "VideoDevice":
        return VideoDevice(self.name, frame_size, frame_rate)


class VideoDevice:
    _media_source: native.MediaSource
    _frame_rate: FrameRate
    _frame_size: FrameSize

    def __init__(
        self,
        name: str | VideoDeviceInfo,
        frame_size: tuple[int, int] | None = None,
        frame_rate: tuple[int, int] | None = None,
    ):
        name = name if isinstance(name, str) else name.name
        self._media_source = native.MediaSource("video", name)
        self._reader = self._media_source.create_source_reader(
            source_reader_enable_advanced_video_processing=True,
        )
        mt = native.MediaType()
        mt.major_type = native.MediaType.MAJOR_TYPES.MFMediaType_Video
        mt.subtype = native.MediaType.VIDEO_FORMATS.MFVideoFormat_RGB32
        if frame_size is not None:
            mt.frame_size = (*frame_size,)
        if frame_rate is not None:
            mt.frame_rate = (*frame_rate,)
        self._reader.set_current_media_type(0, mt)
        mt = self._reader.get_current_media_type(0)
        self._frame_size = FrameSize(*mt.frame_size)
        self._frame_rate = FrameRate(*mt.frame_rate)

    def close(self):
        self._reader.close()
        self._media_source.close()

    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc_value, traceback):
        self.close()

    def read_sample_sync(self):
        while True:
            index, flags, timestamp, sample = self._reader.read_sample(0)
            if sample is None:
                continue
            with sample:
                with sample.convert_to_contiguous_buffer() as buffer:
                    buffer: native.MediaBuffer2D
                    return buffer.get_bytes_2d()

    async def read_sample(self):
        loop = asyncio.get_event_loop()
        return await loop.run_in_executor(None, self.read_sample_sync)

    @property
    def frame_size(self) -> FrameSize:
        return self._frame_size

    @property
    def frame_rate(self) -> FrameRate:
        return self._frame_rate

    @staticmethod
    def enum_options(name: str | VideoDeviceInfo) -> list[VideoDeviceInfo]:
        name = name if isinstance(name, str) else name.name
        with native.MediaSource("video", name) as media_sources:
            source = media_sources[0]

            options: set[tuple[FrameSize, FrameRate]] = set()
            for i in range(len(source)):
                options.add(
                    (
                        FrameSize(*source[i].frame_size),
                        FrameRate(*source[i].frame_rate),
                    )
                )
            return options

    @staticmethod
    def frame_rates(name: str | VideoDeviceInfo) -> list[FrameRate]:
        return {frame_rate for _, frame_rate in VideoDevice.enum_options(name)}

    @staticmethod
    def frame_sizes(name: str | VideoDeviceInfo) -> list[FrameSize]:
        return {frame_size for frame_size, _ in VideoDevice.enum_options(name)}
