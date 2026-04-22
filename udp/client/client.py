import socket

SERVER_HOST = "udp-server"  # Docker service name
SERVER_PORT = 9000

LOCAL_HOST = "0.0.0.0"
LOCAL_PORT = 9000

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((LOCAL_HOST, LOCAL_PORT))

last_message = None


def decode_packet(data: bytes):
    try:
        if len(data) < 1:
            return None

        pid = data[0]

        # -------------------- PID 1 --------------------
        if pid == 1:
            if len(data) < 7:
                return None

            msg_id = data[3]

            attr_format = int.from_bytes(data[4:6], "big")
            color = (attr_format >> 2) & 0x3FF

            text_len = data[6]

            if len(data) < 7 + text_len:
                return None

            text_bytes = data[7:7 + text_len]

            try:
                text = text_bytes.decode("latin-1")
            except UnicodeDecodeError:
                return None

            color_map = {
                0: "White",
                1: "Black",
                2: "Red"
            }

            return f'Receive message ID {msg_id} "{text}" with {color_map.get(color, "Unknown")} color'

        # -------------------- PID 2 --------------------
        elif pid == 2:
            if len(data) < 4:
                return None

            msg_id = data[3]
            return f"Delete message {msg_id}"

        return None

    except Exception as e:
        print(f"Decode error: {e}")
        return None


print(f"UDP Client listening on {LOCAL_HOST}:{LOCAL_PORT}...")

# ----------------------------
# Send handshake to server
# ----------------------------
sock.sendto(b"hello", (SERVER_HOST, SERVER_PORT))
print("Sent handshake to server")

while True:
    try:
        data, addr = sock.recvfrom(2048)

        print(f"[RECV] {len(data)} bytes from {addr}")  # debug

        msg = decode_packet(data)

        if msg and msg != last_message:
            print(msg)
            last_message = msg

    except KeyboardInterrupt:
        print("\nShutting down client...")
        break

    except Exception as e:
        print(f"Runtime error: {e}")