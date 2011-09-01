#include "ruby.h"
#include "badgewars.h"

static VALUE mBadgeWars = Qnil;
static VALUE cBadgeWarsOp = Qnil;
static VALUE cBadgeWarsWorld = Qnil;

static VALUE BadgeWars_core_size(VALUE self);
static VALUE BadgeWars_world_size(VALUE self);

static VALUE BadgeWarsOp_allocate(VALUE klass);
static void BadgeWarsOp_mark(CELL *cell);
static void BadgeWarsOp_free(CELL *cell);
static VALUE BadgeWarsOp_initialize(VALUE self, VALUE op, VALUE lhs, VALUE rhs);
static VALUE BadgeWarsOp_op(VALUE self);
static VALUE BadgeWarsOp_mode(VALUE self);
static VALUE BadgeWarsOp_lhs(VALUE self);
static VALUE BadgeWarsOp_rhs(VALUE self);
static VALUE BadgeWarsOp_raw(VALUE self);
static VALUE BadgeWarsOp_inspect(VALUE self);
static VALUE BadgeWarsOp_eql(VALUE self, VALUE op);

static VALUE BadgeWarsWorld_allocate(VALUE klass);
static void BadgeWarsWorld_mark(struct bw_world *world);
static void BadgeWarsWorld_free(struct bw_world *world);

static VALUE BadgeWarsWorld_peek(VALUE self, VALUE addr);
static VALUE BadgeWarsWorld_poke(VALUE self, VALUE addr, VALUE value);
static VALUE BadgeWarsWorld_step(VALUE self);
static VALUE BadgeWarsWorld_spawn(VALUE self, VALUE addr);

void Init_badgewars_ext()
{
    mBadgeWars = rb_define_module("BadgeWars");
    rb_define_singleton_method(mBadgeWars, "core_size", BadgeWars_core_size, 0);
    rb_define_singleton_method(mBadgeWars, "world_size", BadgeWars_world_size, 0);
    
    cBadgeWarsOp = rb_define_class_under(mBadgeWars, "Op", rb_cObject);
    rb_define_alloc_func(cBadgeWarsOp, BadgeWarsOp_allocate);
    rb_define_method(cBadgeWarsOp, "initialize", BadgeWarsOp_initialize, 3);
    rb_define_method(cBadgeWarsOp, "op", BadgeWarsOp_op, 0);
    rb_define_method(cBadgeWarsOp, "mode", BadgeWarsOp_mode, 0);
    rb_define_method(cBadgeWarsOp, "lhs", BadgeWarsOp_lhs, 0);
    rb_define_method(cBadgeWarsOp, "rhs", BadgeWarsOp_rhs, 0);
    rb_define_method(cBadgeWarsOp, "raw", BadgeWarsOp_raw, 0);
    rb_define_method(cBadgeWarsOp, "inspect", BadgeWarsOp_inspect, 0);
    rb_define_method(cBadgeWarsOp, "eql?", BadgeWarsOp_eql, 1);
    
    /* Opcodes */
    rb_define_const(cBadgeWarsOp, "DAT", INT2NUM(OP_DAT));
    rb_define_const(cBadgeWarsOp, "MOV", INT2NUM(OP_MOV));
    rb_define_const(cBadgeWarsOp, "ADD", INT2NUM(OP_ADD));
    rb_define_const(cBadgeWarsOp, "SUB", INT2NUM(OP_SUB));
    rb_define_const(cBadgeWarsOp, "MUL", INT2NUM(OP_MUL));
    rb_define_const(cBadgeWarsOp, "DIV", INT2NUM(OP_DIV));
    rb_define_const(cBadgeWarsOp, "MOD", INT2NUM(OP_MOD));
    rb_define_const(cBadgeWarsOp, "JMP", INT2NUM(OP_JMP));
    rb_define_const(cBadgeWarsOp, "JMZ", INT2NUM(OP_JMZ));
    rb_define_const(cBadgeWarsOp, "JMN", INT2NUM(OP_JMN));
    rb_define_const(cBadgeWarsOp, "DJN", INT2NUM(OP_DJN));
    rb_define_const(cBadgeWarsOp, "SPL", INT2NUM(OP_SPL));
    rb_define_const(cBadgeWarsOp, "SEQ", INT2NUM(OP_SEQ));
    rb_define_const(cBadgeWarsOp, "SNE", INT2NUM(OP_SNE));
    rb_define_const(cBadgeWarsOp, "SLT", INT2NUM(OP_SLT));
    rb_define_const(cBadgeWarsOp, "NOP", INT2NUM(OP_NOP));
    
    cBadgeWarsWorld = rb_define_class_under(mBadgeWars, "World", rb_cObject);
    rb_define_alloc_func(cBadgeWarsWorld, BadgeWarsWorld_allocate);
    rb_define_method(cBadgeWarsWorld, "peek", BadgeWarsWorld_peek, 1);
    rb_define_method(cBadgeWarsWorld, "poke", BadgeWarsWorld_poke, 2);
    rb_define_method(cBadgeWarsWorld, "[]", BadgeWarsWorld_peek, 1);
    rb_define_method(cBadgeWarsWorld, "[]=", BadgeWarsWorld_poke, 2);
    rb_define_method(cBadgeWarsWorld, "step", BadgeWarsWorld_step, 0);
    rb_define_method(cBadgeWarsWorld, "spawn", BadgeWarsWorld_spawn, 1);
}

static VALUE BadgeWars_core_size(VALUE self)
{
    return INT2NUM(BW_CORE_SIZE);
}

static VALUE BadgeWars_world_size(VALUE self)
{
    return INT2NUM(sizeof(struct bw_world));
}

static VALUE BadgeWarsOp_allocate(VALUE klass)
{
    CELL *cell = (CELL *)xmalloc(sizeof(struct bw_opcode));
    cell->op = cell->mode = cell->lhs = cell->rhs = 0;
    
    return Data_Wrap_Struct(klass, BadgeWarsOp_mark, BadgeWarsOp_free, cell);
}

static void BadgeWarsOp_mark(CELL *cell)
{
    /* This function intentionally left blank */
}

static void BadgeWarsOp_free(CELL *cell)
{
    xfree(cell);
}

static VALUE BadgeWarsOp_initialize(VALUE self, VALUE op, VALUE lhs, VALUE rhs)
{
    CELL *opcode;
    Data_Get_Struct(self, struct bw_opcode, opcode);
    
    opcode->op = NUM2INT(op) & 0x3F;
    opcode->mode = 0;
    opcode->lhs = NUM2INT(lhs) & 0x7FF;
    opcode->rhs = NUM2INT(rhs) & 0x7FF;
    
    return Qnil;
}

static VALUE BadgeWarsOp_op(VALUE self)
{
    CELL *opcode;
    Data_Get_Struct(self, struct bw_opcode, opcode);
    return INT2NUM(opcode->op);
}

static VALUE BadgeWarsOp_mode(VALUE self)
{
    CELL *opcode;
    Data_Get_Struct(self, struct bw_opcode, opcode);
    return INT2NUM(opcode->mode);
}

static VALUE BadgeWarsOp_lhs(VALUE self)
{
    CELL *opcode;
    Data_Get_Struct(self, struct bw_opcode, opcode);
    return INT2NUM(opcode->lhs);
}

static VALUE BadgeWarsOp_rhs(VALUE self)
{
    CELL *opcode;
    Data_Get_Struct(self, struct bw_opcode, opcode);
    return INT2NUM(opcode->rhs);
}

static VALUE BadgeWarsOp_raw(VALUE self)
{
    CELL *opcode;
    Data_Get_Struct(self, struct bw_opcode, opcode);
    
    return rb_str_new((char *)opcode, 4);
}

static VALUE BadgeWarsOp_inspect(VALUE self)
{
    VALUE str;
    CELL *opcode;
    
    const char *cname = rb_class2name(rb_obj_class(self));
    Data_Get_Struct(self, struct bw_opcode, opcode);
    
    str = rb_str_new2("#<");
    rb_str_cat2(str, cname);
    rb_str_cat2(str, " ");
    
    rb_str_cat2(str, "op=0x");
    rb_str_append(str, rb_funcall(INT2NUM(opcode->op), rb_intern("to_s"), 1, INT2NUM(16)));
    rb_str_cat2(str, " ");
    
    rb_str_cat2(str, "mode=");
    rb_str_append(str, rb_inspect(INT2NUM(opcode->mode)));
    rb_str_cat2(str, " ");
    
    rb_str_cat2(str, "lhs=0x");
    rb_str_append(str, rb_funcall(INT2NUM(opcode->lhs), rb_intern("to_s"), 1, INT2NUM(16)));
    rb_str_cat2(str, " ");

    rb_str_cat2(str, "rhs=0x");
    rb_str_append(str, rb_funcall(INT2NUM(opcode->rhs), rb_intern("to_s"), 1, INT2NUM(16)));
    rb_str_cat2(str, ">");
    
    return str;       
}

static VALUE BadgeWarsOp_eql(VALUE self, VALUE op)
{
    CELL *left, *right;
    Data_Get_Struct(self, struct bw_opcode, left);
    Data_Get_Struct(op,   struct bw_opcode, right);
    
    if(left->op   == right->op &&
       left->mode == right->mode &&
       left->lhs  == right->lhs &&
       left->rhs  == right->rhs)
    {
        return Qtrue;
    } else {
        return Qfalse;
    }
}

static VALUE BadgeWarsWorld_allocate(VALUE klass)
{
    struct bw_world *world = (struct bw_world *)xmalloc(sizeof(struct bw_world));
    bw_init(world);
    
    return Data_Wrap_Struct(klass, BadgeWarsWorld_mark, BadgeWarsWorld_free, world);
}

static void BadgeWarsWorld_mark(struct bw_world *world)
{
    /* This function intentionally left blank */
}

static void BadgeWarsWorld_free(struct bw_world *world)
{
    xfree(world);
}

static VALUE BadgeWarsWorld_peek(VALUE self, VALUE addr)
{
    CELL *opcode;
    struct bw_world *world;
    Data_Get_Struct(self, struct bw_world, world);
    
    opcode = bw_peek(world, NUM2INT(addr));
    return rb_funcall(cBadgeWarsOp, rb_intern("new"), 3, INT2NUM(opcode->op), INT2NUM(opcode->lhs), INT2NUM(opcode->rhs));
}

static VALUE BadgeWarsWorld_poke(VALUE self, VALUE addr, VALUE opcode)
{
    CELL *cell;
    struct bw_world *world;
    Data_Get_Struct(self, struct bw_world, world);
    Data_Get_Struct(opcode, struct bw_opcode, cell);
    bw_poke(world, NUM2INT(addr), *cell);
    
    return Qnil;
}

static VALUE BadgeWarsWorld_step(VALUE self)
{
    struct bw_world *world;
    Data_Get_Struct(self, struct bw_world, world);
    return INT2NUM(bw_step(world));
}

static VALUE BadgeWarsWorld_spawn(VALUE self, VALUE addr)
{
    struct bw_world *world;
    Data_Get_Struct(self, struct bw_world, world);
    bw_spawn(world, NUM2INT(addr));
    
    return Qnil;
}
