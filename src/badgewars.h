/* 
 * BADGEWARS: definition of the core structure 
 */
 
/* Total number of cells in the core */
#define BW_CORE_SIZE  8192
#define BW_QUEUE_SIZE 4096

/* Size of a single cell in the core */
typedef long CELL;

/* Size of a "pointer" to a location in the core */
typedef short CELLPTR;

/* Opaque identifier representing a return badge address */
typedef short BW_ADDR;

/* Structure of the BadgeWars world */
struct bw_world {
  CELL core[BW_CORE_SIZE];
  CELLPTR queue[BW_QUEUE_SIZE];
  CELLPTR *queue_head, *queue_tail;
};

enum OP {
  OP_NOP, /* no operation: don't do anything */
  OP_DAT, /* data: kills the process */
  OP_MOV, /* move: copies data from one address to another */
  OP_ADD, /* add: adds one number to another */
  OP_SUB, /* subtract: subtracts one number from another */
  OP_MUL, /* multiply: multiplies one number with another */
  OP_DIV, /* divide: divides one number with another */
  OP_MOD, /* modulus: divides one number with another, storing the remainder */
  OP_JMP, /* jump: contines execution at the given address */
  OP_JMZ, /* jump if zero: tests a number and jumps if it's 0 */
  OP_JMN, /* jump if nonzero: tests a number and jumps if it isn't 0 */
  OP_DJN, /* decrement and jump if nonzero: like above, but subtract 1 first */
  OP_SPL, /* split: start a new process at the given address */
  OP_CMP, /* compare: same as SEQ. See below */
  OP_SEQ, /* skip if equal: if two instructions are equal, skip the next */
  OP_SNE, /* skip if not equal: like above, but with the opposite semantics */
  OP_SLT, /* skip if lower than: like above, but compare two values */
  OP_OPN, /* open: create a connection to another badge */
  OP_RSW, /* remote store word: store a word on a remote badge */
  OP_RFW, /* remote fetch word: store a remote word at the given location */ 
  OP_RSP, /* remote split: like SPL, but start a process on a remote badge */
  OP_CLS  /* close: close the connection to a remote badge */
};

/* Opcode definition (opcodes fit inside of CELLs) */
typedef struct opcode {
  unsigned int op:   6; /* Operation to perform */  
  unsigned int mode: 4; /* Addressing mode */
  unsigned int lhs: 11; /* Left-hand operand */
  unsigned int rhs: 11; /* Right-hand operand */
} OPCODE;

/* Initialize the BadgeWars world */
void bw_init(struct bw_world *world);
 
/* Run the BadgeWars world for a single instruction */
void bw_run(struct bw_world *world);

/* Receive a BadgeWars command from the outside world */
/*void bw_receive(struct bm_world *world, OPCODE command, BW_ADDR addr);*/