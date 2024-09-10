<?php declare(strict_types=1);
namespace stackware\phphi\pshm;

define('_HOME',__DIR__.'/');
define('_PARENT',realpath(__DIR__.'/../'));


// const SHM_MODE = 0666;
// const SHM_OF_LAG = 0x02;

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


require(_HOME.'src/shm_table.php');
require(_HOME.'src/shm_deque.php');


class phphi
{
    public static function shm_create(string $name, int $size, int $oflag, int $mode)
    {
        return \posix_shm_create($name, $size, $oflag, $mode);
    }

    public static function shm_unlink(string $name)
    {
        return \posix_shm_unlink($name);
    }

    public static function shm_attach(string $name, int $size, int $oflag)
    {
        return \posix_shm_attach($name, $size, $oflag);
    }

    public static function shm_detach($addr, int $size)
    {
        return \posix_shm_detach($addr, $size);
    }
}


$ffi = FFI::cdef(file_get_contents(_HOME.'ext/shm_table.h'), _HOME.'ext/shm_table.so');




// $shm_name = "/my_shared_memory";
// $sem_name = "/my_semaphore";
// $capacity = 100; // Maximum number of items in the deque
// $shm_size = 16 + ($capacity * 256); // head + tail + data
// $oflag = O_CREAT | O_RDWR;
// $mode = 0666;

// try {
//     // Initialize deque
//     $deque = new posix_shm_deque($shm_name, $capacity, $oflag, $mode, $sem_name);

//     // Push items
//     $deque->push("First Item");
//     $deque->push("Second Item");

//     // Pop items
//     echo $deque->pop() . PHP_EOL; // Outputs: First Item
//     echo $deque->pop() . PHP_EOL; // Outputs: Second Item

//     // Check if empty
//     var_dump($deque->isEmpty()); // bool(true)

//     // Clean up
//     $deque->delete();
// } catch (Exception $e) {
//     echo "Error: " . $e->getMessage() . PHP_EOL;
// }

