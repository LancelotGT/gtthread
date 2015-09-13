/*
  A program demonstrating signal handlers and masks.  
  We block the signal that makes us test launch a missle
  during a nuclear crisis.
 */

#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include<string.h>
#include<sys/time.h>

static sigset_t vtalrm;

void alrm_safe_puts(char *s){
  sigprocmask(SIG_BLOCK, &vtalrm, NULL);
  puts(s);
  sigprocmask(SIG_UNBLOCK, &vtalrm, NULL);
}

void alrm_handler(int sig){
  puts("Missile Test");
}

int main(){
  struct itimerval *T;

  struct sigaction act;
  int i,j;
  
/*
  Setting up the signal mask
  */
  sigemptyset(&vtalrm);
  sigaddset(&vtalrm, SIGVTALRM);
  sigprocmask(SIG_UNBLOCK, &vtalrm, NULL);

  /* 
     Setting up the alarm
  */
  T = (struct itimerval*) malloc(sizeof(struct itimerval));
  T->it_value.tv_sec = T->it_interval.tv_sec = 0;
  T->it_value.tv_usec = T->it_interval.tv_usec = 1000;

  setitimer(ITIMER_VIRTUAL, T, NULL);

  
  /*
    Setting up the handler
  */
  memset (&act, '\0', sizeof(act));
  act.sa_handler = &alrm_handler;
  if (sigaction(SIGVTALRM, &act, NULL) < 0) {
    perror ("sigaction");
    return 1;
  }
 
  puts("Usually, missile tests are safe.");
  for(i = 0; i < 10; i++){
    for(j = 0; j < 1000000;j++);
    alrm_safe_puts(".");
  }
  
  /*
    blocking the alarm
  */
  sigprocmask(SIG_BLOCK, &vtalrm, NULL);
  puts("But when there is a nuclear crisis....(DEFCON 2!).... we halt them,");
  
  for(i = 0; i < 10; i++){
    for(j = 0; j < 1000000;j++);
    puts(".");
  }

  puts("until the crisis is past.");

  /*
    unblocking the alarm
  */
  sigprocmask(SIG_UNBLOCK, &vtalrm, NULL);

  for(i = 0; i < 10; i++){
    for(j = 0; j < 1000000;j++);
    alrm_safe_puts(".");
  }
}
