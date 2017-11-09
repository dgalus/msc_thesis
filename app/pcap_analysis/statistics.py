from scapy.all import *

packets = rdpcap(sys.argv[1])

for packet in packets:
    print(packet)