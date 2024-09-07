#ifndef SHM_TABLE_H
#define SHM_TABLE_H

#include <sys/types.h>  // This defines mode_t
#include <fcntl.h>      // If you're using O_* constants like O_CREAT
#include <sys/mman.h>   // For shared memory related functions (shm_open, etc.)

typedef unsigned int mode_t;


void* shm_table_create(const char* name, size_t size, int oflag, mode_t mode);
int shm_table_unlink(const char* name);
void* shm_table_attach(const char* name, size_t size, int oflag);
int shm_table_detach(void* addr, size_t size);
void shm_table_write(void* addr, size_t offset, const void* data, size_t size);
void shm_table_read(void* addr, size_t offset, void* buffer, size_t size);



#endif // SHM_TABLE_H


// #ifndef SHM_TABLE_H
// #define SHM_TABLE_H

// #include <sys/types.h>
// #include <stddef.h>
// #include <fcntl.h>


// #endif // SHM_TABLE_H




// void* posix_shm_create(const char* name, size_t size, int oflag, mode_t mode);
// int posix_shm_unlink(const char* name);
// void* posix_shm_attach(const char* name, size_t size, int oflag);
// int posix_shm_detach(void* addr, size_t size);
// void memcpy(void* dest, const void* src, size_t n);

// void* posix_shm_create(const char* name, size_t size, int oflag, mode_t mode);
// int posix_shm_unlink(const char* name);
// void* posix_shm_attach(const char* name, size_t size, int oflag);
// int posix_shm_detach(void* addr, size_t size);

// void write_row(void* base, int row_offset, int* data, int row_size);
// void read_row(void* base, int row_offset, int* buffer, int row_size);
// void write_column(void* base, int row_offset, int col_offset, int value);
// int read_column(void* base, int row_offset, int col_offset);

// void push_row(void* base, int* head, int* tail, int capacity, int row_size, int* data);
// void pop_row(void* base, int* head, int* tail, int capacity, int row_size, int* buffer);
// int buffer_size(int head, int tail, int capacity);
// int buffer_is_empty(int head, int tail);
// int buffer_is_full(int head, int tail, int capacity);



// #endif // STUBS_H

