CLASS DIAGRAM:

+---------------------------+
|       TcpServer           |
+---------------------------+
| - serverSocket            |
| - clientSocket            |
| - clientAddr              |
+---------------------------+
| + start()                 |
| + acceptClient()          |
| + sendPacket()            |
| + scheduleStream()        |
+---------------------------+
            |
            | uses
            v
+---------------------------+
|      PacketBuilder        |
+---------------------------+
| + buildPacket1()          |
| + buildPacket2()          |
| + encodeHexStream()       |
| + addHeader()             |
+---------------------------+

-------------------------------------------------------

+---------------------------+
|       TcpClient           |
+---------------------------+
| - socketFd               |
| - buffer                 |
| - lastMessage            |
+---------------------------+
| + connect()              |
| + receiveStream()        |
| + readBuffer()           |
| + decodePacket()         |
| + printMessage()         |
+---------------------------+
            |
            | uses
            v
+---------------------------+
|     PacketDecoder         |
+---------------------------+
| + extractHeader()         |
| + parseStream()           |
| + handlePartialPackets()  |
| + interpretPacket()       |
+---------------------------+

BLOCK DIAGRAM:

            ┌──────────────────────┐
            │     TCP Server       │
            │ (Connection Oriented)│
            └─────────┬────────────┘
                      │
              TCP Handshake (SYN/SYN-ACK/ACK)
                      │
        ┌─────────────▼─────────────┐
        │     Reliable Stream        │
        │   (Byte Stream / Socket)   │
        └─────────────┬─────────────┘
                      │
        ┌─────────────▼─────────────┐
        │     TCP Client             │
        │  (Persistent Connection)   │
        └─────────────┬─────────────┘
                      │
            ┌─────────▼─────────┐
            │ Packet Decoder    │
            │ - stream buffer   │
            │ - reassembly      │
            │ - parsing logic   │
            └─────────┬─────────┘
                      │
            ┌─────────▼─────────┐
            │ Application Layer │
            │ output display    │
            └───────────────────┘

BRIEF RATIONALE:
- It is much more reliable since the transmission is in order and connection-based (needs handshake).
- It ensures that the HEX stream is correct and complete.