#ifdef UBRR
# undef UBRR
#endif

#define UBRR ((10 * (F_CPU) / 16L / (BAUD) - 5) / 10)

#ifdef UBRR_2X
# undef UBRR_2X
#endif

#define UBRR_2X ((10 * (F_CPU) / 8L / (BAUD) - 5) / 10)

#if UBRR < 0
# undef UBRR
#endif

#if UBRR_2X < 0
# undef UBRR_2X
#endif

#ifdef UBRR
# ifdef UBRR_BAUD
#   undef UBRR_BAUD
# endif

# define UBRR_BAUD ((F_CPU) / (16L * ((UBRR) + 1)))

# ifdef UBRR_ERROR
#   undef UBRR_ERROR
# endif

# define _UBRR_ERROR (((UBRR_BAUD) * 1000L) / (BAUD) - 1000L)
# if _UBRR_ERROR < 0
#   define UBRR_ERROR -(_UBRR_ERROR)
# else
#   define UBRR_ERROR _UBRR_ERROR
# endif
#endif

#ifdef UBRR_2X
# ifdef UBRR_2X_BAUD
#   undef UBRR_2X_BAUD
# endif

# define UBRR_2X_BAUD ((F_CPU) / (8L * ((UBRR_2X) + 1)))

# ifdef UBRR_2X_ERROR
#   undef UBRR_2X_ERROR
# endif

# define _UBRR_2X_ERROR (((UBRR_2X_BAUD) * 1000L) / (BAUD) - 1000L)
# if _UBRR_2X_ERROR < 0
#   define UBRR_2X_ERROR -(_UBRR_2X_ERROR)
# else
#   define UBRR_2X_ERROR _UBRR_2X_ERROR
# endif
#endif

#ifdef USE_2X
# undef USE_2X
#endif

#if defined(UBRR_2X_ERROR) && defined(UBRR_ERROR) && UBRR_2X_ERROR < UBRR_ERROR
# define USE_2X 1
#else
# define USE_2X 0
#endif

#ifdef UBRR_VALUE
# undef UBRR_VALUE
#endif

#ifdef UBRR_VALID
# undef UBRR_VALID
#endif

#if USE_2X == 1
# ifdef UBRR_2X
#   define UBRR_VALUE UBRR_2X
#   define UBRR_VALID 1
# else
#   define UBRR_VALUE 0
#   define UBRR_VALID 0
# endif
#else
# ifdef UBRR
#   define UBRR_VALUE UBRR
#   define UBRR_VALID 1
# else
#   define UBRR_VALUE 0
#   define UBRR_VALID 0
# endif
#endif

#ifdef UBRRL_VALUE
# undef UBRRL_VALUE
#endif

#ifdef UBRRH_VALUE
# undef UBRRH_VALUE
#endif

#define UBRRL_VALUE ((UBRR_VALUE) & 0xff)
#define UBRRH_VALUE ((UBRR_VALUE) >> 8)
