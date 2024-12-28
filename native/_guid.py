__all__ = ["GUID"]


class GUID:
    def __init__(self, guid: str) -> None: ...


class GuidEnum:
    def __getattr__(self, key: str) -> GUID: ...
    def get_name(self, guid: GUID) -> str: ...
