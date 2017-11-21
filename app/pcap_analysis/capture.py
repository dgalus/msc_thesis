#from scapy.all import *

#packets = sniff(count=100)
#wrpcap('traffic.pcap', packets)

from threading import Thread
from queue import Queue, Empty
from scapy.all import *

def print_summary(pkt):
    print(pkt.summary())

def threaded_sniff_target(q):
    sniff(prn = lambda x : q.put(x))

def threaded_sniff():
    q = Queue()
    sniffer = Thread(target = threaded_sniff_target, args = (q,))
    sniffer.daemon = True
    sniffer.start()
    while(True):
        try:
            pkt = q.get(timeout=1)
            print_summary(pkt)
        except Empty:
            pass

#threaded_sniff()
