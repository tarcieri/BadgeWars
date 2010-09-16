#include <badgewars.h>
#include <string.h>

/* Initialize the BadgeWars world */
void bw_init(struct bw_world *world)
{
    memset(&world->core, 0, sizeof(world->core));
    world->queue_head = world->queue_tail = 0;
}

/* Run the BadgeWars world for a single instruction */
int bw_run(struct bw_world *world)
{
    return 0;
}

/* Receive a BadgeWars command from the outside world */
void bw_receive(struct bw_world *world, OPCODE command, void *addr, void(*send_response)(int, void *))
{
}

/* Peek into the core state */
CELL bw_peek(struct bw_world *world, CELLPTR addr)
{
    return world->core[addr];
}