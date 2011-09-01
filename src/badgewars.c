#include <badgewars.h>
#include <string.h>

#include <stdio.h> /* Debugging only */

#define MODC(n) (n % BW_CORE_SIZE)
#define MODQ(n) (n % BW_QUEUE_SIZE)

#define INCWRAP(n, modulus) ((n + 1) % modulus)

/* Initialize the BadgeWars world */
void bw_init(struct bw_world *world)
{
    memset(&world->core, 0, sizeof(world->core));
    world->queue_head = world->queue_tail = 0;
}

/* Tell the BadgeWars world to execute a single instruction */
int bw_step(struct bw_world *world)
{
    unsigned int lhs, rhs, continue_task = 1;
    CELLPTR addr;
    CELL *opcode;
    
    if(world->queue_head == world->queue_tail)
        return 0;
        
    addr = world->queue[world->queue_head];    
    opcode = (CELL *)&world->core[addr];
    printf("*** bw_run -- op: 0x%x mode: %x lhs: 0x%x rhs: 0x%x\n", opcode->op, opcode->mode, opcode->lhs, opcode->rhs);
    
    lhs = MODC(world->queue_head + opcode->lhs);
    rhs = MODC(world->queue_head + opcode->rhs);
    
    switch(opcode->op) {
        case OP_DAT:
            /* Executing the DAT instruction terminates the current task */
            continue_task = 0;
            break;
        case OP_MOV:
            memcpy(&world->core[rhs], &world->core[lhs], sizeof(CELL));
            break;
        case OP_ADD:
            world->core[rhs].lhs = MODC(world->core[rhs].lhs + world->core[lhs].lhs);
            world->core[rhs].rhs = MODC(world->core[rhs].rhs + world->core[lhs].rhs);
            break;
    }
    
    /* If we should continue this task, add a new entry to the task queue at
       at the next instruction after this one */
    if(continue_task)
        bw_spawn(world, MODC(addr + 1));
        
    world->queue_head = MODQ(world->queue_head + 1);
    
    return 1;
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