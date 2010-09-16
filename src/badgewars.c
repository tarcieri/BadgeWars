#include <badgewars.h>
#include <stdio.h>

/* Initialize the BadgeWars world */
void bw_init(struct bw_world *world)
{
    puts("bw_init requested");
}

/* Run the BadgeWars world for a single instruction */
int bw_run(struct bw_world *world)
{
    puts("bw_run requested");
    return 0;
}

/* Receive a BadgeWars command from the outside world */
void bw_receive(struct bw_world *world, OPCODE command, void *addr, void(*send_response)(int, void *))
{
    printf("bw_receive got op: %d\n", command.op);
}

/* Peek into the core state */
CELL bw_peek(struct bw_world *world, CELLPTR addr)
{
    printf("bw_peek(%d) called\n", addr);
    return 0;
}