#include <sys/heap.h>
#include <sys/libk.h>

struct blck
{
    struct blck *next, *prev;
    size_t size;
    int free;
};

static struct blck *s_head = NULL;

// Expand the heap, allocate pages
static size_t expand(size_t n)
{
    return n;
}

// Split a block into two
static struct blck *split(struct blck *b, size_t n)
{
    struct blck* new = (struct blck*)((uintptr_t)b + sizeof(struct blck) + n);
   
    // Set the data
    new->free = 1;
    new->size = b->size - n - sizeof(struct blck);
    
    b->size = n;
    
    // Adjust the pointers
    new->next = b->next;
    b->next = new;
    new->prev = b;
    
    // Try to adjust the 'next' field
    if (new->next)
        new->next->prev = new;

    return b;
}

// Combine two blocks (prevents fragmentation)
static struct blck *combine(struct blck *b1, struct blck *b2)
{
    // Add the size and adjust 'next' to jump over b2
    b1->size += b2->size + sizeof(struct blck);
    b1->next = b2->next;

    // Try to adjust b1's next prev
    if (b1->next)
        b1->next->prev = b1;

    return b1;
}

// Allocate kernel heap memory
void *malloc(size_t size)
{
    if (!size) return NULL;

    size |= 31; // 32-byte alignment

    for (struct blck *b = s_head; b; b = b->next)
    {
        if (!b->free) continue;

        if (b->size == size)
        {
            b->free = 0;
            return b + 1; // Memory following the header
        }
        else if (b->size > size)
        {
            b = split(b, size);
            b->free = 0;
            return b + 1;
        }
    }

    return malloc(expand(size));
}

// Allocate memory and zero it
void *calloc(size_t nitems, size_t size)
{
    void *ptr = malloc(nitems * size);
    if (!ptr) return ptr;

    memset(ptr, 0, nitems * size);
    return ptr;
}

// Free kernel memory
void free(void *ptr)
{
    if (!ptr) return; // If NULL, do nothing.
    
    struct blck *b = (struct blck*)ptr - 1;
    b->free = 1;

    if (b->prev && b->prev->free) b = combine(b->prev, b);
    if (b->next && b->next->free) b = combine(b, b->next);
}
