# MALLOC

* allcoates memorywe 
* have a bunch of memory WE know about but no one else does
* We store it (for some reason) on a HEAP
* reserving space for who called us

# FREE

* deallocating memory
* user gives us memory back
* add that into what we know about (and user is supposed to forget about now)

# HEAP

* A bunch of memory
`struct chunk on_heap { int size; char* position_to_start };`


* Add a chunk back (on free)
`create the chunk_on_heap; heap_insert()`

* get a chunk out (it's not ours any more, it's the users) on malloc
`
try to find the size or bigger;
if found a chunk: cut it into two(acutal size, and leftover)
    return the actual size
    heap_insert(leftover)
if no find:
    get more memory from system (sbrk)
`

* MINheap based on size of chunk -- sorted from smallest to largest
* small chunks are more frequently requested

* what do we do when the heap is empty?
    sbrk()
* what do we do when the user requests a larger thing than is in the heap?
    sbrk()

# sbrk() (Big pieces of memory)

* Ask linus for a HUGE chunk of memory