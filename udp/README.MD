CLASS DIAGRAM:


+------------------------+
|      UdpServer         |
+------------------------+
| - socketFd             |
| - clientAddr           |
| - packetScheduler      |
+------------------------+
| + start()              |
| + sendPacket1()        |
| + sendPacket2()        |
| + scheduleLoop()       |
+------------------------+
            |
            | uses
            v
+------------------------+
|   PacketBuilder        |
+------------------------+
| + buildPacket1()       |
| + buildPacket2()       |
| + encodeHexStream()    |
+------------------------+

------------------------------------------------------

+------------------------+
|      UdpClient         |
+------------------------+
| - socketFd             |
| - lastMessage          |
+------------------------+
| + start()              |
| + receive()            |
| + decodePacket()       |
| + printMessage()       |
+------------------------+
            |
            | uses
            v
+------------------------+
|    PacketDecoder       |
+------------------------+
| + parseHeader()        |
| + parsePayload()       |
| + extractText()        |
| + interpretPacket()    |
+------------------------+

BLOCK DIAGRAM:

            ┌──────────────────────┐
            │     UDP Server       │
            │  (C++ or Python)     │
            └─────────┬────────────┘
                      │
          HEX STREAM (UDP Datagram)
                      │
        ┌─────────────▼─────────────┐
        │     Network Layer         │
        │   (Docker Bridge / LAN)   │
        └─────────────┬─────────────┘
                      │
        ┌─────────────▼─────────────┐
        │      UDP Client           │
        │   (C++ Decoder App)       │
        └─────────────┬─────────────┘
                      │
            ┌─────────▼─────────┐
            │ Packet Decoder    │
            │ - HEX parsing     │
            │ - Field extract   │
            └─────────┬─────────┘
                      │
            ┌─────────▼─────────┐
            │ Console Output    │
            │ "Message printed" │
            │ "Delete event"    │
            └───────────────────┘

BRIEF RATIONALE:
- UDP is lightweight and fast
- No connection overhead (no handshake)
- Suitable for real-time streaming / telemetry / signaling systems
- Accepts possible packet loss (acceptable in this design)
- HEX format enables Compact binary representation, reduces bandwidth usage. Thus, it makes it easier to control packet structure and is very common in embedded / railway / telecom systems
