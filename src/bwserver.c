#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <fcntl.h>
#include <assert.h>

#include <badgewars.h>

int create_udp_listener(long addr, int port);
int io_check(int listener, struct bw_world *world);
int io_wait(int listener, struct bw_world *world);
void send_response(CELL opcode, void *opaque_addr);

int main(int argc, char **argv)
{
    int listener, ret;
    struct bw_world world;

    printf("Starting BadgeWars server...\n");
    printf("Core size:        %d cells\n", BW_CORE_SIZE);
    printf("World image size: %d bytes\n", (int)sizeof(struct bw_world));
    
    listener = create_udp_listener(INADDR_ANY, 3579);
    bw_init(&world);
    
    for(;;) {
        ret = bw_run(&world);
        if(ret)
            io_check(listener, &world);
        else
            io_wait(listener, &world);
    }

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

int io_check(int listener, struct bw_world *world)
{
    return 0;
}

int io_wait(int listener, struct bw_world *world)
{
    OPCODE opcode;
    int n;
    struct sockaddr_in addr;
    socklen_t len;
    void(*callback)(int, void *);
    
    /* BadgeWars opcodes must be 32-bit */
    assert(sizeof(OPCODE) == 4);
    
    len = sizeof(&addr);
    n = recvfrom(listener, &opcode, sizeof(OPCODE), 0, (struct sockaddr *)&addr, &len);
    
    bw_receive(world, opcode, &addr, callback);
    
    return 0;
}

void send_response(CELL opcode, void *opaque_addr)
{
    struct sockaddr_in *addr = (struct sockaddr_in *)addr;
}