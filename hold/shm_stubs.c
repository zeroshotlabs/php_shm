#include "posix_shm.h"
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>



// Shared Memory Operations
void* posix_shm_create(const char* name, size_t size, int oflag, mode_t mode) {
    int fd = shm_open(name, oflag, mode);
    if (fd == -1) {
        perror("shm_open");
        return NULL;
    }

    if (ftruncate(fd, size) == -1) {
        perror("ftruncate");
        close(fd);
        return NULL;
    }
    
    void* addr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);
    if (addr == MAP_FAILED) {
        perror("mmap");
        return NULL;
    }
    
    return addr;
}

int posix_shm_unlink(const char* name) {
    return shm_unlink(name);
}

void* posix_shm_attach(const char* name, size_t size, int oflag) {
    int fd = shm_open(name, oflag, 0);
    if (fd == -1) {
        perror("shm_open");
        return NULL;
    }
    
    void* addr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);
    if (addr == MAP_FAILED) {
        perror("mmap");
        return NULL;
    }
    
    return addr;
}

int posix_shm_detach(void* addr, size_t size) {
    return munmap(addr, size);
}

// Row/Column Operations
void write_row(void* base, int row_offset, int* data, int row_size) {
    memcpy(base + row_offset, data, row_size);
}

void read_row(void* base, int row_offset, int* buffer, int row_size) {
    memcpy(buffer, base + row_offset, row_size);
}

void write_column(void* base, int row_offset, int col_offset, int value) {
    memcpy(base + row_offset + col_offset, &value, sizeof(int));
}

int read_column(void* base, int row_offset, int col_offset) {
    int value;
    memcpy(&value, base + row_offset + col_offset, sizeof(int));
    return value;
}


// Circular Buffer Management
void push_row(void* base, int* head, int* tail, int capacity, int row_size, int* data) {
    int head_pos = *head;
    int tail_pos = *tail;
    int next_head = (head_pos + 1) % capacity;

    if (next_head == tail_pos) {
        // Buffer is full, advance the tail to drop the oldest entry
        *tail = (tail_pos + 1) % capacity;
    }

    write_row(base, head_pos * row_size, data, row_size);
    *head = next_head;
}


void pop_row(void* base, int* head, int* tail, int capacity, int row_size, int* buffer) {
    int tail_pos = *tail;

    if (tail_pos == *head) {
        fprintf(stderr, "Buffer is empty\n");
        return;
    }

    read_row(base, tail_pos * row_size, buffer, row_size);
    *tail = (tail_pos + 1) % capacity;
}


int buffer_size(int head, int tail, int capacity) {
    if (head >= tail) {
        return head - tail;
    } else {
        return capacity - tail + head;
    }
}


int buffer_is_empty(int head, int tail) {
    return head == tail;
}


int buffer_is_full(int head, int tail, int capacity) {
    return (head + 1) % capacity == tail;
}


