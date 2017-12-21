#!/bin/bash
curl -X GET 'http://localhost:9200/sniffer/frame/_search?pretty' -H "Content-Type: application/json" -d '
{
  "query": {
    "bool": {
      "must" : {
        "exists" : {
          "field" : "tcp"
        }
      }, 
      "must_not": [
        {
          "match": {
            "ip.src": "10.200.240.72"
          }
        },
        {
          "match" : {
            "ip.dest" : "10.200.240.72"
          }
        }
      ]
    }
  }
}'
