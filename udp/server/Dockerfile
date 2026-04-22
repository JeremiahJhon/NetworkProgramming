FROM gcc:latest

WORKDIR /app

COPY server.cpp .

RUN g++ server.cpp -o server

CMD ["./server"]