#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int  regs[4] = { 0 };
int  inst_pointer = 0, inst_count = 0;
enum reg       { REGA = 'a', REGB, REGC, REGD, VAL };
enum inst_type { CPY, INC, DEC, JNZ, INST_COUNT };
char* inst_names[] = { "cpy", "inc", "dec", "jnz" };
struct instruction {
  enum inst_type type;
  enum reg       regs[2];
  int            vals[2];
};

struct instruction instruction_list[50];

void copy(struct instruction inst) {
  if (inst.regs[0] == VAL) regs[inst.regs[1] - 'a'] = inst.vals[0];
  else regs[inst.regs[1] - 'a'] = regs[inst.regs[0] - 'a'];
  ++inst_pointer;
}

void increment(struct instruction inst) {
  ++regs[inst.regs[0] - 'a']; 
  ++inst_pointer;
}

void decrement(struct instruction inst) { 
  --regs[inst.regs[0] - 'a'];
  ++inst_pointer;
}

void jumpnotzero(struct instruction inst) { 
  if (inst.regs[0] == VAL && inst.vals[0] != 0) inst_pointer += inst.vals[1];
  else inst_pointer += (regs[inst.regs[0] - 'a'] ? inst.vals[1] : 1);
}

void (*inst_handler[])(struct instruction) = { copy, increment, decrement, jumpnotzero };

void run_program(int part) {
  enum inst_type type;
  if (part == 2) regs['c' - 'a'] = 1;
  while (inst_pointer >= 0 && inst_pointer < inst_count) {
    type = instruction_list[inst_pointer].type;
    inst_handler[type](instruction_list[inst_pointer]); 
  }
} 

void get_instructions(void) {
  int j = 0, k;
  enum inst_type i;
  char buf[10], regs[2][5];
  while (fgets(buf, 10, stdin)) {
    for (i = CPY; i < INST_COUNT; i++) {
      if (0 == strncmp(inst_names[i], buf, 3)) {
        instruction_list[j].type = i;
        switch (i) {
        case JNZ:
        case CPY:
          sscanf(buf, "%*s %s %s\n", regs[0], regs[1]);
          for (k = 0; k < 2; k++) {
            if (regs[k][0] >= REGA && regs[k][0] < VAL) {
              instruction_list[j].regs[k] = regs[k][0];
            } else {
              instruction_list[j].regs[k] = VAL;
              instruction_list[j].vals[k] = atoi(regs[k]);
            }
          }
          break; 
        case INC:
        case DEC:
          sscanf(buf, "%*s %c\n", (char*)&instruction_list[j].regs[0]);
          break;
        default:
          break;
        }
      } 
    }
    j++;
  }
  inst_count = j;
}

int main(int argc, char** argv) {
  int part = (argc > 1 ? (atoi(argv[1]) > 1 ? 2 : 1) : 1);
  get_instructions();
  run_program(part);
  printf("value of register a: %d\n", regs[0]);
  return 0;
}
