import json

def frame_to_dict(frame):
    out = {}
    out["ether"] = frame.__dict__["fields"]
    if frame.__dict__["fields"]["type"] == 2048:
        out["ip"] = frame.__dict__["payload"].__dict__["fields"]
        if frame.__dict__["payload"].__dict__["fields"]["proto"] == 6:
            out["tcp"] = frame.__dict__["payload"].__dict__["payload"].__dict__["fields"]
    return out


#TODO: elasticsearch.exceptions.SerializationError: ({'time': datetime.datetime(2017, 11, 20, 14, 58, 19, 876762), 'frame': {'ether': {'dst': '84:a4:23:5d:91:8a', 'src': '90:b1:1c:68:9a:d4', 'type': 2048}, 'ip': {'version': 4, 'ihl': 5, 'tos': 0, 'len': 60, 'id': 51700, 'flags': 2, 'frag': 0, 'ttl': 64, 'proto': 6, 'chksum': 41926, 'src': '192.168.1.17', 'dst': '40.77.226.250', 'options': []}, 'tcp': {'sport': 47954, 'dport': 443, 'seq': 3229322253, 'ack': 0, 'dataofs': 10, 'reserved': 0, 'flags': 2, 'window': 29200, 'chksum': 52527, 'urgptr': 0, 'options': [('MSS', 1460), ('SAckOK', b''), ('Timestamp', (1229366708, 0)), ('NOP', None), ('WScale', 7)]}}}, TypeError("Unable to serialize b'' (type: <class 'bytes'>)",))
