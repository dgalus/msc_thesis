import requests
import json

UNSAFE_URLS_LINK = "https://openphish.com/feed.txt"

def get_unsafe_urls():
    contents = requests.get(UNSAFE_URLS_LINK).text
    urls = [x for x in contents.split('\n') if x]
    return urls

