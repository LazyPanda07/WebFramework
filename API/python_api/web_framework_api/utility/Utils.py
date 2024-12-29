import os.path
import shutil
import sys
import zipfile

import requests

from tqdm import tqdm

WEB_FRAMEWORK_MAJOR_VERSION = "0"
WEB_FRAMEWORK_MINOR_VERSION = "12"
WEB_FRAMEWORK_LIBRARIES_FILE_NAME = "libraries.zip"
WEB_FRAMEWORK_DOWNLOAD_BLOCK_SIZE = 1024


def download_libraries(download_path: str):
    url = get_url()
    response = requests.get(url, stream=True)

    total_size = int(response.headers.get("content-length", 0))

    with tqdm(total=total_size, unit="B", unit_scale=True) as progress_bar:
        progress_bar.set_description(f"Downloading {url}")

        with open(f"{download_path}/{WEB_FRAMEWORK_LIBRARIES_FILE_NAME}", "wb") as file:
            for data in response.iter_content(WEB_FRAMEWORK_DOWNLOAD_BLOCK_SIZE):
                progress_bar.update(len(data))
                file.write(data)

    if total_size != 0 and progress_bar.n != total_size:
        raise RuntimeError("Could not download file")

    return f"{download_path}/{WEB_FRAMEWORK_LIBRARIES_FILE_NAME}"


def unzip_libraries(path_to_zip: str, output_path: str):
    with zipfile.ZipFile(path_to_zip, "r") as zip_file:
        zip_file.extractall(output_path)


def copy_libraries(libraries_directory: str):
    if sys.platform == "win32":
        directory_path = os.path.join(libraries_directory, "ReleaseDLL_Windows", "dll")

        for file in os.listdir(directory_path):
            shutil.copy(os.path.join(directory_path, file), libraries_directory)

    else:
        directory_path = os.path.join(libraries_directory, "ReleaseDLL_Linux", "lib")

        for file in os.listdir(directory_path):
            if file.endswith(".so"):
                shutil.copy(os.path.join(directory_path, file), libraries_directory)


def remove_files(libraries_directory: str):
    for file in os.listdir(libraries_directory):
        if os.path.isdir(os.path.join(libraries_directory, file)):
            shutil.rmtree(os.path.join(libraries_directory, file))

    os.remove(os.path.join(libraries_directory, WEB_FRAMEWORK_LIBRARIES_FILE_NAME))


def create_directories(libraries_directory: str):
    if sys.platform == "win32":
        os.makedirs(os.path.join(libraries_directory, "dll"))
    else:
        os.makedirs(os.path.join(libraries_directory, "lib"))

def get_url():
    if sys.platform == "win32":
        return f"https://github.com/LazyPanda07/WebFramework/releases/download/v3.{WEB_FRAMEWORK_MAJOR_VERSION}.{WEB_FRAMEWORK_MINOR_VERSION}/windows.zip"
    else:
        return f"https://github.com/LazyPanda07/WebFramework/releases/download/v3.{WEB_FRAMEWORK_MAJOR_VERSION}.{WEB_FRAMEWORK_MINOR_VERSION}/linux.zip"


def get_libraries(libraries_directory: str):
    create_directories(libraries_directory)

    zip_path = download_libraries(libraries_directory)

    unzip_libraries(zip_path, libraries_directory)

    copy_libraries(libraries_directory)

    remove_files(libraries_directory)
