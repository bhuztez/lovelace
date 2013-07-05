#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>


struct lambda {
  void (*func)();
  void *closure;
};


struct frame {
  struct frame *parent;
  struct lambda *lambda;
  int step;
};


enum opcode {
  INVALID,
  RETURN,
  CALL,
  TAILCALL
};


union operand {
  char c;
  int i;
  void *ptr;
};

struct frame *stack=NULL;

enum opcode next_opcode = INVALID;
union operand next_operand = {0};


struct lol_array_char {
  int length;
  char *data;
};

struct lol_frame_print_string {
  struct frame frame;
  struct lol_array_char *arg1;
};

struct lol_frame_main {
  struct frame frame;
};

void lol_print_string();
void lol_main();


struct lambda lol_closure_print_string = {
  .func = lol_print_string,
  .closure = NULL,
};

struct lambda lol_closure_main = {
  .func = lol_main,
  .closure = NULL,
};


void lol_print_string() {
  struct lol_frame_print_string *frame = (struct lol_frame_print_string *) stack;
  printf("%s\n", frame->arg1->data);

  next_opcode = RETURN;
  next_operand.i = 0;
}


void lol_main() {
  struct lol_frame_print_string *frame;
  switch (stack->step) {

  case 0:
    frame = malloc(sizeof(struct lol_frame_print_string));
    frame->frame.lambda = &lol_closure_print_string;

    frame->arg1 = malloc(sizeof(struct lol_array_char));
    frame->arg1->length = 14;
    frame->arg1->data = malloc(14);

    memcpy(frame->arg1->data, "Hello, world!", 14);

    next_opcode = CALL;
    next_operand.ptr = frame;
    break;
  case 1:
    next_opcode = RETURN;
    next_operand.i = 0;
    break;
  };
}


int main() {
  next_opcode = CALL;

  struct lol_frame_main *frame = malloc(sizeof(struct lol_frame_main));
  frame -> frame.parent = NULL;
  frame -> frame.lambda = &lol_closure_main;
  frame -> frame.step = 0;
  stack = (struct frame *)frame;

  while((stack) || (next_opcode != RETURN)) {
    stack->lambda->func();

    struct frame *top = stack;
    struct frame *next = (struct frame *)next_operand.ptr;

    switch(next_opcode) {
    case RETURN:
      stack = stack -> parent;
      free(top);
      break;
    case CALL:
      (stack->step)++;
      next -> parent = stack;
      stack = next;
      stack -> step = 0;
      break;
    case TAILCALL:
      stack = stack -> parent;
      free(top);
      next -> parent = stack;
      stack = next;
      stack -> step = 0;
      break;
    }
  }

  return 0;
}
