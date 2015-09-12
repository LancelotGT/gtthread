/*
  A program to demostrate the use of getcontext, swapcontext, makecontext, etc.
  Federer returns control to Nadal, who returns control back to Federer....
 */

#include <ucontext.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

static ucontext_t uctx_federer, uctx_nadal;

static void federer(void* p){
  double v = *(double*)p;

  puts("Federer serves!"); fflush(stdout);
  
  swapcontext(&uctx_federer, &uctx_nadal);

  while(rand() / (RAND_MAX+1.0) < v){
      puts("Federer returns!");

      swapcontext(&uctx_federer, &uctx_nadal);
  }
  puts("Nadal wins the point!");
}

static void nadal(void *p){
  double v = *(double*)p;
  
  while(rand() / (RAND_MAX+1.0) < v){
      puts("Nadal returns!");

      swapcontext(&uctx_nadal, &uctx_federer);
  }
  puts("Federer wins the point!");
}

int main(int argc, char *argv[]) {
  ucontext_t uctx_main;
  double p = 0.9;

  /*
    getcontext must be called on a uncontext object
    before makecontext can.
  */
  if (getcontext(&uctx_federer) == -1 || 
      getcontext(&uctx_nadal) == -1){
    perror("getcontext");
    exit(EXIT_FAILURE);
  }

  /*
    Allocating a new stacks 
   */
  uctx_federer.uc_stack.ss_sp = (char*) malloc(SIGSTKSZ);
  uctx_federer.uc_stack.ss_size = SIGSTKSZ;
  
  uctx_nadal.uc_stack.ss_sp = (char*) malloc(SIGSTKSZ);
  uctx_nadal.uc_stack.ss_size = SIGSTKSZ;
  
  /*
    Setting the successor context
  */
  uctx_federer.uc_link = &uctx_main;
  uctx_nadal.uc_link = &uctx_main;

  /*
    makecontext sets the starting routine for the context
   */
  makecontext(&uctx_federer, federer, 1, &p);  
  makecontext(&uctx_nadal, nadal, 1, &p);
  
  /*
    federer serves.  When we switch back to uctx_main,
    control will resume here.
  */
  if (swapcontext(&uctx_main, &uctx_federer) == -1){
    perror("swapcontext");
    exit(EXIT_FAILURE);
  }
  
  printf("main: exiting\n");
  exit(EXIT_SUCCESS);
}
