#include <badgewars.h>
#include <string.h>

#include <stdio.h> /* Debugging only */

#define INCWRAP(n, modulus) ((n + 1) % modulus)

/* Initialize the BadgeWars world */
void bw_init(struct bw_world *world)
{
    memset(&world->core, 0, sizeof(world->core));
    world->queue_head = world->queue_tail = 0;
}

/* Run the BadgeWars world for a single instruction */
int bw_run(struct bw_world *world)
{
    CELLPTR addr;
    CELL *opcode;
    
    if(world->queue_head == world->queue_tail)
        return 0;
        
    addr = world->queue[world->queue_head];
    world->queue_head = INCWRAP(world->queue_head, BW_QUEUE_SIZE);
    
    opcode = (CELL *)&world->core[addr];
    printf("*** bw_run -- op: 0x%x mode: %x lhs: 0x%x rhs: 0x%x\n", opcode->op, opcode->mode, opcode->lhs, opcode->rhs);
    
    return 1;
}

/* Receive a BadgeWars command from the outside world */
void bw_receive(struct bw_world *world, CELL command, void *addr, void(*send_response)(int, void *))
{
}

/* Peek into the core state */
CELL *bw_peek(struct bw_world *world, CELLPTR addr)
{
    return &world->core[addr % BW_CORE_SIZE];
}

/* Modify the core state */
void bw_poke(struct bw_world *world, CELLPTR addr, CELL value)
{
    memcpy(&world->core[addr % BW_CORE_SIZE], &value, sizeof(CELL));
}

/* Add a new process to the queue */
void bw_spawn(struct bw_world *world, CELLPTR addr)
{
    world->queue[world->queue_tail] = addr;
    world->queue_tail = INCWRAP(world->queue_tail, BW_QUEUE_SIZE);
}