#include <badgewars.h>

/* Initialize the BadgeWars world */
void bw_init(struct bw_world *world)
{
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