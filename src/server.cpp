#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <sstream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "audiostreamer.h"
#include "audioanalytics.h"

#define DEFAULT_PORT 27015
#define BUFFER_SIZE 1024

// Represents a server which can analyze audio files sent by clients
class FrettysEssentia {
    public:
        FrettysEssentia() {
            startup();

            while (true) {
                acceptConnection();
                receiveData();
                processAudio();
            }
        }

        ~FrettysEssentia() {
            stop();
        }

    private:
        int servserSocket, clientSocket;
        struct sockaddr_in serverAddr, clientAddr;
        socklen_t clientAddrLen = sizeof(clientAddr);
        char buffer[BUFFER_SIZE];

        void startup() {
            servserSocket = socket(AF_INET, SOCK_STREAM, 0);
            if (servserSocket == -1) {
                std::cerr << "Error creating socket\n";
                return;
            }

            serverAddr.sin_family = AF_INET;
            serverAddr.sin_addr.s_addr = INADDR_ANY;
            serverAddr.sin_port = htons(DEFAULT_PORT);

            if (bind(servserSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
                std::cerr << "Bind failed\n";
                return;
            }

            listen(servserSocket, 5);
        }

        void acceptConnection() {
            clientSocket = accept(servserSocket, (struct sockaddr *)&clientAddr, &clientAddrLen);
            if (clientSocket < 0) {
                std::cerr << "Accept failed\n";
                return;
            }

            std::cout << "Client connected" << std::endl;
        }

        void receiveData() {
            std::ofstream outputFile("./audio.mp3", std::ios::binary);

            int bytesRead;
            if (!outputFile) {
                std::cerr << "Error opening file for writing." << std::endl;
                return;
            }

            while ((bytesRead = recv(clientSocket, buffer, BUFFER_SIZE, 0)) > 0) {
                outputFile.write(buffer, bytesRead);
                if (!outputFile) {
                    std::cerr << "Error writing to file." << std::endl;
                    break;
                }
            }

            outputFile.close();
            std::cout << "File Received" << std::endl;
        }

        void processAudio() {
            audiostreamer::AudioStreamer audioStreamer;
            audioanalytics::AudioAnalytics audioAnalytics = audioStreamer.processAudio("./audio.mp3");
            std::string forNetwork = audioAnalytics.forNetwork();

            int bytesSent = send(clientSocket, forNetwork.c_str(), forNetwork.size(), 0);
            if (bytesSent == -1) {
                std::cerr << "Error sending data." << std::endl;
            } else {
                std::cout << "Data sent successfully." << std::endl;
            }

            shutdown(clientSocket, SHUT_WR);
        }

        void stop() {
            close(clientSocket);
            close(servserSocket);
        }
};

int main() {
    FrettysEssentia frettysEssentia;
    return 0;
}
