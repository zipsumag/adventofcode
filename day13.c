#include <stdio.h>
#include <stdlib.h>


int main(int argc, char** argv) {
  int i, favnum = (argc > 1 ? atoi(argv[1]) : 1);
  printf("%d\n", favnum);
  return 0;
}

int get_n_high_bits(int x)
{
  int n_high = 0;
  for (n_high = 0; x; ++n_high)
    x &= x - 1;
  return n_high; 
}
