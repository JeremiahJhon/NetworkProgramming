import socket

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind(("0.0.0.0", 9000))  # important for Docker

last_message = None

def decode_packet(data):
    pid = data[0]

    if pid == 1:
        msg_id = data[3]
        attr_format = int.from_bytes(data[4:6], "big")

        color = (attr_format >> 2) & 0x3FF
        fmt = attr_format & 0x3

        text_len = data[6]
        text = data[7:7+text_len].decode("latin-1")

        color_map = {0:"White",1:"Black",2:"Red"}

        return f'Receive message ID {msg_id} "{text}" with {color_map.get(color,"Unknown")} color'

    elif pid == 2:
        msg_id = data[3]
        return f"Delete message {msg_id}"

    return None


print("UDP Client listening on port 9000...")

while True:
    data, _ = sock.recvfrom(1024)
    msg = decode_packet(data)

    if msg and msg != last_message:
        print(msg)
        last_message = msg