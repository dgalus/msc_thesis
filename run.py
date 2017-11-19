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
df.insert_unsafe_urls()
df.insert_unsafe_addresses()