Network theory

2. Please explain about OSI model
    - It is a standard framework or a guideline when designing how data is transmitted over a network.
    - Layers: Application -> Presentation -> Session -> Transport -> Network -> Data Link -> Physical
5. What Linux command lines can be used to examine kernel bootup messages after boot time?
    - dmesg/journalctl -k
6. Please give one advantage of hard links over symbolic links
    - It remains valid even if the original file has been deleted or removed.
7. Suppose that after installing a serial multi-port card on a system, the mouse connected to a serial port
on your motherboard stops functioning. Which of the following commands is MOST likely to be useful in
diagnosing this problem? C
A. setserial /dev/com1 -v auto_irq
B. cat /dev/mouse > serial-info.dump
C. setserial -a /dev/cua0 - (Answer: it displays configuration of all serial ports)
D. ioaddr /dev/ttyS1
8. Which of the following command lines would provide the most useful information in diagnosing a
suspected hardware address conflict between an an EISA ethernet NIC card and a video controller? C
• A. cat /proc/irq
• B. cat /proc/meminfo
• C. cat /proc/iomem - (Answer: Shows memory address allocation)
• D. cat /proc/modules
9. Which TCP/IP network utilities are the BEST tool to establish if a given IP address is reachable under
the current network configuration? 
    - ping, arping
10. Which TCP/IP network utilities are the BEST tool to use to identify bottlenecks between remote
machines on the network? Specifically, assuming a goal in this debugging is to determine the paths
travelled in the forwarding of network packets, and identify intermediate routers that may be dropping
packets.
    - traceroute (Shows each hop between sourec and destination)
