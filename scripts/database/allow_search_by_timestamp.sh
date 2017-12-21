#!/bin/bash
curl -XPUT 'localhost:9200/sniffer/_mapping/frame?pretty' -H 'Content-Type: application/json' -d'
{
  "properties": {
    "datetime": { 
      "type":     "text",
      "fielddata": true
    }
  }
}
'

