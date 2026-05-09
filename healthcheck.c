#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

int main(void) {
    const char *listen_on = getenv("LISTEN_ON");
    if (!listen_on || strncmp(listen_on, "unix://", 7) != 0) {
        // The API is now expected to listen on a Unix Domain Socket.
        // If the LISTEN_ON env var is not set correctly, this healthcheck
        // must fail.
        return 1;
    }

    const char *socket_path = listen_on + 7; // Skip "unix://"

    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd < 0) {
        return 1;
    }

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path) - 1);

    if (connect(fd, (struct sockaddr *)&addr, sizeof(addr)) != 0) {
        close(fd);
        return 1;
    }

    const char *request = "GET /ready HTTP/1.1\r\nHost: localhost\r\nConnection: close\r\n\r\n";
    if (write(fd, request, strlen(request)) < 0) {
        close(fd);
        return 1;
    }

    char buffer[256];
    ssize_t len = read(fd, buffer, sizeof(buffer) - 1);
    close(fd);
    if (len <= 0) return 1;
    buffer[len] = '\0';

    if (strncmp(buffer, "HTTP/1.1 2", 10) != 0 && strncmp(buffer, "HTTP/1.0 2", 10) != 0) return 1;
    return 0;
}
