/*
 * BADGEWARS: definition of the core structure
 */

/* Total number of cells in the core */
#define BW_CORE_SIZE  8192

/* Total number of entries in a queue */
#define BW_QUEUE_SIZE 512

/* Size of an op in bits */
#define BW_OP_SIZE 4

/* Size of an operand in bits */
#define BW_OPERAND_SIZE 8

/* Instruction definition (each instruction is a CELL) */
typedef struct bw_instr {
    unsigned int op:     BW_OP_SIZE;      /* Operation to perform */
    unsigned int a_mode: 2;               /* Addressing mode for A operand */
    unsigned int b_mode: 2;               /* Addressing mode for B operand */
    int a_value:         BW_OPERAND_SIZE; /* Left-hand operand */
    int b_value:         BW_OPERAND_SIZE; /* Right-hand operand */
} CELL;

/* Size of a "pointer" to a location in the core (should be a 16-bit type) */
typedef unsigned short CELLPTR;

/* A queue of addresses to warriors in the core. Each player has their own */
typedef struct bw_queue {
    CELLPTR slots[BW_QUEUE_SIZE];
    unsigned int head, tail;
} QUEUE;

/* Structure of the BadgeWars world */
typedef struct bw_world {
    CELL core[BW_CORE_SIZE];
    QUEUE p0_queue, p1_queue;
} WORLD;

enum MODE {
    MODE_DIRECT,    /* Operand points to offset from it's own address */
    MODE_IMMEDIATE, /* Operand is the data for its own instruction    (#) */
    MODE_INDIRECT,  /* Operand points to a cell that points elsewhere (@) */
    MODE_PREDINDR   /* Same as above, except decrement the B operand  (<) */
};

enum OP {
    OP_DAT, /* data: kills the process */
    OP_MOV, /* move: copies data from one address to another */
    OP_ADD, /* add: adds one number to another */
    OP_SUB, /* subtract: subtracts one number from another */
    OP_MUL, /* multiply: multiplies one number with another */
    OP_DIV, /* divide: divides one number with another */
    OP_MOD, /* modulus: divides one number with another. stores the remainder */
    OP_JMP, /* jump: contines execution at the given address */
    OP_JMZ, /* jump if zero: tests a number and jumps if it's 0 */
    OP_JMN, /* jump if nonzero: tests a number and jumps if it isn't 0 */
    OP_DJN, /* decrement and jump if nonzero: like above but subtract 1 first */
    OP_SPL, /* split: start a new process at the given address */
    OP_SEQ, /* skip if equal: if two instructions are equal, skip the next */
    OP_SNE, /* skip if not equal: like above, but with the opposite semantics */
    OP_SLT, /* skip if lower than: like above, but compare two values */
    OP_NOP  /* no operation: don't do anything */
};

/* CMP is an alias for SEQ */
#define OP_CMP OP_SEQ

/* Initialize the BadgeWars world */
void bw_init(struct bw_world *world);

/* Tell the BadgeWars world to execute a single instruction */
int bw_step(struct bw_world *world);

/* Peek into the core state */
CELL *bw_peek(struct bw_world *world, CELLPTR addr);

/* Modify the core state */
void bw_poke(struct bw_world *world, CELLPTR addr, CELL value);

/* Add a new process to the given player's queue */
int bw_spawn(struct bw_world *world, CELLPTR addr, unsigned int player);
