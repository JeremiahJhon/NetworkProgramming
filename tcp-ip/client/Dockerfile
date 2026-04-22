FROM gcc:latest

WORKDIR /app
COPY client.cpp .

RUN g++ client.cpp -o client

CMD ["./client"]