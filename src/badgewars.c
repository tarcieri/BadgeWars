#include <badgewars.h>
#include <string.h>

#include <stdio.h> /* Debugging only */

#define INITQ(q) ((q)->head = (q)->tail = 0) /* Initialize a bw_queue */
#define EMPTYQ(q) ((q)->head == (q)->tail) /* Is the queue empty */

#define MODC(n) (n % BW_CORE_SIZE)  /* Modulo the core size */
#define MODQ(n) (n % BW_QUEUE_SIZE) /* Modulo the world size */
#define INCC(n) (MODC(n + 1))       /* Increment modulo core size */
#define INCQ(n) (MODQ(n + 1))       /* Increment modulo queue size */

static int bw_exec(CELL *core, QUEUE *queue);

/* Initialize the BadgeWars world */
void bw_init(struct bw_world *world)
{
    memset(&world->core, 0, sizeof(world->core));
	INITQ(&world->p0_queue);
	INITQ(&world->p1_queue);
}


/* Process the next instruction for both players */
int bw_step(struct bw_world *world)
{
	if(!bw_exec(world->core, &world->p0_queue))
		return 0;

	if(!bw_exec(world->core, &world->p1_queue))
		return 0;

	return 1;
}

/* Peek into the core state */
CELL *bw_peek(struct bw_world *world, CELLPTR addr)
{
    return &world->core[MODC(addr)];
}

/* Modify the core state */
void bw_poke(struct bw_world *world, CELLPTR addr, CELL value)
{
    memcpy(&world->core[MODC(addr)], &value, sizeof(CELL));
}

/* Add a new process to the queue */
int bw_spawn(struct bw_world *world, CELLPTR addr, unsigned int player)
{
	QUEUE *queue;
	unsigned int tail_pos;

	switch(player) {
		case 0:
			queue = &world->p0_queue;
			break;
		case 1:
			queue = &world->p1_queue;
			break;
		default:
			return -1;
	}

	tail_pos = INCQ(queue->tail);
	if(tail_pos == queue->head) {
		/* Queue is full */
		return -1;
	}

    queue->slots[queue->tail] = addr;
	queue->tail = tail_pos;

	return 0;
}

/* Execute a single instruction for a single player */
static int bw_exec(CELL *core, QUEUE *queue)
{
	CELLPTR addr; /* Address of the instruction to execute */
    CELL *instr;  /* Pointers to the instruction to execute */
	unsigned int a_value, b_value; /* Operands to the instruction */
	unsigned int running = 1; /* Is the warrior still running? */

    if(EMPTYQ(queue))
        return 0;

    addr = queue->slots[queue->head];
    instr = core + addr;

    printf("*** bw_step -- op: 0x%x a_mode: %x b_mode: %x a: 0x%x b: 0x%x\n", instr->op, instr->a_mode, instr-> b_mode, instr->a_value, instr->b_value);

	/* FIXME: only direct addressing is supported */
    a_value = MODC(queue->slots[queue->head] + instr->a_value);
    b_value = MODC(queue->slots[queue->head] + instr->b_value);

    switch(instr->op) {
        case OP_DAT:
            /* Executing the DAT instruction terminates the current task */
            running = 0;
            break;
        case OP_MOV:
            memcpy(core + b_value, core + a_value, sizeof(CELL));
            break;
        case OP_ADD:
            core[b_value].a_value = MODC(core[a_value].a_value + core[b_value].a_value);
            core[b_value].b_value = MODC(core[a_value].b_value + core[b_value].b_value);
            break;
    }

	queue->head = INCQ(queue->head);

    /* If we should continue this task, add a new entry to the task queue at
       at the next instruction after this one */
    if(running) {
	    queue->slots[queue->tail] = INCC(addr);
		queue->tail = INCQ(queue->tail);
	}

	return EMPTYQ(queue);
}
