#ifndef __INIT__
#define __INIT__

#define INIT_FUNC(func, sec) \
  void func() __attribute__((naked)) __attribute((section(sec))); \
  void func()

#define INIT0(func) INIT_FUNC(func, ".init0")
#define INIT1(func) INIT_FUNC(func, ".init1")
#define INIT2(func) INIT_FUNC(func, ".init2")
#define INIT3(func) INIT_FUNC(func, ".init3")
#define INIT4(func) INIT_FUNC(func, ".init4")
#define INIT5(func) INIT_FUNC(func, ".init5")
#define INIT6(func) INIT_FUNC(func, ".init6")
#define INIT7(func) INIT_FUNC(func, ".init7")
#define INIT8(func) INIT_FUNC(func, ".init8")
#define INIT9(func) INIT_FUNC(func, ".init9")

#define INIT_USR0(func) INIT1(func)
#define INIT_USR1(func) INIT3(func)
#define INIT_USR2(func) INIT5(func)
#define INIT_USR3(func) INIT7(func)
#define INIT_USR4(func) INIT8(func)

#endif /* __INIT__ */
