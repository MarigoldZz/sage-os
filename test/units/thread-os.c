#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <spinlock.h>

#define MAX_CPU 8

typedef union mytask {
  struct {
    const char *name;
    union mytask *next;
    void (*entry)(void *);
    Context *context;
  };
  uint8_t stack[8192];
} Task;

Task *currents[MAX_CPU];
#define current currents[cpu_current()]

// user-defined tasks

spinlock_t testLocked;
void lock() {
  spin_lock(&testLocked);
}
void unlock() {
  spin_unlock(&testLocked);
}

void func(void *arg) {
  while (1) {
    lock();
    printf("Thread-%s on CPU #%d\n", arg, cpu_current());
    unlock();
    for (int volatile i = 0; i < 100000; i++)
      ;
  }
}

Task tasks[] = {
    {.name = "A", .entry = func}, {.name = "B", .entry = func},
    {.name = "C", .entry = func}, {.name = "D", .entry = func},
    {.name = "E", .entry = func},
};

// ------------------

Context *on_interrupt(Event ev, Context *ctx) {
  extern Task tasks[];
  if (!current)
    current = &tasks[0];
  else
    current->context = ctx;
  do {
    current = current->next;
  } while ((current - tasks) % cpu_count() != cpu_current());
  return current->context;
}

void mp_entry() {
  iset(true);
  yield();
}

int main() {
  cte_init(on_interrupt);

  for (int i = 0; i < LENGTH(tasks); i++) {
    Task *task    = &tasks[i];
    Area stack    = (Area){&task->context + 1, task + 1};
    task->context = kcontext(stack, task->entry, (void *)task->name);
    task->next    = &tasks[(i + 1) % LENGTH(tasks)];
  }

  spin_init(&testLocked, "nametest");
  mpe_init(mp_entry);
}