#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

// We updated the operators to letters to avoid URL-encoding bugs!
double calculate(double num1, char operator, double num2) {
    switch(operator) {
        case 'p': return num1 + num2; // p = plus
        case 'm': return num1 - num2; // m = minus
        case 'x': return num1 * num2; // x = multiply
        case 'd':                     // d = divide
            if (num2 == 0.0) return 0.0;
            return num1 / num2;
        default: return 0.0;
    }
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0}; // A text bucket to hold the browser's message

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    
    // Pro-tip: This line prevents the annoying "Address already in use" error 
    // if you restart the server too fast.
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);
    printf("Math Server listening on http://localhost:%d\n", PORT);

    while(1) {
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        
        memset(buffer, 0, 1024);
        read(new_socket, buffer, 1024);
        
        // DEBUG LOG: Print exactly what the browser is asking for to our terminal!
        printf("\n--- NEW REQUEST ---\n%s\n", buffer);
        
        double n1 = 0, n2 = 0;
        char op = 0;
        
        if (sscanf(buffer, "GET /?n1=%lf&op=%c&n2=%lf", &n1, &op, &n2) == 3) {
            double result = calculate(n1, op, n2);
            char response[1024];
            
            // FIX: Upgraded \n to \r\n to strictly follow HTTP rules for Firefox
            sprintf(response, "HTTP/1.1 200 OK\r\nAccess-Control-Allow-Origin: *\r\nContent-Type: text/plain\r\n\r\n%.2f", result);
            
            write(new_socket, response, strlen(response));
            printf("DEBUG: Successfully sent answer: %.2f\n", result);
        } else {
            // FIX: Upgraded \n to \r\n here as well
            char *fallback = "HTTP/1.1 200 OK\r\nAccess-Control-Allow-Origin: *\r\nContent-Type: text/plain\r\n\r\nSend math in the URL! Example: /?n1=10&op=p&n2=5";
            write(new_socket, fallback, strlen(fallback));
            printf("DEBUG: Sent fallback message.\n");
        }
        
        close(new_socket);
    }
    return 0;
}