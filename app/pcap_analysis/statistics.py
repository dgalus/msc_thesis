from scapy.all import *

packets = rdpcap(sys.argv[1])

ip_packets = 0

for p in packets:
    if IP in p:
        ip_packets += 1 

print("IP packets count: " + str(ip_packets))