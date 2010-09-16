#include "ruby.h"
#include "../src/badgewars.h"

static VALUE mBadgeWars = Qnil;
static VALUE cBadgeWarsWorld = Qnil;

static VALUE BadgeWars_core_size(VALUE self);
static VALUE BadgeWars_world_size(VALUE self);

static VALUE BadgeWarsWorld_allocate(VALUE klass);
static void BadgeWarsWorld_mark(struct bw_world *world);
static void BadgeWarsWorld_free(struct bw_world *world);

static VALUE BadgeWarsWorld_peek(VALUE self, VALUE addr);
static VALUE BadgeWarsWorld_poke(VALUE self, VALUE addr, VALUE value);

void Init_badgewars()
{
    mBadgeWars = rb_define_module("BadgeWars");
    rb_define_singleton_method(mBadgeWars, "core_size", BadgeWars_core_size, 0);
    rb_define_singleton_method(mBadgeWars, "world_size", BadgeWars_world_size, 0);
    
    cBadgeWarsWorld = rb_define_class_under(mBadgeWars, "World", rb_cObject);
    rb_define_alloc_func(cBadgeWarsWorld, BadgeWarsWorld_allocate);
    rb_define_method(cBadgeWarsWorld, "peek", BadgeWarsWorld_peek, 1);
    rb_define_method(cBadgeWarsWorld, "[]", BadgeWarsWorld_peek, 1);
    
    rb_define_method(cBadgeWarsWorld, "poke", BadgeWarsWorld_poke, 2);
    rb_define_method(cBadgeWarsWorld, "[]=", BadgeWarsWorld_poke, 2);
    
    /* Opcodes */
    rb_define_const(mBadgeWars, "OP_DAT", INT2NUM(OP_DAT));
    rb_define_const(mBadgeWars, "OP_MOV", INT2NUM(OP_MOV));
    rb_define_const(mBadgeWars, "OP_ADD", INT2NUM(OP_ADD));
    rb_define_const(mBadgeWars, "OP_SUB", INT2NUM(OP_SUB));
    rb_define_const(mBadgeWars, "OP_MUL", INT2NUM(OP_MUL));
    rb_define_const(mBadgeWars, "OP_DIV", INT2NUM(OP_DIV));
    rb_define_const(mBadgeWars, "OP_MOD", INT2NUM(OP_MOD));
    rb_define_const(mBadgeWars, "OP_JMP", INT2NUM(OP_JMP));
    rb_define_const(mBadgeWars, "OP_JMZ", INT2NUM(OP_JMZ));
    rb_define_const(mBadgeWars, "OP_JMN", INT2NUM(OP_JMN));
    rb_define_const(mBadgeWars, "OP_DJN", INT2NUM(OP_DJN));
    rb_define_const(mBadgeWars, "OP_SPL", INT2NUM(OP_SPL));
    rb_define_const(mBadgeWars, "OP_CMP", INT2NUM(OP_CMP));
    rb_define_const(mBadgeWars, "OP_SEQ", INT2NUM(OP_SEQ));
    rb_define_const(mBadgeWars, "OP_SNE", INT2NUM(OP_SNE));
    rb_define_const(mBadgeWars, "OP_SLT", INT2NUM(OP_SLT));
    rb_define_const(mBadgeWars, "OP_NOP", INT2NUM(OP_NOP));
    rb_define_const(mBadgeWars, "OP_OPN", INT2NUM(OP_OPN));
    rb_define_const(mBadgeWars, "OP_RSW", INT2NUM(OP_RSW));
    rb_define_const(mBadgeWars, "OP_RFW", INT2NUM(OP_RFW));
    rb_define_const(mBadgeWars, "OP_RSP", INT2NUM(OP_RSP));
    rb_define_const(mBadgeWars, "OP_CLS", INT2NUM(OP_CLS));
}

static VALUE BadgeWars_core_size(VALUE self)
{
    return INT2NUM(BW_CORE_SIZE);
}

static VALUE BadgeWars_world_size(VALUE self)
{
    return INT2NUM(sizeof(struct bw_world));
}

static VALUE BadgeWarsWorld_allocate(VALUE klass)
{
    struct bw_world *world = (struct bw_world *)xmalloc(sizeof(struct bw_world));
    bw_init(world);
    
    return Data_Wrap_Struct(klass, BadgeWarsWorld_mark, BadgeWarsWorld_free, world);
}

static void BadgeWarsWorld_mark(struct bw_world *world)
{
}

static void BadgeWarsWorld_free(struct bw_world *world)
{
    xfree(world);
}

static VALUE BadgeWarsWorld_peek(VALUE self, VALUE addr)
{
    struct bw_world *world;
    
    Data_Get_Struct(self, struct bw_world, world);
    return INT2NUM(bw_peek(world, NUM2INT(addr)));
}

static VALUE BadgeWarsWorld_poke(VALUE self, VALUE addr, VALUE value)
{
    struct bw_world *world;
    
    Data_Get_Struct(self, struct bw_world, world);
    bw_poke(world, NUM2INT(addr), NUM2INT(value));
    
    return Qnil;
}