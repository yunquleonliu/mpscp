#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>

int main() {
    int sockfd;
    struct sockaddr_in dest_addr;
    struct ifreq ifr;

    // Create a socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Specify the interface to bind to
    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, "eth3", IFNAMSIZ - 1); // Replace "eth3" with your interface name

    if (setsockopt(sockfd, SOL_SOCKET, SO_BINDTODEVICE, (void *)&ifr, sizeof(ifr)) < 0) {
        perror("setsockopt");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Set up the destination address
    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(80); // Replace with your desired port
    inet_pton(AF_INET, "93.184.216.34", &dest_addr.sin_addr); // Replace with your destination IP

    // Connect to the destination
    if (connect(sockfd, (struct sockaddr *)&dest_addr, sizeof(dest_addr)) < 0) {
        perror("connect");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Now you can send and receive data using sockfd

    close(sockfd);
    return 0;
}

