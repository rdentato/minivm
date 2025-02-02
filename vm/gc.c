#include "gc.h"
#include "obj.h"

void vm_gc_mark_ptr(vm_gc_entry_t *ent)
{
    if (ent->keep)
    {
        return;
    }
    ent->keep = true;
    vm_gc_entry_t *arr_ent = (vm_gc_entry_t *)ent;
    for (size_t cur = 0; cur < arr_ent->len; cur++)
    {
        vm_obj_t obj = ((vm_obj_t *)arr_ent->obj)[cur];
        if (vm_obj_is_ptr(obj))
        {
            vm_gc_mark_ptr(vm_obj_to_ptr(obj));
        }
    }
}

void vm_gc_run1(vm_gc_t *gc)
{
    if (gc->low == NULL)
    {
        return;
    }
    for (vm_obj_t *base = gc->low; base < gc->high; base++)
    {
        vm_obj_t cur = *base;
        if (vm_obj_is_ptr(cur))
        {
            vm_gc_entry_t *ptr = vm_obj_to_ptr(cur);
            vm_gc_mark_ptr(ptr);
        }
    }
    size_t begin = 0;
    vm_gc_entry_t *first = gc->first;
    vm_gc_entry_t *last = NULL;
    size_t n = 0;
    while (first != NULL)
    {
        vm_gc_entry_t *ent = first;
        first = first->next;
        if (ent->keep)
        {
            ent->keep = false;
            ent->next = last;
            last = ent;
            n++;
        }
        else
        {
            vm_free(ent);
        }
    }
    gc->remain = (gc->high - gc->low) + n * 2;
    gc->first = last;
}

void vm_gc_start(vm_gc_t *gc)
{
    gc->remain = 1000;
    gc->first = NULL;
    gc->low = NULL;
    gc->high = NULL;
}

void vm_gc_stop(vm_gc_t *gc)
{
    vm_gc_entry_t *first = gc->first;
    while (first != NULL)
    {
        vm_gc_entry_t *next = first->next;
        vm_free(first);
        first = next;
    }
}

vm_gc_entry_t *vm_gc_array_new(vm_gc_t *gc, size_t size)
{
    gc->remain--;
    if (gc->remain == 0) {
        vm_gc_run1(gc);
    }
    vm_gc_entry_t *entry = vm_malloc(sizeof(vm_gc_entry_t));
    *entry = (vm_gc_entry_t){
        .next = gc->first,
        .keep = false,
        .alloc = size,
        .len = size,
        .obj = vm_malloc(sizeof(vm_obj_t) * size),
    };
    vm_gc_entry_t *obj = (vm_gc_entry_t *)entry;
    gc->first = (vm_gc_entry_t *) entry;
    return obj;
}

vm_obj_t vm_gc_get_index(vm_gc_entry_t *ptr, int index)
{
    if (index < 0) {
        index += ptr->len;
    }
    if (index >= ptr->len) {
        __builtin_trap();
    }
    return ptr->obj[index];
}

void vm_gc_set_index(vm_gc_entry_t *ptr, int index, vm_obj_t value)
{
    if (index < 0) {
        index += ptr->len;
    }
    if (index >= ptr->len) {
        __builtin_trap();
    }
    ptr->obj[index] = value;
}

int vm_gc_sizeof(vm_gc_entry_t *ptr)
{
    return ((vm_gc_entry_t *)ptr)->len;
}

void vm_gc_extend(vm_gc_entry_t *ato, vm_gc_entry_t *afrom)
{
    if (ato->len + afrom->len >= ato->alloc)
    {
        int alloc = (ato->len + afrom->len) * 2;
        ato->obj = vm_realloc(ato->obj, sizeof(vm_obj_t) * alloc);
        ato->alloc = alloc;
    }
    for (size_t i = 0; i < afrom->len; i++)
    {
        ato->obj[ato->len++] = afrom->obj[i];
    }
    return;
}

void vm_gc_push(vm_gc_entry_t *ato, vm_obj_t from)
{
    if (ato->len + 2 >= ato->alloc)
    {
        int alloc = (ato->len + 1) * 2;
        ato->obj = vm_realloc(ato->obj, sizeof(vm_obj_t) * alloc);
        ato->alloc = alloc;
    }
    ato->obj[ato->len++] = from;
}

vm_obj_t vm_gc_concat(vm_gc_t *gc, vm_obj_t lhs, vm_obj_t rhs)
{
    vm_gc_entry_t *left = vm_obj_to_ptr(lhs);
    vm_gc_entry_t *right = vm_obj_to_ptr(rhs);
    int llen = left->len;
    int rlen = right->len;
    vm_gc_entry_t *ent = (vm_gc_entry_t *)vm_gc_array_new(gc, llen + rlen);
    for (int i = 0; i < llen; i++)
    {
        ((vm_obj_t *)ent->obj)[i] = left->obj[i];
    }
    for (int i = 0; i < rlen; i++)
    {
        ((vm_obj_t *)ent->obj)[llen + i] = right->obj[i];
    }
    return vm_obj_of_ptr(ent);
}
