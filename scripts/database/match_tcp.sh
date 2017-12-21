#!/bin/bash
curl -X GET 'http://localhost:9200/sniffer/frame/_search?pretty' -H "Content-Type: application/json" -d '
{
  "query": {
    "bool": {
      "must" : {
        "exists" : {
          "field" : "tcp"
        }
      }
    }
  }
}'
