#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef unsigned int uint;

uint twitter[1024][1024] = {};
uint followings[1024] = {};
uint users = 0;

int compare(const void*, const void*);
void push(uint);
uint pop();
bool find(uint, uint);

uint counts_src[1024] = {};
uint counts_dst[1024] = {};
uint counts[1024] = {};


int main(int argc, char** argv) {
  // Check argc
  if (argc != 3) {
    fprintf(stderr, "Wrong number of arguments\n");
    return 1;
  }


  FILE* input;
  char buffer[256];

  // Parses input_file1, initialize twitter[][]
  input = fopen(argv[1], "r");
  fgets(buffer, sizeof buffer, input);
  while (true) {
    uint user;
    if (fscanf(input, "%u%*c ", &user) == EOF) { break; };
    if ((int)fgets(buffer, sizeof buffer, input) == EOF) { break; };

    char* ptr = strtok(buffer, ", \n");
    uint index = 0;
    while(ptr) {
      uint following = atoi(ptr);
      twitter[user][index++] = following;
      ptr = strtok(NULL, ", \n");
    }

    ++users;
  }

  // Initialize followings[]
  for (uint user = 1; user <= users; ++user) {
    uint following = 0;
    while (twitter[user][following]) { ++following; }
    followings[user] = following;
  }

  // Sort twitter[][]
  for (uint user = 1; user <= users; ++user) {
    qsort(twitter[user], followings[user], sizeof twitter[0][0], compare);
  }


  // A
  puts("A) Out-going degree: ");
  for (uint user = 1; user <= users; ++user) {
    printf("%u: %u\n", user, followings[user]);
  }
  putchar('\n');


  // B, Parses input_file2
  puts("B) Intermediate Node:");
  input = fopen(argv[2], "r");
  fgets(buffer, sizeof buffer, input);
  for (uint i = 0;; ++i) {
    uint src, dst;
    if (fscanf(input, "%u %*s %u", &src, &dst) == EOF) { break; };
    find(src, dst);

    uint count = 1;
    pop();

    printf("%u -> %u: ", src, dst);
    while (true) {
      uint hop = pop();
      if (!hop) { break; }

      printf("%u, ", hop);
      ++count;
    }
    printf("\b\b \b\n");

    counts_src[i] = src;
    counts_dst[i] = dst;
    counts[i] = count;
  }
  putchar('\n');

  // C
  puts("C) Hop count:");
  for (uint i = 0; counts[i]; ++i) {
    printf("%d -> %d: %d\n", counts_src[i], counts_dst[i], counts[i]);
  }
  putchar('\n');

  // D
  uint sum = 0;
  uint asdf = 0;
  for (uint src = 1; src <= users; ++src) {
    for (uint dst = 1; dst <= users; ++dst) {
      if (src == dst) { continue; }

      find(src, dst);

      uint count = 0;
      while (pop()) { ++count; }

      if (!count) { continue; }
      sum += count;
      ++asdf;
    }
  }
  printf("D) Average hop count:\n%g\n", (double)sum/(double)asdf);

  return 0;
}

int compare(const void* pleft, const void* pright) {
  uint left = *(uint*)pleft;
  uint right = *(uint*)pright;

  if (followings[right] != followings[left]) {
    return followings[right] - followings[left];
  } else {
    return right - left;
  }
}


uint _stack[1024] = {};
uint* _stack_top = _stack;

void push(uint input) { *(_stack_top++) = input; }
uint pop() {
  if (_stack == _stack_top) { return 0; }
  return *(--_stack_top);
}


bool find(uint src, uint dst) {
  if (src == dst) { return true; }

  for (uint i = 0; twitter[src][i]; ++i) {
    if (!find(twitter[src][i], dst)) { continue; }

    push(src);
    return true;
  }

  return false;
}
