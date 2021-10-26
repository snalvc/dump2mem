#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

int main(int argc, char const *argv[]) {
  const uint32_t offset_mask = getpagesize() - 1;
  const uint32_t pfn_mask = ~offset_mask;
  int rv = 0;
  int input_fd, mem_fd;
  size_t input_length;
  uint32_t phy_addr;
  void *file_base, *mem_base;

  if (argc < 3) {
    fprintf(stderr, "Usage: %s <FILE TO BE DUMP> <PHY ADDR>\n", argv[0]);
  }

  input_fd = open(argv[1], O_RDONLY);
  if (input_fd < 0) {
    perror("open()");
    exit(errno);
  }
  input_length = lseek(input_fd, 0, SEEK_END);
  lseek(input_fd, 0, SEEK_SET);

  phy_addr = strtoul(argv[2], NULL, 0);

  file_base = mmap(NULL, input_length, PROT_READ, MAP_PRIVATE, input_fd, 0);
  if (file_base == MAP_FAILED) {
    perror("mmap(input)");
    rv = errno;
    goto out_close_input;
  }

  mem_fd = open("/dev/mem", O_RDWR | O_SYNC);
  if (mem_fd < 0) {
    perror("open(\"/dev/mem\")");
    rv = errno;
    goto out_unmap_file;
  }

  mem_base = mmap(NULL, input_length, PROT_READ | PROT_WRITE, MAP_SHARED,
                  mem_fd, phy_addr & pfn_mask);
  if (mem_base == MAP_FAILED) {
    perror("mmap(\"/dev/mem\")");
    rv = errno;
    goto out_close_mem;
  }

  memcpy(mem_base, file_base, input_length);

  munmap(mem_base, input_length);
out_close_mem:
  close(mem_fd);
out_unmap_file:
  munmap(file_base, input_length);
out_close_input:
  close(input_fd);

  return rv;
}
