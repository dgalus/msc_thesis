from scapy.all import *

packets = sniff(count=100)
wrpcap('traffic.pcap', packets)

