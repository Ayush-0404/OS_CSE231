


#include <stdio.h>
#include <stdlib.h>
 
#include <sys/mman.h>

#define PAGE_SIZE 4096

// Struct for a node in the sub-chain
typedef struct SubChainNode {
    void *start;  // Start address of the segment
    size_t size;  // Size of the segment
    int type;     // 0 for HOLE, 1 for PROCESS
    struct SubChainNode *next;
    struct SubChainNode *prev;
} SubChainNode;

// Struct for a node in the main chain
typedef struct MainChainNode {
    void *start;  // Start address of the main chain
    size_t size;  // Size of the main chain
    SubChainNode *sub_chain;
    struct MainChainNode *next;
    struct MainChainNode *prev;
} MainChainNode;

MainChainNode *free_list_head = NULL;  // Head of the free list
void *mems_virtual_address = NULL;     // MeMS virtual address
void *mems_physical_address = NULL;    // MeMS physical address

void mems_init() {
    // Initialize MeMS system
    // Create the initial free list node with the whole available memory
    MainChainNode *main_chain_node = (MainChainNode *)mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (main_chain_node == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }
    main_chain_node->start = main_chain_node;
    main_chain_node->size = PAGE_SIZE;
    main_chain_node->sub_chain = NULL;
    main_chain_node->next = main_chain_node->prev = NULL;

    free_list_head = main_chain_node;
    mems_virtual_address = main_chain_node;
}

void mems_finish() {
    // Unmap allocated memory and clean up
    MainChainNode *current_main_chain = free_list_head;
    while (current_main_chain != NULL) {
        SubChainNode *current_sub_chain = current_main_chain->sub_chain;
        while (current_sub_chain != NULL) {
            SubChainNode *temp = current_sub_chain;
            current_sub_chain = current_sub_chain->next;
            munmap(temp, temp->size);
        }
        MainChainNode *temp = current_main_chain;
        current_main_chain = current_main_chain->next;
        munmap(temp, temp->size);
    }
    free_list_head = NULL;
}void *mems_malloc(size_t size) {
    // Allocate memory using MeMS
    MainChainNode *current_main_chain = free_list_head;
    SubChainNode *selected_sub_chain = NULL;
    size_t aligned_size = (size + PAGE_SIZE - 1) / PAGE_SIZE * PAGE_SIZE;

    while (current_main_chain != NULL) {
        SubChainNode *current_sub_chain = current_main_chain->sub_chain;
        while (current_sub_chain != NULL) {
            if (current_sub_chain->type == 0 && current_sub_chain->size >= aligned_size) {
                // Found a suitable HOLE segment in the sub-chain
                selected_sub_chain = current_sub_chain;
                break;
            }
            current_sub_chain = current_sub_chain->next;
        }
        if (selected_sub_chain != NULL) {
            break;
        }
        current_main_chain = current_main_chain->next;
    }

    if (selected_sub_chain != NULL) {
        // Reuse the HOLE segment

        // Check if the sum of adjacent HOLEs is large enough
        size_t total_hole_size = selected_sub_chain->size;

        // Traverse the sub-chain to accumulate the total hole size and remove adjacent holes
        SubChainNode *hole_to_merge = selected_sub_chain->next;
        while (hole_to_merge != NULL && hole_to_merge->type == 0) {
            total_hole_size += hole_to_merge->size;
            SubChainNode *next_hole = hole_to_merge->next;
            if (next_hole != NULL && next_hole->type == 0) {
                // Merge the next hole into the current one
                hole_to_merge->size += next_hole->size;
                hole_to_merge->next = next_hole->next;
                if (next_hole->next != NULL) {
                    next_hole->next->prev = hole_to_merge;
                }
                // Unmap the next HOLE segment
                munmap(next_hole->start, next_hole->size);
                if (next_hole == current_main_chain->sub_chain) {
                    current_main_chain->sub_chain = hole_to_merge;
                }
            }
            hole_to_merge = hole_to_merge->next;
        }

        if (total_hole_size >= aligned_size) {
            // Create a new HOLE segment with the remaining memory using mmap
            SubChainNode *new_sub_chain = (SubChainNode *)mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
            if (new_sub_chain == MAP_FAILED) {
                perror("mmap");
                exit(1);
            }
            new_sub_chain->start = selected_sub_chain->start + aligned_size;
            new_sub_chain->size = total_hole_size - aligned_size;
            new_sub_chain->type = 0;
            new_sub_chain->next = hole_to_merge;
            new_sub_chain->prev = selected_sub_chain;
            if (hole_to_merge != NULL) {
                hole_to_merge->prev = new_sub_chain;
            }
            if (selected_sub_chain == current_main_chain->sub_chain) {
                current_main_chain->sub_chain = new_sub_chain;
            }
            selected_sub_chain->size = aligned_size;
            selected_sub_chain->type = 1;
            selected_sub_chain->next = new_sub_chain;
        } else {
            // The entire HOLE segment is used
            selected_sub_chain->type = 1;
        }
    } else {
        // No suitable segment found, allocate a new main chain node using mmap
        current_main_chain = (MainChainNode *)mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        if (current_main_chain == MAP_FAILED) {
            perror("mmap");
            exit(1);
        }
        current_main_chain->start = current_main_chain;
        current_main_chain->size = PAGE_SIZE;
        current_main_chain->sub_chain = NULL;
        current_main_chain->next = free_list_head;
        current_main_chain->prev = NULL;
        free_list_head->prev = current_main_chain;
        free_list_head = current_main_chain;

        // Create a new PROCESS segment in the sub-chain
        selected_sub_chain = (SubChainNode *)mmap(NULL, aligned_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        if (selected_sub_chain == MAP_FAILED) {
            perror("mmap");
            exit(1);
        }
        selected_sub_chain->start = current_main_chain->start;
        selected_sub_chain->size = aligned_size;
        selected_sub_chain->type = 1;
        selected_sub_chain->next = NULL;
        selected_sub_chain->prev = NULL;
        current_main_chain->sub_chain = selected_sub_chain;
    }

    return selected_sub_chain->start;
}

void mems_print_stats() {
    // Print MeMS system statistics
    int pages_used = 0;
    size_t space_unused = 0;
    MainChainNode *main_chain_node = free_list_head;
    int main_chain_length = 0;
    int sub_chain_lengths[100];  // Assuming a maximum of 100 sub-chains
    for (int i = 0; i < 100; i++) {
        sub_chain_lengths[i] = 0;
    }

    printf("----- MEMS SYSTEM STATS -----\n");

    while (main_chain_node != NULL) {
        main_chain_length++;
        printf("MAIN[%p:%p]->", main_chain_node->start, main_chain_node->start + main_chain_node->size - 1);
        SubChainNode *sub_chain_node = main_chain_node->sub_chain;

        while (sub_chain_node != NULL) {
            sub_chain_lengths[main_chain_length - 1]++;
            if (sub_chain_node->type == 0) {
                printf(" H[%p:%p] <->", sub_chain_node->start, sub_chain_node->start + sub_chain_node->size - 1);
            } else {
                pages_used++;
                printf(" P[%p:%p] <->", sub_chain_node->start, sub_chain_node->start + sub_chain_node->size - 1);
            }
            sub_chain_node = sub_chain_node->next;
        }

        printf(" NULL\n");
        
        // Calculate space_unused for the main chain node and add it to the total space_unused
        space_unused += main_chain_node->size;
        
        main_chain_node = main_chain_node->next;
    }

    printf("Pages used: %d\n", pages_used);
    printf("Space unused: %zu\n", space_unused);
    printf("Main Chain Length: %d\n", main_chain_length);
    printf("Sub-chain Length array: [");

    for (int i = 0; i < main_chain_length; i++) {
        printf("%d", sub_chain_lengths[i]);
        if (i < main_chain_length - 1) {
            printf(", ");
        }
    }

    printf("]\n");
}


void *mems_get(void *v_ptr) {
    // Get MeMS physical address corresponding to the provided MeMS virtual address


    return v_ptr;
    //return (void *)((char *)mems_physical_address + ((char *)v_ptr - (char *)mems_virtual_address));
}

 
void mergeAdjacentHolesInSubChain(SubChainNode *sub_chain_node) {
    while (sub_chain_node != NULL) {
        if (sub_chain_node->type == 0) {
            if (sub_chain_node->prev != NULL && sub_chain_node->prev->type == 0) {
                sub_chain_node->prev->size += sub_chain_node->size;
                sub_chain_node->prev->next = sub_chain_node->next;
                if (sub_chain_node->next != NULL) {
                    sub_chain_node->next->prev = sub_chain_node->prev;
                }
                free(sub_chain_node);
                sub_chain_node = sub_chain_node->prev;
            } else if (sub_chain_node->next != NULL && sub_chain_node->next->type == 0) {
                sub_chain_node->size += sub_chain_node->next->size;
                sub_chain_node->next = sub_chain_node->next->next;
                if (sub_chain_node->next != NULL) {
                    sub_chain_node->next->prev = sub_chain_node;
                }
                free(sub_chain_node->next);
            } else {
                break;
            }
        } else {
            break;
        }
    }
}

// The main mems_free function


void mems_free(void* v_ptr) {
    //printNodeDetails(&ptr);
    MainChainNode *main_chain_node = free_list_head;
    SubChainNode *sub_chain_node = NULL;

    // Find the sub_chain_node for the provided MeMS virtual address
    while (main_chain_node != NULL) {
        SubChainNode *sub_node = main_chain_node->sub_chain;
        while (sub_node != NULL) {
            if (sub_node->start == v_ptr) {
                sub_chain_node = sub_node;
                break;
            }
            sub_node = sub_node->next;
        }
        if (sub_chain_node != NULL) {
            break;
        }
        main_chain_node = main_chain_node->next;
    }

    if (sub_chain_node != NULL) {
        // Mark the segment as a HOLE
        sub_chain_node->type = 0;
        
        // Merge adjacent HOLEs in the sub-chain
        mergeAdjacentHolesInSubChain(sub_chain_node);

        // Merge adjacent HOLEs in the main chain
        main_chain_node = free_list_head;
        while (main_chain_node != NULL) {
            mergeAdjacentHolesInSubChain(main_chain_node->sub_chain);
            main_chain_node = main_chain_node->next;
        }
    }
}
