import yaml
from elasticsearch import Elasticsearch
from app.data_fetcher import DataFetcher

def read_config():
    with open("config.yml", 'r') as stream:
        try:
            return yaml.load(stream)
        except yaml.YAMLError as exc:
            print(exc)

config = read_config()
es = Elasticsearch([{'host' : config['elasticsearch']['host'], 'port' : config['elasticsearch']['port']}])

df = DataFetcher(es)
df.insert_unsafe_urls(unsafe=config['elasticsearch']['unsafe_index'], unsafe_url=config['elasticsearch']['unsafe_url'])
df.insert_unsafe_addresses(unsafe=config['elasticsearch']['unsafe_index'], unsafe_ip=config['elasticsearch']['unsafe_ip'])
ret = es.search(index='unsafe', doc_type='ip', q='ip:*1*', size=1000)
print(len(ret['hits']['hits']))