#include <common.h>
#include <trap.h>
#include <logger.h>
#include <buddy.h>

void* alloc_addr[16];

bool check_overlap(int num, intptr_t addr) {
  struct chunk* chunk0 = virt2chunk(&global_mm_pool, (void*)addr);
  for (int i = 0; i < num; i++) {
    struct chunk* chunk = virt2chunk(&global_mm_pool, alloc_addr[i]);
    if ((intptr_t)alloc_addr[i] >= addr) {
      if (addr + (1 << (chunk0->order + 12)) >= alloc_addr[i]) {
        return false;
      }
    } else {
      if (alloc_addr[i] + (1 << (chunk->order + 12))) {
        return false;
      }
    }
  }
  return true;
}
int main() {
  pmm->init();

  // test 4KB aligned
  for (int i = 0; i < 16; i++) {
    alloc_addr[i] = pmm->alloc(((i % 16) + 1) * 4096);
    check(alloc_addr[i] && (intptr_t)alloc_addr[i] % 4096 == 0);
    bool overlap = check_overlap(i, alloc_addr[i]);
    check(!overlap);
    success("%d allocation : 0x%x size: %dKB\n", i, alloc_addr[i],
            ((i % 16) + 1) * 4);
  }
  for (int i = 0; i < 16; i++) {
    pmm->free(alloc_addr[i]);
    success("free %d: 0x%x\n", i, alloc_addr[i]);
  }

  // test < 4KB space
  for (int i = 0; i < 16; i++) {
    alloc_addr[i] = pmm->alloc((i + 1) * 64);
    check(alloc_addr[i]);
    // bool overlap = check_overlap(i, alloc_addr[i]);
    // check(!overlap);
  }
  for (int i = 0; i < 16; i++) {
    pmm->free(alloc_addr[i]);
    success("free %d: 0x%x\n", i, alloc_addr[i]);
  }

  // test trivial allocation
  for (int i = 0; i < 16; i++) {
    alloc_addr[i] = pmm->alloc((i + 1) * 512);
    check(alloc_addr[i]);
    if ((i + 1) * 512 >= 4096) {
      check((intptr_t)alloc_addr[i] % 4096 == 0);
    }
  }
  for (int i = 0; i < 16; i++) {
    pmm->free(alloc_addr[i]);
    success("free %d: 0x%x\n", i, alloc_addr[i]);
  }
  return 0;
}