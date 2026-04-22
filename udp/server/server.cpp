#include <iostream>
#include <thread>
#include <chrono>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        std::cerr << "Socket creation failed\n";
        return 1;
    }

    // Bind server to 0.0.0.0:9000
    sockaddr_in server{};
    server.sin_family = AF_INET;
    server.sin_port = htons(9000);
    server.sin_addr.s_addr = INADDR_ANY;

    if (bind(sock, (sockaddr*)&server, sizeof(server)) < 0) {
        std::cerr << "Bind failed\n";
        return 1;
    }

    std::cout << "UDP Server started on port 9000...\n";

    // ----------------------------
    // Wait for client handshake
    // ----------------------------
    sockaddr_in client{};
    socklen_t client_len = sizeof(client);
    char buffer[1024];

    int bytes = recvfrom(sock, buffer, sizeof(buffer), 0,
                         (sockaddr*)&client, &client_len);

    if (bytes < 0) {
        std::cerr << "Failed to receive handshake\n";
        return 1;
    }

    std::cout << "Client connected (UDP)\n";

    // ----------------------------
    // Packet definitions
    // ----------------------------
    unsigned char packet1[] = {
        0x01,0x00,0x12,0x63,0x00,0x80,0x0B,
        0x48,0x65,0x6C,0x6C,0x6F,0x20,
        0x77,0x6F,0x72,0x6C,0x64
    };

    unsigned char packet2[] = {0x02,0x00,0x04,0x63};

    auto start = std::chrono::steady_clock::now();
    bool sentDelete = false;

    while (true) {
        auto now = std::chrono::steady_clock::now();
        auto seconds =
            std::chrono::duration_cast<std::chrono::seconds>(now - start).count();

        // Send delete after 17 seconds
        if (seconds >= 17 && !sentDelete) {
            sendto(sock, packet2, sizeof(packet2), 0,
                   (sockaddr*)&client, client_len);
            std::cout << "[SEND] Delete message\n";
            sentDelete = true;
        }

        // Send message continuously
        if (!sentDelete || seconds >= 27) {
            sendto(sock, packet1, sizeof(packet1), 0,
                   (sockaddr*)&client, client_len);
            std::cout << "[SEND] Message packet\n";
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    close(sock);
    return 0;
}