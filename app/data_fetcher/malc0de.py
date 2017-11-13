import requests
import json

UNSAFE_ADDRESSES_LINK = "http://malc0de.com/bl/IP_Blacklist.txt"

def get_unsafe_addresses():
    contents = requests.get(UNSAFE_ADDRESSES_LINK).text
    addresses = [x for x in contents.split('\n') if x and not x.startswith('//')]
    return addresses
