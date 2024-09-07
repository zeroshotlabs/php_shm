#include "shm_table.h"
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

void* shm_table_create(const char* name, size_t size, int oflag, mode_t mode) {
    int fd = shm_open(name, oflag, mode);
    if (fd == -1) return NULL;
    if (ftruncate(fd, size) == -1) {
        close(fd);
        return NULL;
    }
    void* addr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);
    return (addr == MAP_FAILED) ? NULL : addr;
}

int shm_table_unlink(const char* name) {
    return shm_unlink(name);
}

void* shm_table_attach(const char* name, size_t size, int oflag) {
    int fd = shm_open(name, oflag, 0);
    if (fd == -1) return NULL;
    void* addr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);
    return (addr == MAP_FAILED) ? NULL : addr;
}

int shm_table_detach(void* addr, size_t size) {
    return munmap(addr, size);
}

void shm_table_write(void* addr, size_t offset, const void* data, size_t size) {
    memcpy((char*)addr + offset, data, size);
}

void shm_table_read(void* addr, size_t offset, void* buffer, size_t size) {
    memcpy(buffer, (char*)addr + offset, size);
}


