import argparse

from web_framework_api import *


def generate_binary_asset():
    def callback(progress: float, asset_path: str, _):
        print(f"Adding asset: {asset_path}, {progress * 100}%")

    parser = argparse.ArgumentParser()

    parser.add_argument("directory_path", type=str)
    parser.add_argument("output_path", type=str)

    args = parser.parse_args()

    initialize_web_framework()

    generate_binary_asset_file(args.directory_path, args.output_path, callback)
