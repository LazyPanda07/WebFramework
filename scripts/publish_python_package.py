import requests
import os

from pathlib import Path

# PyPI API URL for uploads
url = "https://upload.pypi.org/legacy/"

# API token (replace this with your actual API token)
api_token = os.environ["PYPI_API_KEY"]

# Prepare the payload for the request
data = {
    ":action": "file_upload",
}

# Prepare headers
headers = {
    "Authorization": f"Basic {api_token}"
}

# Upload each file
for path, _, file_names in os.walk("dist"):
    for file_name in file_names:
        with open(f"{path}/{file_name}", "rb") as file_content:
            files = {"content": (file_name, file_content)}
            response = requests.post(url, data=data, headers=headers, files=files)
        
            if response.status_code == 200:
                print(f"Successfully uploaded {file_name} to PyPI.")
            else:
                print(f"Failed to upload {file_name}. Status code: {response.status_code}")
                print(response.text)

                exit(1)
