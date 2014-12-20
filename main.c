#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

int main(int argc, char** argv) {
  if (argc != 3) {
    fprintf(stderr, "Wrong number of arguments\n");
    return 1;
  }

  FILE* input;
  char buffer[256];

  input = fopen(argv[1], "r");
  fgets(buffer, sizeof buffer, input);
  while (true) {
    int first;
    if (fscanf(input, "%d%*c ", &first) == EOF) { break; };
    if (fgets(buffer, sizeof buffer, input) == EOF) { break; };

    printf("%d, ", first); // TODO : remove this line

    char* ptr = strtok(buffer, ", \n");
    while(ptr) {
      int number = atoi(ptr);
      printf("%d, ", number); // TODO : remove this line
      ptr = strtok(NULL, ", \n");
    }
    putchar('\n');
  }
  return 0;
}
