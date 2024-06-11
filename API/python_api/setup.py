from setuptools import find_packages
from setuptools import setup

setup(
    name="web_framework_api",
    version="v1.0.0",
    packages=find_packages(),
    install_requires=[
        "multipledispatch"
    ],
    description="Python API for WebFramework",
    author="LazyPanda07",
    author_email="semengricenko@gmail.com",
    url="http://github.com/LazyPanda07/WebFramework",
    license="MIT",
    keywords="Web"
)
