#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>

std::string decode(const std::vector<unsigned char>& data) {
    if (data.size() < 4) return "";

    int pid = data[0];

    if (pid == 1) {
        if (data.size() < 7) return "";

        int msg_id = data[3];
        int attr_format = (data[4] << 8) | data[5];
        int color = (attr_format >> 2) & 0x3FF;

        int text_len = data[6];

        if (data.size() < 7 + text_len) return "";

        std::string text(data.begin() + 7, data.begin() + 7 + text_len);

        std::string color_str =
            (color == 2) ? "Red" :
            (color == 1) ? "Black" : "White";

        return "Receive message ID " + std::to_string(msg_id) +
               " \"" + text + "\" with " + color_str + " color";
    }
    else if (pid == 2) {
        if (data.size() < 4) return "";
        int msg_id = data[3];
        return "Delete message " + std::to_string(msg_id);
    }

    return "";
}

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in server{};
    server.sin_family = AF_INET;
    server.sin_port = htons(9000);

    inet_pton(AF_INET, "tcp-server", &server.sin_addr);

    if (connect(sock, (sockaddr*)&server, sizeof(server)) < 0) {
        std::cerr << "Connection failed\n";
        return 1;
    }

    std::cout << "Connected to server...\n";

    std::vector<unsigned char> buffer(2048);
    std::vector<unsigned char> streamBuffer;

    while (true) {
        int bytes = recv(sock, buffer.data(), buffer.size(), 0);
        if (bytes <= 0) break;

        // Append stream data
        streamBuffer.insert(streamBuffer.end(),
                            buffer.begin(),
                            buffer.begin() + bytes);

        // Try decode repeatedly (simple approach)
        std::string msg = decode(streamBuffer);

        if (!msg.empty()) {
            std::cout << msg << std::endl;
            streamBuffer.clear(); // reset after successful decode
        }
    }

    close(sock);
    return 0;
}