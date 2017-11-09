import pyshark

cap = pyshark.FileCapture('/home/avc/tmp/traffic.pcap')
print(cap)

print(cap[1])

print(len(cap))