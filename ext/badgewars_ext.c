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
static VALUE BadgeWarsOp_initialize(VALUE self, VALUE op, VALUE a_value, VALUE b_value);
static VALUE BadgeWarsOp_op(VALUE self);
static VALUE BadgeWarsOp_a_mode(VALUE self);
static VALUE BadgeWarsOp_b_mode(VALUE self);
static VALUE BadgeWarsOp_a_value(VALUE self);
static VALUE BadgeWarsOp_b_value(VALUE self);
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

static const char *op_names[] = {
    "DAT", "MOV", "ADD", "SUB", "MUL", "DIV", "MOD", "JMP",
    "JMZ", "JMN", "DJN", "SPL", "SEQ", "SNE", "SLT", "NOP"
};

static const char *mode_sigils[] = {"", "#", "@", "<"};

void Init_badgewars_ext()
{
    mBadgeWars = rb_define_module("BadgeWars");
    rb_define_singleton_method(mBadgeWars, "core_size", BadgeWars_core_size, 0);
    rb_define_singleton_method(mBadgeWars, "world_size", BadgeWars_world_size, 0);

    cBadgeWarsOp = rb_define_class_under(mBadgeWars, "Op", rb_cObject);
    rb_define_alloc_func(cBadgeWarsOp, BadgeWarsOp_allocate);
    rb_define_method(cBadgeWarsOp, "initialize", BadgeWarsOp_initialize, 3);
    rb_define_method(cBadgeWarsOp, "op", BadgeWarsOp_op, 0);
    rb_define_method(cBadgeWarsOp, "a_mode", BadgeWarsOp_a_mode, 0);
    rb_define_method(cBadgeWarsOp, "b_mode", BadgeWarsOp_b_mode, 0);
    rb_define_method(cBadgeWarsOp, "a_value", BadgeWarsOp_a_value, 0);
    rb_define_method(cBadgeWarsOp, "b_value", BadgeWarsOp_b_value, 0);
    rb_define_method(cBadgeWarsOp, "raw", BadgeWarsOp_raw, 0);
    rb_define_method(cBadgeWarsOp, "inspect", BadgeWarsOp_inspect, 0);
    rb_define_method(cBadgeWarsOp, "eql?", BadgeWarsOp_eql, 1);

    /* instrs */
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
    CELL *cell = (CELL *)xmalloc(sizeof(struct bw_instr));
    cell->op = cell->a_mode = cell->b_mode = cell->a_value = cell->b_value = 0;

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

static VALUE BadgeWarsOp_initialize(VALUE self, VALUE op, VALUE a_value, VALUE b_value)
{
    CELL *instr;
    Data_Get_Struct(self, struct bw_instr, instr);

    instr->op = NUM2INT(op) & ((1 << BW_OP_SIZE) - 1);
    instr->a_mode = instr->b_mode = 0;
    instr->a_value = NUM2INT(a_value) & ((1 << BW_OPERAND_SIZE) - 1);
    instr->b_value = NUM2INT(b_value) & ((1 << BW_OPERAND_SIZE) - 1);

    return Qnil;
}

static VALUE BadgeWarsOp_op(VALUE self)
{
    CELL *instr;
    Data_Get_Struct(self, struct bw_instr, instr);
    return INT2NUM(instr->op);
}

static VALUE BadgeWarsOp_a_mode(VALUE self)
{
    CELL *instr;
    Data_Get_Struct(self, struct bw_instr, instr);
    return INT2NUM(instr->a_mode);
}

static VALUE BadgeWarsOp_b_mode(VALUE self)
{
    CELL *instr;
    Data_Get_Struct(self, struct bw_instr, instr);
    return INT2NUM(instr->b_mode);
}


static VALUE BadgeWarsOp_a_value(VALUE self)
{
    CELL *instr;
    Data_Get_Struct(self, struct bw_instr, instr);
    return INT2NUM(instr->a_value);
}

static VALUE BadgeWarsOp_b_value(VALUE self)
{
    CELL *instr;
    Data_Get_Struct(self, struct bw_instr, instr);
    return INT2NUM(instr->b_value);
}

static VALUE BadgeWarsOp_raw(VALUE self)
{
    CELL *instr;
    Data_Get_Struct(self, struct bw_instr, instr);

    return rb_str_new((char *)instr, sizeof(struct bw_instr));
}

static VALUE BadgeWarsOp_inspect(VALUE self)
{
    VALUE str;
    CELL *instr;
    const char *mode;

    const char *cname = rb_class2name(rb_obj_class(self));
    Data_Get_Struct(self, struct bw_instr, instr);

    str = rb_str_new2("#<");
    rb_str_cat2(str, cname);

    rb_str_cat2(str, "[");
    rb_str_cat2(str, op_names[instr->op]);
    rb_str_cat2(str, " ");

    rb_str_cat2(str, mode_sigils[instr->a_mode]);
    rb_str_append(str, rb_funcall(INT2NUM(instr->a_value), rb_intern("to_s"), 1, INT2NUM(16)));
    rb_str_cat2(str, " ");

    rb_str_cat2(str, mode_sigils[instr->b_mode]);
    rb_str_append(str, rb_funcall(INT2NUM(instr->b_value), rb_intern("to_s"), 1, INT2NUM(16)));
    rb_str_cat2(str, "]>");

    return str;
}

static VALUE BadgeWarsOp_eql(VALUE self, VALUE op)
{
    CELL *left, *right;
    Data_Get_Struct(self, struct bw_instr, left);
    Data_Get_Struct(op,   struct bw_instr, right);

    if(left->op       == right->op &&
       left->a_mode   == right->a_mode &&
       left->b_mode   == right->b_mode &&
       left->a_value  == right->a_value &&
       left->b_value  == right->b_value)
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
    CELL *instr;
    struct bw_world *world;
    Data_Get_Struct(self, struct bw_world, world);

    instr = bw_peek(world, NUM2INT(addr));
    return rb_funcall(cBadgeWarsOp, rb_intern("new"), 3, INT2NUM(instr->op), INT2NUM(instr->a_value), INT2NUM(instr->b_value));
}

static VALUE BadgeWarsWorld_poke(VALUE self, VALUE addr, VALUE instr)
{
    CELL *cell;
    struct bw_world *world;
    Data_Get_Struct(self, struct bw_world, world);
    Data_Get_Struct(instr, struct bw_instr, cell);
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
    bw_spawn(world, NUM2INT(addr), 0);

    return Qnil;
}
