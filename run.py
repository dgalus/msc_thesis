import yaml
from app.data_fetcher import DataFetcher

def read_config():
    with open("config.yml", 'r') as stream:
        try:
            print(yaml.load(stream))
        except yaml.YAMLError as exc:
            print(exc)

df = DataFetcher(None)
df.insert_unsafe_urls()