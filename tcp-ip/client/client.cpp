#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

constexpr int PORT = 9000;
constexpr const char* HOST = "tcp-server";

// -------------------- Decode --------------------
std::string decode(const std::vector<unsigned char>& data) {
    if (data.empty()) return "";

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

// -------------------- Resolve + Connect --------------------
int connect_to_server() {
    addrinfo hints{}, *res, *p;

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    int status = getaddrinfo(HOST, std::to_string(PORT).c_str(), &hints, &res);
    if (status != 0) {
        std::cerr << "getaddrinfo failed: " << gai_strerror(status) << "\n";
        return -1;
    }

    int sock = -1;

    for (p = res; p != nullptr; p = p->ai_next) {
        sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sock < 0) continue;

        if (connect(sock, p->ai_addr, p->ai_addrlen) == 0) {
            break; // success
        }

        close(sock);
        sock = -1;
    }

    freeaddrinfo(res);

    return sock;
}

// -------------------- Main --------------------
int main() {
    int sock = -1;

    // Retry loop (important for Docker startup timing)
    for (int i = 0; i < 5; ++i) {
        sock = connect_to_server();
        if (sock >= 0) break;

        std::cerr << "Retrying connection...\n";
        sleep(1);
    }

    if (sock < 0) {
        std::cerr << "Failed to connect to server\n";
        return 1;
    }

    std::cout << "Connected to server...\n";

    std::vector<unsigned char> buffer(2048);
    std::vector<unsigned char> streamBuffer;

    while (true) {
        int bytes = recv(sock, buffer.data(), buffer.size(), 0);

        if (bytes == 0) {
            std::cout << "Server closed connection\n";
            break;
        }

        if (bytes < 0) {
            std::cerr << "recv error\n";
            break;
        }

        // Append incoming data
        streamBuffer.insert(streamBuffer.end(),
                            buffer.begin(),
                            buffer.begin() + bytes);

        // Process complete messages
        while (true) {
            if (streamBuffer.empty()) break;

            int pid = streamBuffer[0];
            size_t total_len = 0;

            if (pid == 1) {
                if (streamBuffer.size() < 7) break;

                int text_len = streamBuffer[6];
                total_len = 7 + text_len;
            }
            else if (pid == 2) {
                total_len = 4;
            }
            else {
                // Unknown packet → resync
                std::cerr << "Unknown PID, dropping byte\n";
                streamBuffer.erase(streamBuffer.begin());
                continue;
            }

            if (streamBuffer.size() < total_len) break;

            // Extract one full message
            std::vector<unsigned char> message(
                streamBuffer.begin(),
                streamBuffer.begin() + total_len
            );

            std::string result = decode(message);
            if (!result.empty()) {
                std::cout << result << std::endl;
            }

            // Remove processed bytes
            streamBuffer.erase(streamBuffer.begin(),
                               streamBuffer.begin() + total_len);
        }
    }

    close(sock);
    return 0;
}