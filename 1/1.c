#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAXLEN 1000

int is_visited(int places[][2], int len, int current[2]) { 
  int i;
  for (i = 0; i < len; i++)
    if (places[i][0] == current[0] && places[i][1] == current[1])
      return (abs(current[0]) + abs(current[1]));
  return 0; 
}


int main(int argc, char** argv) {
  int  i, steps, prevdir = 1, pos[2] = { 0 }, visited[MAXLEN][2];
  int  stepcount = 0, firstcross, templen;
  char dir, prevcomp = 1;
  while (scanf("%c%d, ", &dir, &steps) == 2) {
    prevdir = (prevcomp ? (prevdir > 0 ? -1 : 1) : (prevdir > 0 ? 1 : -1));
    if (dir == 'R') prevdir *= -1;
    for (i = 1; i <= steps; i++) {
      visited[stepcount][prevcomp]  = pos[prevcomp];
      visited[stepcount][!prevcomp] = pos[!prevcomp] + prevdir * i;
      templen = is_visited(visited, stepcount - 1, visited[stepcount]);
      if (!firstcross && templen) firstcross = templen;
      stepcount++;
    }
    pos[!prevcomp] += prevdir * steps;
    prevcomp        = !prevcomp;
  }
  printf("distance at the end                  : %d\n", abs(pos[0]) + abs(pos[1]));
  printf("distance of first place visited twice: %d\n", firstcross);
  return 0; 
}

