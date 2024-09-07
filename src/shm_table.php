<?php declare(strict_types=1);
namespace stackware\ffi\posix_shm;



use FFI;

abstract class shm_table {
    public FFI $ffi;
    public $shm_addr;
    public string $name;
    public int $row_size;
    public int $max_rows;
    public array $columns = [];
    public array $column_offsets = [];

    public function __construct(string $name, int $max_rows, array $columns, int $oflag, int $mode) {
        $this->ffi = FFI::cdef(file_get_contents("ext/shm_table.h"), "ext/lib/shm_table.so");

        $this->name = $name;
        $this->max_rows = $max_rows;
        $this->columns = $columns;

        $this->row_size = 0;
        foreach ($columns as $column => $length) {
            $this->column_offsets[$column] = $this->row_size;
            $this->row_size += max(1, $length);
        }

        $size = $this->max_rows * $this->row_size;

        $this->shm_addr = $this->ffi->shm_table_create($name, $size, $oflag, $mode);

        if ($this->shm_addr === null) {
            throw new \RuntimeException("Failed to create shared memory segment");
        }
    }

    public function __destruct() {
        if ($this->shm_addr !== null) {
            $this->ffi->shm_table_detach($this->shm_addr, $this->max_rows * $this->row_size);
            $this->ffi->shm_table_unlink($this->name);
        }
    }

    public function getRowOffset(int $row): int {
        return $row * $this->row_size;
    }

    public function writeRow(int $row, array $data): void {
        $buffer = FFI::new("char[{$this->row_size}]");
        $offset = 0;
        foreach ($this->columns as $column => $length) {
            $value = $data[$column] ?? '';
            $value = substr($value, 0, $length);
            FFI::memcpy(FFI::addr($buffer) + $offset, $value, strlen($value));
            $offset += $length;
        }
        $this->ffi->shm_table_write($this->shm_addr, $this->getRowOffset($row), FFI::addr($buffer), $this->row_size);
    }

    public function readRow(int $row): array {
        $buffer = FFI::new("char[{$this->row_size}]");
        $this->ffi->shm_table_read($this->shm_addr, $this->getRowOffset($row), FFI::addr($buffer), $this->row_size);
        
        $result = [];
        $offset = 0;
        foreach ($this->columns as $column => $length) {
            $result[$column] = rtrim(FFI::string(FFI::addr($buffer) + $offset, $length), "\0");
            $offset += $length;
        }
        return $result;
    }

    public function writeColumn(int $row, string $column, string $value): void {
        if (!isset($this->columns[$column])) {
            throw new \InvalidArgumentException("Invalid column: $column");
        }
        $length = $this->columns[$column];
        $value = substr($value, 0, $length);
        $buffer = FFI::new("char[$length]");
        FFI::memcpy(FFI::addr($buffer), $value, strlen($value));
        $offset = $this->getRowOffset($row) + $this->column_offsets[$column];
        $this->ffi->shm_table_write($this->shm_addr, $offset, FFI::addr($buffer), $length);
    }

    public function readColumn(int $row, string $column): string {
        if (!isset($this->columns[$column])) {
            throw new \InvalidArgumentException("Invalid column: $column");
        }
        $length = $this->columns[$column];
        $offset = $this->getRowOffset($row) + $this->column_offsets[$column];
        $buffer = FFI::new("char[$length]");
        $this->ffi->shm_table_read($this->shm_addr, $offset, FFI::addr($buffer), $length);
        return rtrim(FFI::string($buffer), "\0");
    }
}


