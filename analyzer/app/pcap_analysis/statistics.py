from scapy.all import *

packets = rdpcap(sys.argv[1])

ip_packets = 0

for p in packets:
    if p.haslayer(IP):
        ip_packets += 1 

print("IP packets count: " + str(ip_packets))