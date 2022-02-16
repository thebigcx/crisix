#include <sys/heap.h>
#include <sys/libk.h>

#include <arch/arch.h>

struct blck
{
    struct blck *next, *prev;
    size_t size;
    int free;
};

static struct blck *s_head = NULL;
static size_t s_size = 0;

// Split a block into two
static struct blck *split(struct blck *b, size_t n)
{
    struct blck* new = (struct blck*)((uintptr_t)(b + 1) + n);
   
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

// Expand the heap, allocate pages
static void expand(size_t n)
{
    // Last block, if no blocks yet then equal to NULL
    struct blck *last;
    for (last = s_head; last && last->next; last = last->next);

    // Initialize heap if necessary
    if (!s_head)
        s_head = (struct blck*)0xffffffffd0000000;

    if (n % PAGE_SIZE)
        n += PAGE_SIZE - (n % PAGE_SIZE);

    for (size_t i = 0; i < n; i += PAGE_SIZE)
        vkmmap((void*)((uintptr_t)s_head + s_size + i), pmalloc(1), 1, MMU_PR | MMU_RW);

    // Create and initialize new block
    struct blck *new = (struct blck*)((uintptr_t)s_head + s_size);

    new->prev = last;
    new->next = NULL;
    new->size = n;
    new->free = 1;

    // Combine backward if possible
    if (new->prev)
    {
        new->prev->next = new;
        if (new->prev->free) new = combine(new->prev, new);
    }

    s_size += n;
}

// Allocate kernel heap memory
void *malloc(size_t size)
{
    if (!size) return NULL;

    // 32-byte alignment
    if (size % 32)
        size += 32 - (size % 32);

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

    expand(size);
    return malloc(size);
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
