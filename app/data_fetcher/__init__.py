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

    def insert_unsafe_urls(self):
        unsafe_urls = list(itertools.chain(malwaredomainlist.get_unsafe_domains(), openphish.get_unsafe_urls()))
        actions = [
            {
                "_index" : "unsafe",
                "_type" : "url",
                "_id" : i,
                "_url" : unsafe_urls[i]
            }            
            for i in range(0, len(unsafe_urls))
        ]
        bulk(self.es, actions)
        

    def insert_unsafe_addresses(self):
        unsafe_addresses = list(itertools.chain(malc0de.get_unsafe_addresses(), malwaredomainlist.get_unsafe_ip_addresses()))
        actions = [
            {
                "_index" : "unsafe",
                "_type" : "ip",
                "_id" : i,
                "_ip" : unsafe_addresses[i]
            }
            for i in range(0, len(unsafe_addresses))
        ]
        bulk(self.es, actions)