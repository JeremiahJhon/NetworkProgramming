#include <iostream>
#include <thread>
#include <chrono>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

int main() {
    // Create UDP socket
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        std::cerr << "Socket creation failed\n";
        return 1;
    }

    // ----------------------------
    // Resolve Docker service name
    // ----------------------------
    struct addrinfo hints{}, *res;

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    if (getaddrinfo("udp-client", "9000", &hints, &res) != 0) {
        std::cerr << "DNS resolution failed\n";
        return 1;
    }

    sockaddr_in client{};
    client = *(sockaddr_in*)res->ai_addr;
    freeaddrinfo(res);

    // ----------------------------
    // Packet 1: "Hello world"
    // ----------------------------
    unsigned char packet1[] = {
        0x01,0x00,0x12,0x63,0x00,0x80,0x0B,
        0x48,0x65,0x6C,0x6C,0x6F,0x20,
        0x77,0x6F,0x72,0x6C,0x64
    };

    // Packet 2: Delete message
    unsigned char packet2[] = {0x02,0x00,0x04,0x63};

    auto start = std::chrono::steady_clock::now();
    bool sentDelete = false;

    std::cout << "UDP Server started...\n";

    while (true) {
        auto now = std::chrono::steady_clock::now();
        auto seconds =
            std::chrono::duration_cast<std::chrono::seconds>(now - start).count();

        // Send delete packet after 17 seconds
        if (seconds >= 17 && !sentDelete) {
            sendto(sock, packet2, sizeof(packet2), 0,
                   (sockaddr*)&client, sizeof(client));
            std::cout << "[SEND] Delete message packet\n";
            sentDelete = true;
        }

        // Send packet 1 continuously:
        // from start and resume after 27 seconds
        if (!sentDelete || seconds >= 27) {
            sendto(sock, packet1, sizeof(packet1), 0,
                   (sockaddr*)&client, sizeof(client));
            std::cout << "[SEND] Message packet\n";
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    close(sock);
    return 0;
}