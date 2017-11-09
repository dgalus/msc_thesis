from scapy.all import *

packets = sniff(count=100)
wrpcap('trafiic.pcap', packets)
