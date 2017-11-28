#from scapy.all import *

#packets = sniff(count=100)
#wrpcap('traffic.pcap', packets)

from threading import Thread
from queue import Queue, Empty
from scapy.all import *

def print_summary(pkt):
    print(pkt.summary())

def threaded_sniff_target(q):
    #sniff(prn = lambda x : q.put(x))
    sniff(prn = lambda x: q.append(x))

def threaded_sniff():
    #q = Queue()
    q = []
    sniffer = Thread(target = threaded_sniff_target, args = (q,))
    sniffer.daemon = True
    sniffer.start()
    #l = []
    while(True):
        try:
            #pkt = q.get(timeout=1)
            # l.append(pkt)
            # if len(l) == 50:
            #     #for i in l:
            #         #print_summary(i)
            #     l = []
            #     print("len = " + str(len(l)))
            if len(q) > 50:
                q = []
        except Empty:
            pass

threaded_sniff()
