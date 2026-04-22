import socket
import time

HOST = "0.0.0.0"
PORT = 9000

# Create TCP socket
server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

server.bind((HOST, PORT))
server.listen(1)

print("TCP Server listening on port 9000...")

conn, addr = server.accept()
print(f"Client connected: {addr}")

# ----------------------------
# Packets (HEX STREAM)
# ----------------------------

# Packet 1: "Hello world"
packet1 = bytes([
    0x01,0x00,0x12,0x63,0x00,0x80,0x0B,
    0x48,0x65,0x6C,0x6C,0x6F,0x20,
    0x77,0x6F,0x72,0x6C,0x64
])

# Packet 2: Delete message
packet2 = bytes([0x02,0x00,0x04,0x63])

start = time.time()
sent_delete = False

try:
    while True:
        elapsed = int(time.time() - start)

        # Send delete packet after 17 seconds
        if elapsed >= 17 and not sent_delete:
            conn.sendall(packet2)
            print("[SEND] Delete message packet")
            sent_delete = True

        # Send packet1 continuously
        if not sent_delete or elapsed >= 27:
            conn.sendall(packet1)
            print("[SEND] Message packet")

        time.sleep(0.5)

except (BrokenPipeError, ConnectionResetError):
    print("Client disconnected")

finally:
    conn.close()
    server.close()