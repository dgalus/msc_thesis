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
        for uu in unsafe_urls:
            self.es.index(index='unsafe', doc_type='urls', body={
                'url' : uu
            })
        

    def insert_unsafe_addresses(self):
        unsafe_addresses = list(itertools.chain(malc0de.get_unsafe_addresses(), malwaredomainlist.get_unsafe_ip_addresses()))