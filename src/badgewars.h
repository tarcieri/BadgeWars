/* 
 * BADGEWARS: definition of the core structure 
 */
 
/* Total number of cells in the core */
#define BW_CORE_SIZE  8192
#define BW_QUEUE_SIZE 1024

/* Opcode definition (opcodes fit inside of CELLs) */
typedef struct bw_opcode {
    unsigned int op:   4; /* Operation to perform */  
    unsigned int mode: 4; /* Addressing mode */
    unsigned int lhs:  8; /* Left-hand operand */
    unsigned int rhs:  8; /* Right-hand operand */
} CELL;

/* Size of a "pointer" to a location in the core (should be a 16-bit type) */
typedef unsigned short CELLPTR;

/* Structure of the BadgeWars world */
struct bw_world {
    CELL core[BW_CORE_SIZE];
    CELLPTR queue[BW_QUEUE_SIZE];
    int queue_head, queue_tail;
};

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

/* Add a new process to the queue */
void bw_spawn(struct bw_world *world, CELLPTR addr);