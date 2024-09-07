<?php declare(strict_types=1);
namespace stackware\ffi\posix_shm;


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



class posix_shm_deque extends posix_shm_table_base
{
    // Metadata offsets
    private const HEAD_OFFSET = 0;
    private const TAIL_OFFSET = 8;
    private const DATA_OFFSET = 16;

    private int $capacity;

    public function __construct(string $name, int $capacity, int $max_rows, array $columns, int $oflag, int $mode) {
        // Calculate the required size: head (8 bytes) + tail (8 bytes) + data
        $size = self::DATA_OFFSET + ($capacity * count($columns) * 4); // Assuming 4 bytes per int per column
        parent::__construct($name, $max_rows, $columns, $oflag, $mode);
        $this->capacity = $capacity;

        // Initialize head and tail pointers if needed
        if ($this->readColumn(self::HEAD_OFFSET, $columns[0]) == 0 && $this->readColumn(self::TAIL_OFFSET, $columns[0]) == 0) {
            $this->writeColumn(self::HEAD_OFFSET, $columns[0], 0);
            $this->writeColumn(self::TAIL_OFFSET, $columns[0], 0);
        }
    }

    public function push(array $row): void {
        $head = $this->readColumn(self::HEAD_OFFSET, $this->columns[0]);
        $tail = $this->readColumn(self::TAIL_OFFSET, $this->columns[0]);
        $next_head = ($head + 1) % $this->capacity;

        if ($next_head == $tail) {
            // Deque is full, advance the tail to drop the oldest entry
            $tail = ($tail + 1) % $this->capacity;
            $this->writeColumn(self::TAIL_OFFSET, $this->columns[0], $tail);
        }

        $this->writeRow($head, $row);
        $this->writeColumn(self::HEAD_OFFSET, $this->columns[0], $next_head);
    }

    public function pop(): array {
        $tail = $this->readColumn(self::TAIL_OFFSET, $this->columns[0]);

        if ($tail == $this->readColumn(self::HEAD_OFFSET, $this->columns[0])) {
            throw new \RuntimeException("Deque is empty");
        }

        $row = $this->readRow($tail);
        $this->writeColumn(self::TAIL_OFFSET, $this->columns[0], ($tail + 1) % $this->capacity);
        
        return $row;
    }

    public function size(): int {
        $head = $this->readColumn(self::HEAD_OFFSET, $this->columns[0]);
        $tail = $this->readColumn(self::TAIL_OFFSET, $this->columns[0]);

        if ($head >= $tail) {
            return $head - $tail;
        } else {
            return $this->capacity - $tail + $head;
        }
    }

    public function isEmpty(): bool {
        return $this->readColumn(self::HEAD_OFFSET, $this->columns[0]) == $this->readColumn(self::TAIL_OFFSET, $this->columns[0]);
    }

    public function isFull(): bool {
        return ($this->readColumn(self::HEAD_OFFSET, $this->columns[0]) + 1) % $this->capacity == $this->readColumn(self::TAIL_OFFSET, $this->columns[0]);
    }
}