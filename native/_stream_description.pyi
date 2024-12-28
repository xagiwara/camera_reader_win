from typing import Literal
from ._media_type import MediaType

__all__ = ["StreamDescriptor", "StreamDescriptorByIndex"]

class StreamDescriptor:
    @property
    def major_type(
        self,
    ) -> Literal[
        "audio",
        "binary",
        "file_transfer",
        "html",
        "image",
        "metadaata",
        "protected",
        "perception",
        "sami",
        "script",
        "stream",
        "video",
    ]: ...
    def __len__(self) -> int: ...
    def __getitem__(self, index: int) -> MediaType: ...

class StreamDescriptorByIndex(StreamDescriptor):
    selected: bool
