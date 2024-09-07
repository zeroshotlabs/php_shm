<?php declare(strict_types=1);
namespace stackware\ffi\posix_shm;

use FFI;

const SHM_MODE = 0666;
const SHM_OF_LAG = 0x02;

// $ffi = FFI::cdef("
//     void* posix_shm_create(const char* name, size_t size, int oflag, mode_t mode);
//     int posix_shm_unlink(const char* name);
//     void* posix_shm_attach(const char* name, size_t size, int oflag);
//     int posix_shm_detach(void* addr, size_t size);

//     void write_row(void* base, int row_offset, int* data, int row_size);
//     void read_row(void* base, int row_offset, int* buffer, int row_size);
//     void write_column(void* base, int row_offset, int col_offset, int value);
//     int read_column(void* base, int row_offset, int col_offset);

//     void push_row(void* base, int* head, int* tail, int capacity, int row_size, int* data);
//     void pop_row(void* base, int* head, int* tail, int capacity, int row_size, int* buffer);
//     int buffer_size(int head, int tail, int capacity);
//     int buffer_is_empty(int head, int tail);
//     int buffer_is_full(int head, int tail, int capacity);
// ", "stubs.so");


require('src/shm_table.php');
require('src/shm_deque.php');


$ffi = FFI::cdef(file_get_contents("ext/shm_table.h"), "ext/shm_table.so");



