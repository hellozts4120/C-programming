/*
 * mm.c - A malloc package using size-ordered segregated free lists.
 *
 * In this approach, the block layout is shown below:
 *
 *      Allocated block:
 *          [Header: <size><alloc><prev_alloc><realloc>]
 *          [Payload and padding]
 *
 *      Free block:
 *          [Header: <size><alloc><prev_alloc><realloc>]
 *          [Pointer to its predecessor in the segregated list]
 *          [Pointer to its successor in the segregated list]
 *          [...]
 *          [Footer: <size><alloc><0><0>]
 *
 * Each free block is stored in one of the segregated free lists with proper size.
 * The n-th segregated free list contains free blocks whose size spans 2^n to 2^(n+1)-1 in bytes.
 * The free blocks in each segregated free list are sorted by size ascending order for a convenient best-fit search.
 *
 * Coalescing is performed immediately after each heap extension and free operation.
 * Reallocation is performed with additional buffer to the target block and a reallocation tag
 * to guarantee that the next reallocation can be done without extending the heap.
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
    "14307110190",
    /* First member's full name */
    "Tianshu Zhou",
    /* First member's email address */
    "14307110190@fudan.edu.cn",
    /* Second member's full name (leave blank if none) */
    "",
    /* Second member's email address (leave blank if none) */
    ""
};

// MARK: - type definations

typedef void * blkp_t;

// MARK: - global variables

static char *heap_listp;       /* pointer to the very first block */
static blkp_t *seg_free_lists; /* array of pointers to the first block of each segregated free lists */

// MARK: - macros

/* target platform's pointer size (bytes) */
#define PTRSIZE 4

/* number of segregated free lists */
#define LIST_COUNT 20

/* size of the smallest block (bytes) */
#define MINSIZE 16

/* size of the reallocation buffer (bytes) */
#define REALLOC_BUFFER (1 << 7)

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT - 1)) & ~0x7)

/* basic constants and macros */
#define WSIZE          4        /* word and header/footer size (bytes) */
#define DSIZE          8        /* double word size (bytes) */
#define CHUNKSIZE     (1 << 12) /* extend heap by this amount (bytes) */
#define INITCHUNKSIZE (1 << 6)  /* extend heap by this amount (bytes) when init */

#define MAX(x, y) ((x) > (y) ? (x) : (y))

/* pack a size and allocated bit into a word */
#define PACK(size, alloc) ((size) | (alloc))

/* read and write a word at address p */
#define GET(p)      (*(unsigned int *)(p))
#define PUT(p, val) (GET(p) = (val))

/* write header info at address p while keeping its tag(s) unmodified */
#define PUT_PSRV_PREV_ALLOC(p, val) (GET(p) = (val) | GET_PREV_ALLOC(p))
#define PUT_PSRV_TAGS(p, val)       (GET(p) = (val) | GET_PREV_ALLOC(p) | GET_REALLOC(p))

/* read the size, (prev) allocated tag and reallocate tag from address p */
#define GET_SIZE(p)       (GET(p) & ~0x7)
#define GET_ALLOC(p)      (GET(p) & 0x1)
#define GET_PREV_ALLOC(p) (GET(p) & 0x2)
#define GET_REALLOC(p)    (GET(p) & 0x4)

/* given block ptr bp, compute address of its header and footer */
#define HDRP(bp) ((char *)(bp) - WSIZE)
#define FTRP(bp) ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)

/* given block ptr bp, compute address of next and previous blocks */
#define NEXT_BLKP(bp) ((char *)(bp) + GET_SIZE(((char *)(bp) - WSIZE)))
#define PREV_BLKP(bp) ((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE)))

/* given block ptr bp, set next block's prev allocated tag */
#define SET_ALLOC(bp)   (GET(HDRP(NEXT_BLKP(bp))) |= 0x2)
#define SET_UNALLOC(bp) (GET(HDRP(NEXT_BLKP(bp))) &= ~0x2)

/* given block ptr bp, set next block's reallocate tag */
#define SET_REALLOC(bp)   (GET(HDRP(NEXT_BLKP(bp))) |= 0x4)
#define SET_UNREALLOC(bp) (GET(HDRP(NEXT_BLKP(bp))) &= ~0x4)

/* given free block ptr bp, compute address of its predecessor and successor in the segregated list */
#define PRED_BLKP(bp) (*((blkp_t *)(bp)))
#define SUCC_BLKP(bp) (*((blkp_t *)((char *)(bp) + PTRSIZE)))

/* the error type that mm_check checks */
#define CHECKTYPE 0


// MARK: - helper function declarations

static void insert_node(blkp_t bp);
static void delete_node(blkp_t bp);
static blkp_t coalesce(blkp_t bp);
static blkp_t extend_heap(size_t size);
static blkp_t place(blkp_t bp, size_t asize);
static blkp_t find_fit(size_t asize);
static int mm_check();

// MARK: - helper function definations

/*
 * insert_node - Insert a free block into a proper segregated free list.
 */
static void insert_node(blkp_t bp) {
    size_t tmp_size, size;
    tmp_size = size = GET_SIZE(HDRP(bp));
    int list_num = 0;
    blkp_t cur;
    blkp_t pred = NULL;
    
    /* find a segregated free list with proper size */
    while ((list_num < LIST_COUNT - 1) && (tmp_size > 1)) {
        tmp_size >>= 1;
        list_num++;
    }
    
    /* search the segregated free list and keep an ascending order of size */
    cur = seg_free_lists[list_num];
    while ((cur != NULL) && (size > GET_SIZE(HDRP(cur)))) {
        pred = cur;
        cur = SUCC_BLKP(cur);
    }
    
    /* insert the block into the proper place of the segregated free list */
    if (cur != NULL) {
        // the block is not the first nor the last block of the segregated free list
        if (pred != NULL) {
            PRED_BLKP(bp) = pred;
            SUCC_BLKP(bp) = cur;
            SUCC_BLKP(pred) = bp;
            PRED_BLKP(cur) = bp;
        }
        // the block is the first block of the segregated free list
        else {
            PRED_BLKP(bp) = NULL;
            SUCC_BLKP(bp) = cur;
            PRED_BLKP(cur) = bp;
            seg_free_lists[list_num] = bp;
        }
    }
    else {
        // the block is the last block of the segregated free list
        if (pred != NULL) {
            PRED_BLKP(bp) = pred;
            SUCC_BLKP(bp) = NULL;
            SUCC_BLKP(pred) = bp;
        }
        // the block is the only one block of the segregated free list
        else {
            PRED_BLKP(bp) = NULL;
            SUCC_BLKP(bp) = NULL;
            seg_free_lists[list_num] = bp;
        }
    }
}

/*
 * delete_node - Delete a block from its segregated free list.
 */
static void delete_node(blkp_t bp) {
    size_t size = GET_SIZE(HDRP(bp));
    int list_num = 0;
    blkp_t pred = PRED_BLKP(bp);
    blkp_t succ = SUCC_BLKP(bp);
    
    /* find the segregated free list that the block belongs to */
    while ((list_num < LIST_COUNT - 1) && (size > 1)) {
        size >>= 1;
        list_num++;
    }
    
    /* delete the block from the segregated free list */
    if (succ != NULL) {
        // the block is not the first nor the last block of the segregated free list
        if (pred != NULL) {
            SUCC_BLKP(pred) = succ;
            PRED_BLKP(succ) = pred;
        }
        // the block is the first block of the segregated free list
        else {
            PRED_BLKP(succ) = NULL;
            seg_free_lists[list_num] = succ;
        }
    }
    else {
        // the block is the last block of the segregated free list
        if (pred != NULL) {
            SUCC_BLKP(pred) = NULL;
        }
        // the block is the only one block of the segregated free list
        else {
            seg_free_lists[list_num] = NULL;
        }
    }
}

/*
 * coalesce - Coalesce a free block and its available neighbors.
 */
static blkp_t coalesce(blkp_t bp) {
    size_t size = GET_SIZE(HDRP(bp));
    int prev_alloc = GET_PREV_ALLOC(HDRP(bp));
    int next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
    
    /* prevent the previous free block from coalescing if it is tagged with reallocation tag */
    if (!prev_alloc && GET_REALLOC(HDRP(PREV_BLKP(bp)))) {
        prev_alloc = 1;
    }
    
    /* coalescing */
    // case 1
    if (prev_alloc && next_alloc) {
        return bp;
    }
    // case 2
    else if (prev_alloc && !next_alloc) {
        delete_node(bp);
        delete_node(NEXT_BLKP(bp));
        size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
        PUT_PSRV_TAGS(HDRP(bp), PACK(size, 0));
        PUT(FTRP(bp), PACK(size, 0));
    }
    // case 3
    else if (!prev_alloc && next_alloc) {
        delete_node(bp);
        delete_node(PREV_BLKP(bp));
        size += GET_SIZE(HDRP(PREV_BLKP(bp)));
        PUT(FTRP(bp), PACK(size, 0));
        PUT_PSRV_TAGS(HDRP(PREV_BLKP(bp)), PACK(size, 0));
        bp = PREV_BLKP(bp);
    }
    // case 4
    else {
        delete_node(bp);
        delete_node(PREV_BLKP(bp));
        delete_node(NEXT_BLKP(bp));
        size += (GET_SIZE(HDRP(PREV_BLKP(bp))) + GET_SIZE(HDRP(NEXT_BLKP(bp))));
        PUT_PSRV_TAGS(HDRP(PREV_BLKP(bp)), PACK(size, 0));
        PUT(FTRP(NEXT_BLKP(bp)), PACK(size, 0));
        bp = PREV_BLKP(bp);
    }
    
    insert_node(bp);
    
    return bp;
}

/*
 * extend_heap - Extend heap to get a free block.
 */
static blkp_t extend_heap(size_t size) {
    blkp_t bp;
    size_t asize = ALIGN(size);
    
    /* allocate with alignment */
    if ((bp = mem_sbrk(asize)) == (blkp_t)-1) {
        return NULL;
    }
    
    /* initialize free block header/footer and the epilogue header */
    PUT_PSRV_PREV_ALLOC(HDRP(bp), PACK(asize, 0));
    PUT(FTRP(bp), PACK(asize, 0));
    PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1));
    
    insert_node(bp);
    return coalesce(bp);
}

/*
 * place - Place the newly allocated blocks, split the block if its size is big enough.
 */
static blkp_t place(blkp_t bp, size_t asize) {
    size_t csize = GET_SIZE(HDRP(bp));
    size_t remainder = csize - asize;
    
    delete_node(bp);
    
    // block too small, do not split
    if (remainder < MINSIZE) {
        PUT_PSRV_TAGS(HDRP(bp), PACK(csize, 1));
        SET_ALLOC(bp);
    }
    // block is large enough to be split
    else {
        // the allocated block's size is big, place it after the free block
        if (asize >= 120) {
            PUT_PSRV_TAGS(HDRP(bp), PACK(remainder, 0));
            PUT(FTRP(bp), PACK(remainder, 0));
            PUT(HDRP(NEXT_BLKP(bp)), PACK(asize, 1));
            SET_UNALLOC(bp);
            SET_ALLOC(NEXT_BLKP(bp));
            insert_node(bp);
            return NEXT_BLKP(bp);
        }
        // the allocated block's size is small, place it before the free block
        else {
            PUT_PSRV_TAGS(HDRP(bp), PACK(asize, 1));
            PUT(HDRP(NEXT_BLKP(bp)), PACK(remainder, 0));
            PUT(FTRP(NEXT_BLKP(bp)), PACK(remainder, 0));
            SET_ALLOC(bp);
            SET_UNALLOC(NEXT_BLKP(bp));
            insert_node(NEXT_BLKP(bp));
        }
    }
    
    return bp;
}

/*
 * find_fit - Find a best-fit free block in segregated free lists.
 */
static blkp_t find_fit(size_t asize) {
    blkp_t bp;
    size_t tmp_size = asize;
    int list_num = 0;
    
    /* search for a best-fit free block in segregated free lists */
    while (list_num < LIST_COUNT) {
        // the search reaches the last segregated free list or a segregated free list with proper size is found
        if ((list_num == LIST_COUNT - 1) || ((tmp_size <= 1) && (seg_free_lists[list_num] != NULL))) {
            // search the segregated free list, ignore blocks that are too small or tagged with reallocation tag
            bp = seg_free_lists[list_num];
            while ((bp != NULL) && ((asize > GET_SIZE(HDRP(bp))) || (GET_REALLOC(HDRP(bp))))) {
                bp = SUCC_BLKP(bp);
            }
            // the best-fit block is found
            if (bp != NULL) {
                return bp;
            }
        }
        tmp_size >>= 1;
        list_num++;
    }
    
    return NULL;
}

/*
 * mm_check - Heap consistency checker.
 */
static int mm_check() {
    int pass_flag = 1;
    int list_num;
    blkp_t cur;
    size_t size;
    int count;
    int tag_tmp;
    
    switch (CHECKTYPE) {
        /* check everything */
        case 0:
        /* check if every block in the segregated free lists is marked as free */
        case 1:
            for (list_num = 0; list_num < LIST_COUNT; list_num++) {
                cur = seg_free_lists[list_num];
                while (cur != NULL) {
                    if (GET_ALLOC(HDRP(cur))) {
                        printf("Case 1 failed.\n");
                        pass_flag = 0;
                    }
                    cur = SUCC_BLKP(cur);
                }
            }
            if (CHECKTYPE) {
                break;
            }
        
        /* check if every free block placed in the segregated free list match the size of the list */
        case 2:
            for (list_num = 0; list_num < LIST_COUNT; list_num++) {
                cur = seg_free_lists[list_num];
                while (cur != NULL) {
                    size = GET_SIZE(HDRP(cur));
                    if ((size < (1 << list_num)) || ((list_num != (LIST_COUNT - 1)) && size > (1 << (list_num + 1)))) {
                        printf("Case 2 failed.\n");
                        pass_flag = 0;
                    }
                    cur = SUCC_BLKP(cur);
                }
            }
            if (CHECKTYPE) {
                break;
            }
        
        /* check if every free block is placed in the segregated free list */
        case 3:
            /* count up the total number of free blocks */
            count = 0;
            cur = NEXT_BLKP(heap_listp);
            while (GET_SIZE(HDRP(cur))) {
                if (!GET_ALLOC(HDRP(cur))) {
                    count++;
                }
                cur = NEXT_BLKP(cur);
            }
            /* count down the number of free blocks placed in the segregated free list */
            for (list_num = 0; list_num < LIST_COUNT; list_num++) {
                cur = seg_free_lists[list_num];
                while (cur != NULL) {
                    count--;
                    cur = SUCC_BLKP(cur);
                }
            }
            /* check the count */
            if (count) {
                printf("Case 3 failed.\n");
                pass_flag = 0;
            }
            if (CHECKTYPE) {
                break;
            }
        
        /* check the correctness of every block's prev_alloc tag */
        case 4:
            cur = NEXT_BLKP(heap_listp);
            tag_tmp = 1;
            while (GET_SIZE(HDRP(cur))) {
                if (tag_tmp != !!GET_PREV_ALLOC(HDRP(cur))) {
                    printf("Case 4 failed.\n");
                    pass_flag = 0;
                }
                tag_tmp = GET_ALLOC(HDRP(cur));
                cur = NEXT_BLKP(cur);
            }
            if (CHECKTYPE) {
                break;
            }
            
        /* check if there are any contiguous free blocks that somehow escaped coalescing (only work when case 4 passed) */
        case 5:
            cur = NEXT_BLKP(heap_listp);
            while (GET_SIZE(HDRP(cur))) {
                if ((!GET_ALLOC(HDRP(cur))) && ((!GET_PREV_ALLOC(HDRP(cur)) && !GET_REALLOC(HDRP(PREV_BLKP(cur)))) || (!GET_ALLOC(HDRP(NEXT_BLKP(cur))) && !GET_REALLOC(HDRP(NEXT_BLKP(cur)))))) {
                    printf("Case 5 failed.\n");
                    pass_flag = 0;
                }
                cur = NEXT_BLKP(cur);
            }
            if (CHECKTYPE) {
                break;
            }
        
        default:
            break;
    }
    
    return pass_flag;
}

// MARK: - main function definations

/*
 * mm_init - Initialize the malloc package.
 */
int mm_init(void) {
    /* initialize segregated free lists */
    if ((seg_free_lists = mem_sbrk(ALIGN(LIST_COUNT * PTRSIZE))) == (blkp_t)-1) {
        return -1;
    }
    memset(seg_free_lists, 0, LIST_COUNT * PTRSIZE);
    
    /* create the initial empty heap */
    if ((heap_listp = mem_sbrk(4 * WSIZE)) == (blkp_t)-1) {
        return -1;
    }
    PUT(heap_listp, 0);
    PUT(heap_listp + (1 * WSIZE), PACK(DSIZE, 1));
    PUT(heap_listp + (2 * WSIZE), PACK(DSIZE, 1));
    PUT(heap_listp + (3 * WSIZE), PACK(0, 1));
    heap_listp += (2 * WSIZE);
    SET_ALLOC(heap_listp);
    
    /* extend the empty heap with a free block of INITCHUNKSIZE bytes */
    if (extend_heap(INITCHUNKSIZE) == NULL) {
        return -1;
    }
    
    return 0;
}

/*
 * mm_malloc - Allocate a block using best-fit placement strategy.
 */
void *mm_malloc(size_t size) {
    blkp_t bp;
    size_t asize;
    size_t extendsize;
    
    /* ignore spurious requests */
    if (size == 0) {
        return NULL;
    }
    
    /* adjust block size to include overhead and alignment reqs */
    if (size <= DSIZE) {
        asize = MINSIZE;
    }
    else {
        asize = ALIGN(size + WSIZE);
    }
    
    /* search the segregated free lists for a fit */
    if ((bp = find_fit(asize)) != NULL) {
        bp = place(bp, asize);
        return bp;
    }
    
    /* no fit found, get more memory and place the block */
    extendsize = MAX(asize, CHUNKSIZE);
    if ((bp = extend_heap(extendsize)) == NULL) {
        return NULL;
    }
    bp = place(bp, asize);
    return bp;
}

/*
 * mm_free - Free a block and place it into the corresponding segregated free list.
 */
void mm_free(void *ptr) {
    size_t size = GET_SIZE(HDRP(ptr));
    
    /* reset header and footer */
    PUT_PSRV_TAGS(HDRP(ptr), PACK(size, 0));
    PUT(FTRP(ptr), PACK(size, 0));
    SET_UNALLOC(ptr);
    SET_UNREALLOC(ptr);
    
    insert_node(ptr);
    coalesce(ptr);
    
    return;
}

/*
 * mm_realloc - Reallocate a block with additional buffer to guarantee that
 *              the next reallocation can be done without extending the heap.
 *
 *              If the buffer is not large enough for the next reallocation,
 *              tag the next block with the reallocation tag. With this tag,
 *              a free block cannot be used for allocation nor coalescing.
 *              The tag is cleared when the block is consumed by reallocation,
 *              when the heap is extended, or when the reallocated block is freed.
 */
void *mm_realloc(void *ptr, size_t size) {
    blkp_t new_ptr = ptr;
    size_t new_size;
    int cur_buffer;
    int remainder;
    int extendsize;
    
    /* ignore spurious requests */
    if (size == 0) {
        return NULL;
    }
    
    /* adjust block size to include overhead and alignment reqs */
    if (size <= DSIZE) {
        new_size = MINSIZE;
    }
    else {
        new_size = ALIGN(size + WSIZE);
    }
    
    /* add buffer to the block's new size */
    new_size += REALLOC_BUFFER;
    
    /* calculate current block's buffer */
    cur_buffer = GET_SIZE(HDRP(ptr))- new_size;
    
    /* no buffer, allocate more space */
    if (cur_buffer < 0) {
        // next block is a free block or the epilogue block
        if (!GET_ALLOC(HDRP(NEXT_BLKP(ptr))) || !GET_SIZE(HDRP(NEXT_BLKP(ptr)))) {
            remainder = GET_SIZE(HDRP(ptr)) + GET_SIZE(HDRP(NEXT_BLKP(ptr))) - new_size;
            if (remainder < 0) {
                extendsize = MAX(-remainder, CHUNKSIZE);
                if (extend_heap(extendsize) == NULL) {
                    return NULL;
                }
                remainder += extendsize;
            }
            delete_node(NEXT_BLKP(ptr));
            PUT_PSRV_PREV_ALLOC(HDRP(ptr), PACK(new_size + remainder, 1));
            SET_ALLOC(ptr);
        }
        // next block is an allocated block
        else {
            new_ptr = mm_malloc(new_size - WSIZE);
            memcpy(new_ptr, ptr, size);
            mm_free(ptr);
        }
        cur_buffer = GET_SIZE(HDRP(new_ptr)) - new_size;
    }
    
    /* current block's buffer is not large enough for the next reallocation */
    if (cur_buffer < 2 * REALLOC_BUFFER) {
        SET_REALLOC(HDRP(NEXT_BLKP(new_ptr)));
    }
    
    return new_ptr;
}

