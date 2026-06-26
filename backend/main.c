#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

// Notice: We removed the #define PORT 8080 line!

double calculate(double num1, char operator, double num2) {
    switch(operator) {
        case 'p': return num1 + num2;
        case 'm': return num1 - num2;
        case 'x': return num1 * num2;
        case 'd': 
            if (num2 == 0.0) return 0.0;
            return num1 / num2;
        default: return 0.0;
    }
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0}; 

    // --- NEW CLOUD PORT LOGIC ---
    int port = 8080; // Default to 8080 if we are testing locally
    char *env_port = getenv("PORT"); // Ask the cloud computer for a port
    if (env_port != NULL) {
        port = atoi(env_port); // Convert the cloud's text port into a number
    }
    // ----------------------------

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port); // Use the dynamic port variable here!
    
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);
    
    // Print the port so we can see it in Railway's logs later
    printf("Math Server listening on port %d\n", port);

    while(1) {
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        
        memset(buffer, 0, 1024);
        read(new_socket, buffer, 1024);
        
        double n1 = 0, n2 = 0;
        char op = 0;
        
        if (sscanf(buffer, "GET /?n1=%lf&op=%c&n2=%lf", &n1, &op, &n2) == 3) {
            double result = calculate(n1, op, n2);
            char response[1024];
            
            sprintf(response, "HTTP/1.1 200 OK\r\nAccess-Control-Allow-Origin: *\r\nContent-Type: text/plain\r\n\r\n%.2f", result);
            write(new_socket, response, strlen(response));
        } else {
            char *fallback = "HTTP/1.1 200 OK\r\nAccess-Control-Allow-Origin: *\r\nContent-Type: text/plain\r\n\r\nSend math in the URL!";
            write(new_socket, fallback, strlen(fallback));
        }
        
        close(new_socket);
    }
    return 0;
}