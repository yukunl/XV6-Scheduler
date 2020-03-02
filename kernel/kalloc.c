// Physical memory allocator, intended to allocate
// memory for user processes, kernel stacks, page table pages,
// and pipe buffers. Allocates 4096-byte pages.

#include "types.h"
#include "defs.h"
#include "param.h"
#include "mmu.h"
#include "spinlock.h"
#include "rand.h"

int allocatedframes[512];
int count = 0; // size of allocated
int freeListSize = 0; // size of free list

struct run {
  struct run *next;
};

struct {
  struct spinlock lock;
  struct run *freelist;
} kmem;

extern char end[]; // first address after kernel loaded from ELF file

// Initialize free list of physical pages.
void
kinit(void)
{
  char *p;

  initlock(&kmem.lock, "kmem");
  p = (char*)PGROUNDUP((uint)end);
  //All the pages from end to PHYSTOP are marked as free pages 
  //at the beginning
  for(; p + PGSIZE <= (char*)PHYSTOP; p += PGSIZE )
    kfree(p);
}

// Free the page of physical memory pointed at by v,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void
kfree(char *v)
{
  struct run *r;
  // address validity checks
  if((uint)v % PGSIZE || v < end || (uint)v >= PHYSTOP) 
    panic("kfree");

  // Fill with junk to catch dangling refs.
  // fill page with 1, to help in case of bugs
  memset(v, 1, PGSIZE);
  // insert our page into the beginning of kmem(a linkedlist with available pages)
  acquire(&kmem.lock);
  r = (struct run*)v;
  r->next = kmem.freelist;
  kmem.freelist = r;
  freeListSize ++; // increment free list size
  release(&kmem.lock);
  }//allocated the kernel can use.
// Returns 0 if the memory cannot be allocated.
char*
kalloc(void)
{
  struct run *r;
  
  acquire(&kmem.lock);
  r = kmem.freelist;
  if(r){
    //xv6_srand (1);
    int randomNum = xv6_rand();
    int location = randomNum / freeListSize;
    allocatedframes[location] = (uint)r;
    count ++;
    kmem.freelist = r->next;
  }
  release(&kmem.lock);
  return (char*)r;
}

int dump_allocated(int *frames, int numframes){
if(frames < 0 || numframes < 0 || numframes > count){
  return -1;
}
int counter = count -1 ;

for(int i = 0; i < numframes; i++){
  frames[i] = allocatedframes[counter];
  counter --;
}
  return 0;
}
