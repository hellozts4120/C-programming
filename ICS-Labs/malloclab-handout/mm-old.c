/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 * 
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
    /* Team name */
    "ateam",
    /* First member's full name */
    "Tianshu Zhou",
    /* First member's email address */
    "14307110190@fudan.edu.cn",
    /* Second member's full name (leave blank if none) */
    "",
    /* Second member's email address (leave blank if none) */
    ""
};

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)


#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

/* Other constants and macros needed */
#define WSIZE 4     /* Word and header/footer size (bytes) */
#define DSIZE 8     /* Double word size (bytes) */
#define CHUNKSIZE (1<<12)     /* Extend heap by this amount (bytes) */
#define INIT_CHUNKSIZE (1<<6)
#define LIST_LIMIT 20

#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) < (y) ? (x) : (y))

/* Pack a size and allocated bit into  word */
#define PACK(size, alloc) ((size) | (alloc))

/* Read and write a word at address p */
#define GET(p) (*(unsigned int *)(p))
#define PUT(p, val) (*(unsigned int *)(p) = (val))

/* Read and write a pointer at address p */
#define GET_PTR(p) ((unsigned int *)(long)(GET(p)))
#define PUT_PTR(p, ptr) (*(unsigned int *)(p) = ((long)ptr))
#define SET_PTR(p, ptr) (*(unsigned int *)(p) = (unsigned int)(ptr))

/* Read the size and allocated fields from address p */
#define GET_SIZE(p) (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) & 0x1)
#define GET_TAG(p) (GET(p) & 0x2)
#define SET_TAG(p) (GET(p) |= 0x2)
#define REMOVE_TAG(p) (GET(p) &= ~0x2)

/* Given block ptr bp, compute address of its header and footer */
#define HDRP(bp) ((char *)(bp) - WSIZE)
#define FTRP(bp) ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)

/* Given block ptr bp, compute address of next and previous blocks */
#define NEXT_BLKP(bp) ((char *)(bp) + GET_SIZE(((char *)(bp) - WSIZE)))
#define PREV_BLKP(bp) ((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE)))

/* Given block ptr bp, compute address of pred and succ blocks */
#define PRED_BLKP(bp) ((char *)(GET(bp)))
#define SUCC_BLKP(bp) ((char *)(GET((char *)(bp) + WSIZE)))

// Address of free block's predecessor and successor entries 
#define PRED_PTR(ptr) ((char *)(ptr))
#define SUCC_PTR(ptr) ((char *)(ptr) + WSIZE)

// Address of free block's predecessor and successor on the segregated list 
#define PRED(ptr) (*(char **)(ptr))
#define SUCC(ptr) (*(char **)(SUCC_PTR(ptr)))

void *segregated_free_list[LIST_LIMIT];

static void *extend_heap(size_t words);
static void *coalesce(void *bp);
static void *place(void *bp, size_t asize);
static void insert_node(void *bp, size_t size);
static void delete_node(void *bp);

/*  Help Functions */

static void insert_node(void *ptr, size_t size) {
    int list = 0;
    void *search_ptr = ptr;
    void *insert_ptr = NULL;
    
    // Select segregated list 
    while ((list < LIST_LIMIT - 1) && (size > 1)) {
        size >>= 1;
        list++;
    }
    
    // Keep size ascending order and search
    search_ptr = segregated_free_list[list];
    while ((search_ptr != NULL) && (size > GET_SIZE(HDRP(search_ptr)))) {
        insert_ptr = search_ptr;
        search_ptr = PRED(search_ptr);
    }
    
    // Set predecessor and successor 
    if (search_ptr != NULL) {
        if (insert_ptr != NULL) {
            SET_PTR(PRED_PTR(ptr), search_ptr);
            SET_PTR(SUCC_PTR(search_ptr), ptr);
            SET_PTR(SUCC_PTR(ptr), insert_ptr);
            SET_PTR(PRED_PTR(insert_ptr), ptr);
        } else {
            SET_PTR(PRED_PTR(ptr), search_ptr);
            SET_PTR(SUCC_PTR(search_ptr), ptr);
            SET_PTR(SUCC_PTR(ptr), NULL);
            segregated_free_list[list] = ptr;
        }
    } else {
        if (insert_ptr != NULL) {
            SET_PTR(PRED_PTR(ptr), NULL);
            SET_PTR(SUCC_PTR(ptr), insert_ptr);
            SET_PTR(PRED_PTR(insert_ptr), ptr);
        } else {
            SET_PTR(PRED_PTR(ptr), NULL);
            SET_PTR(SUCC_PTR(ptr), NULL);
            segregated_free_list[list] = ptr;
        }
    }
    
    return;
}


static void delete_node(void *ptr) {
    int list = 0;
    size_t size = GET_SIZE(HDRP(ptr));
    
    // Select segregated list 
    while ((list < LIST_LIMIT - 1) && (size > 1)) {
        size >>= 1;
        list++;
    }
    
    if (PRED(ptr) != NULL) {
        if (SUCC(ptr) != NULL) {
            SET_PTR(SUCC_PTR(PRED(ptr)), SUCC(ptr));
            SET_PTR(PRED_PTR(SUCC(ptr)), PRED(ptr));
        } else {
            SET_PTR(SUCC_PTR(PRED(ptr)), NULL);
            segregated_free_list[list] = PRED(ptr);
        }
    } else {
        if (SUCC(ptr) != NULL) {
            SET_PTR(PRED_PTR(SUCC(ptr)), NULL);
        } else {
            segregated_free_list[list] = NULL;
        }
    }
    
    return;
}


/*
 * extend_heap - Extends the heap with a new free block
 *
 */
static void *extend_heap(size_t words) {
    void *bp;
    size_t size;

    /* Allocate words to maintain alignment */
    size = ALIGN(words);
    if ((long)(bp = mem_sbrk(size)) == (void *)(-1)) {
        return NULL;
    }

    /* Init free block header/footer and the epilogue header */
    PUT(HDRP(bp), PACK(size, 0)); // free block header
    PUT(FTRP(bp), PACK(size, 0)); // free block footer
    PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1)); // new epilogue header
    insert_node(bp, size);

    return coalesce(bp);
}

/*
 * coalesce - Use boundary-tag coalescing to merge freed block.
 *     Merge with any adjacent free blocks in constant time
 */
static void *coalesce(void *bp) {
    size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(bp)));
    size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
    size_t size = GET_SIZE(HDRP(bp));

    if (GET_TAG(HDRP(PREV_BLKP(bp)))) { // if previous block is tagged with reallocation tag, don't coalesce
        prev_alloc = 1;
    }

    if (prev_alloc && next_alloc) { // case 1
        return bp;
    }

    else if (prev_alloc && !next_alloc) { // case 2
        delete_node(bp);
        delete_node(NEXT_BLKP(bp));
        size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
        PUT(HDRP(bp), PACK(size, 0));
        PUT(FTRP(bp), PACK(size, 0));
    }

    else if (!prev_alloc && next_alloc) { // case 3
        delete_node(bp);
        delete_node(PREV_BLKP(bp));
        size += GET_SIZE(HDRP(PREV_BLKP(bp)));
        PUT(FTRP(bp), PACK(size, 0));
        PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
        bp = PREV_BLKP(bp);
    }

    else { // case 4
        delete_node(bp);
        delete_node(PREV_BLKP(bp));
        delete_node(NEXT_BLKP(bp));
        size += (GET_SIZE(HDRP(NEXT_BLKP(bp))) + GET_SIZE(HDRP(PREV_BLKP(bp))));
        PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
        PUT(FTRP(NEXT_BLKP(bp)), PACK(size, 0));
        bp = PREV_BLKP(bp);
    }

    insert_node(bp, size);

    return bp;
}

/*
 * place - Perform a first-fit search of the implicit free list
 *
 */
static void *place(void *ptr, size_t asize)
{
    size_t ptr_size = GET_SIZE(HDRP(ptr));
    size_t remainder = ptr_size - asize;
    
    delete_node(ptr);
    
    
    if (remainder <= DSIZE * 2) {
        // Do not split block 
        PUT(HDRP(ptr), PACK(ptr_size, 1)); 
        PUT(FTRP(ptr), PACK(ptr_size, 1)); 
    }
    
    else if (asize >= 100) {
        // Split block
        PUT(HDRP(ptr), PACK(remainder, 0));
        PUT(FTRP(ptr), PACK(remainder, 0));
        PUT(HDRP(NEXT_BLKP(ptr)), PACK(asize, 1));
        PUT(FTRP(NEXT_BLKP(ptr)), PACK(asize, 1));
        insert_node(ptr, remainder);
        return NEXT_BLKP(ptr);
        
    }
    
    else {
        // Split block
        PUT(HDRP(ptr), PACK(asize, 1)); 
        PUT(FTRP(ptr), PACK(asize, 1)); 
        PUT(HDRP(NEXT_BLKP(ptr)), PACK(remainder, 0)); 
        PUT(FTRP(NEXT_BLKP(ptr)), PACK(remainder, 0)); 
        insert_node(NEXT_BLKP(ptr), remainder);
    }
    return ptr;
}

/*  MM Functions   */

/* 
 * mm_init - initialize the malloc package, return -1 when problem detected, otherwise return 0.
 */
int mm_init(void) {

    char *heap_listp; // beginning of heap
    int i = 0;

    for (i = 0; i < LIST_LIMIT; i++) {
        segregated_free_list[i] = NULL;
    }

    /* Create the init empty heap */
    if ((heap_listp = mem_sbrk(4 * WSIZE)) == (void *)(-1)) {
        return -1;
    }

    PUT(heap_listp, 0); // alignment padding
    PUT(heap_listp + (1 * WSIZE), PACK(DSIZE, 1)); // prologue header
    PUT(heap_listp + (2 * WSIZE), PACK(DSIZE, 1)); // prologue footer
    PUT(heap_listp + (3 * WSIZE), PACK(0, 1)); // epilogue header
    heap_listp += (2 * WSIZE);

    /* Extend the empty heap with a free block of CHUNKSIZE bytes */
    if (extend_heap(INIT_CHUNKSIZE) == NULL) {
        return -1;
    }
    return 0;
}

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size) {
    size_t asize;
    size_t extend_size;


    // Ignore size 0 cases
    if (!size)
        return NULL;
    
    // Align block size
    if (size <= DSIZE) {
        asize = 2 * DSIZE;
    } else {
        asize = ALIGN(size + DSIZE);
    }

    int i = 0;
    void *ptr;
    size_t cur_size = asize;
    while (i < LIST_LIMIT) {
        if ((i == LIST_LIMIT - 1) || ((cur_size <= 1) && (segregated_free_list[i] != NULL))) {
            ptr = segregated_free_list[i];
        }

        // Ignore blocks that are too small or marked with the reallocation bit
        while ((ptr != NULL) && ((asize > GET_SIZE(HDRP(ptr))) || (GET_TAG(HDRP(ptr))))) {
            ptr = PRED(ptr);
        }
        if (ptr != NULL) {
            break;
        }

        cur_size = cur_size >> 1;
        i++;
    }

    // free block is not found, then extend heap
    if (ptr == NULL) {
        extend_size = MAX(asize, CHUNKSIZE);
        
        if ((ptr = extend_heap(extend_size)) == NULL) {
            return NULL;
        }
    }
    
    // Place block
    ptr = place(ptr, asize);
    
    return ptr;
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *bp) {
    size_t size = GET_SIZE(HDRP(bp));

    REMOVE_TAG(HDRP(NEXT_BLKP(bp)));
    PUT(HDRP(bp), PACK(size, 0));
    PUT(FTRP(bp), PACK(size, 0));

    insert_node(bp, size);
    coalesce(bp);
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size) {
    // Special cases 
    if (ptr == NULL) {
        return mm_malloc(size);
    }
    if (size == 0) {
        mm_free(ptr);
        return NULL;
    }

    size_t new_size = size;
    void *insert;
    int buffer;
    int fragmentation;
    int extend_size;

    // Align block size
    if (new_size <= DSIZE) {
        new_size = 2 * DSIZE;
    } else {
        new_size = ALIGN(new_size + DSIZE);
    }

    new_size += (1 << 7);
    buffer = GET_SIZE(HDRP(ptr) - new_size);

    if (!buffer) {
        if (!GET_ALLOC(HDRP(NEXT_BLKP(ptr))) || !GET_SIZE(HDRP(NEXT_BLKP(ptr)))) {
            fragmentation = GET_SIZE(HDRP(ptr)) + GET_SIZE(HDRP(NEXT_BLKP(ptr))) - new_size;
            if (fragmentation < 0) {
                extend_size = MAX(-fragmentation, CHUNKSIZE);
                if (extend_heap(extend_size) == NULL) {
                    return NULL;
                }
                fragmentation += extend_size;
            }
            
            delete_node(NEXT_BLKP(ptr));
            
            // Do not split block
            PUT(HDRP(ptr), PACK(new_size + fragmentation, 1)); 
            PUT(FTRP(ptr), PACK(new_size + fragmentation, 1)); 
        }
        else {
            insert = mm_malloc(new_size - DSIZE);
            memcpy(insert, ptr, MIN(size, new_size));
            mm_free(ptr);
        }
        buffer = GET_SIZE(HDRP(insert)) - new_size;
    }

    if (buffer < 2 * (1 << 7)) {
        SET_TAG(HDRP(NEXT_BLKP(insert)));
    }

    return insert;
}