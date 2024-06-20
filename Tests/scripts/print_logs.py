import os

def print_log(file_path: str):
    with open(file_path, "r") as log_file:
        print(log_file.read())

for root, dirs, files in os.walk(rootFolderPath):
    for filename in files:
        print_log(os.path.join(root, filename))
