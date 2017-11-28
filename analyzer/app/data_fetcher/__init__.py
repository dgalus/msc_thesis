from . import malc0de
from . import malwaredomainlist
from . import openphish
from elasticsearch import Elasticsearch
from elasticsearch.helpers import bulk
import itertools

class DataFetcher:
    es = None

    def __init__(self, es):
        self.es = es

    def insert_unsafe_urls(self, unsafe="unsafe", unsafe_url="url"):
        unsafe_urls = list(itertools.chain(malwaredomainlist.get_unsafe_domains(), openphish.get_unsafe_urls()))
        actions = [
            {
                "_index" : unsafe,
                "_type" : unsafe_url,
                "_id" : i,
                "url" : unsafe_urls[i]
            }            
            for i in range(0, len(unsafe_urls))
        ]
        bulk(self.es, actions)
        

    def insert_unsafe_addresses(self, unsafe="unsafe", unsafe_ip="ip"):
        unsafe_addresses = list(itertools.chain(malc0de.get_unsafe_addresses(), malwaredomainlist.get_unsafe_ip_addresses()))
        actions = [
            {
                "_index" : unsafe,
                "_type" : unsafe_ip,
                "_id" : i,
                "ip" : unsafe_addresses[i]
            }
            for i in range(0, len(unsafe_addresses))
        ]
        bulk(self.es, actions)