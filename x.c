typedef long long unsigned int size_t;

size_t slen(char *s);

//====================================================

void call(size_t num, size_t a1, size_t a2, size_t a3, size_t a4, size_t a5, size_t a6){
  register size_t  x  asm("rax") = num;
  register size_t  x1 asm("rdi") = a1;
  register size_t  x2 asm("rsi") = a2;
  register size_t  x3  asm("rdx") = a3;
  register size_t  x4  asm("r10") = a4;
  register size_t  x5  asm("r8") = a5;
  register size_t  x6  asm("r9") = a6;
  asm("syscall");
}

#define CALL(X) call(X,0,0,0,0,0,0)
#define CALL1(X,a1) call(X,a1,0,0,0,0,0)
#define CALL2(X,a1,a2) call(X,a1,a2,0,0,0,0)
#define CALL3(X,a1,a2,a3) call(X,a1,a2,a3,0,0,0)
#define CALL4(X,a1,a2,a3,a4) call(X,a1,a2,a3,a4,0,0)
#define CALL5(X,a1,a2,a3,a4,a5) call(X,a1,a2,a3,a4,a5,0)
#define CALL6(X,a1,a2,a3,a4,a5,a6) call(X,a1,a2,a3,a4,a5,a6)

enum syscall{READ = 0, WRITE = 1, EXIT = 60}; 

//====================================================

int leave(int n){ CALL1(EXIT, n); }

void print(char *s){ CALL3(WRITE, 1, (size_t)s, slen(s)); }

//====================================================

void println(char *s){ print(s); print("\n"); }

size_t slen(char *s){
	int i;for(i=0;s[i];i++);return i;
}

//===================================================

void mymain(void) {
  char *s = "COUCOU";
  println(s);
  register size_t x asm("r12") = 0x11111111;
  asm("jmp *%r12");
  leave(0);
}

