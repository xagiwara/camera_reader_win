from setuptools import setup, find_packages

setup(
    name="camera_reader_win",
    version="0.0.1a1",
    description="A package containing a .pyd module",
    packages=find_packages(),
    include_package_data=True,
    package_data={
        "camera_reader_win": ["**/*.pyd"],  # .pydファイルを含める
    },
    python_requires=">=3.12",
    classifiers=[
        "Development Status :: 3 - Alpha",
        "Programming Language :: Python :: 3.12",
        "Operating System :: Microsoft :: Windows",  # .pydはWindows限定
    ],
)
