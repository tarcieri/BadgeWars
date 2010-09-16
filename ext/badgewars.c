#include "ruby.h"
#include "../src/badgewars.h"

static VALUE mBadgeWars = Qnil;
static VALUE cBadgeWarsWorld = Qnil;

static VALUE BadgeWarsWorld_allocate(VALUE klass);
static void BadgeWarsWorld_mark(struct bw_world *world);
static void BadgeWarsWorld_free(struct bw_world *world);

void Init_badgewars()
{
    mBadgeWars = rb_define_module("BadgeWars");
    cBadgeWarsWorld = rb_define_class_under(mBadgeWars, "World", rb_cObject);
    
    rb_define_alloc_func(cBadgeWarsWorld, BadgeWarsWorld_allocate);
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
