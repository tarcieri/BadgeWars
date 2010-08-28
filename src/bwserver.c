#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <fcntl.h>

#include <badgewars.h>

int create_udp_listener(long addr, int port);

int main(int argc, char **argv)
{
    int listener;
    struct bw_world world;

    printf("Starting BadgeWars server...\n");
    printf("Core size:        %d cells\n", BW_CORE_SIZE);
    printf("World image size: %d bytes\n", (int)sizeof(struct bw_world));
    
    listener = create_udp_listener(INADDR_ANY, 3579);
    bw_init(&world);

    return 0;
}

int create_udp_listener(long addr, int port)
{
	int sock;
    struct sockaddr_in servaddr;
    socklen_t len;
    
	sock = socket(AF_INET, SOCK_DGRAM, 0);
    
    len = sizeof(servaddr);
    memset(&servaddr, 0, len);
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(addr);
    servaddr.sin_port = htons(port);
    bind(sock, (struct sockaddr *)&servaddr, len);
    
    return sock;
}