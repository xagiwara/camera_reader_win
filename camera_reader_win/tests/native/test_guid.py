from camera_reader_win import native


def test_guid_null():
    guid_null = native.GUID("{00000000-0000-0000-0000-000000000000}")
    assert str(guid_null) == "{00000000-0000-0000-0000-000000000000}"


def test_guid_eq():
    guid1 = native.GUID("{76e1ef59-43dd-4492-9921-23f129c735b8}")
    guid2 = native.GUID("{76e1ef59-43dd-4492-9921-23f129c735b8}")
    assert guid1 == guid2
