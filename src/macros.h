#ifndef FOO_MACROS_H
#define FOO_MACROS_H

#include <R.h>
#include <Rinternals.h>
#include <Rdefines.h>

#define UNPACK_REAL_VECTOR(S, D, N) \
    double *D = REAL(S); \
    const R_len_t N = length(S);

#define UNPACK_REAL_MATRIX(S, D, N, K) \
    double *D = REAL(S); \
    const R_len_t N = nrows(S); \
    const R_len_t K = ncols(S);
  
#define UNPACK_STRING(S, C) \
    const char* C = CHAR(STRING_ELT(S, 0));

#define RETURN_INT_RESULT(X) \
  SEXP s_res = PROTECT(allocVector(INTSXP, 1)); \
  INTEGER(s_res)[0] = X; \
  UNPROTECT(1); \
  return(s_res); 

#define RETURN_REAL_RESULT(X) \
  SEXP s_res = PROTECT(allocVector(REALSXP, 1)); \
  REAL(s_res)[0] = X; \
  UNPROTECT(1); \
  return(s_res); 

#define RETURN_STR_RESULT(X) \
  SEXP s_res = mkString(X); \
  return(s_res); 

#define HANDLE_ERROR_INT(X, ERROR) \
  if (X == ERROR) { \
    const char* msg = errorMessage(); \
    error("bbcompr error (%i): %s", X, msg); \
  }

#define HANDLE_ERROR_OBJ(X, ERROR) \
  if (X == ERROR) { \
    const char* msg = errorMessage(); \
    error("bbcompr error: %s", msg); \
  } 

#define MAKE_WRAPPER_GETTER_INT(LOW_LEV_CALL, WRAPPER_NAME, ERROR) \
  SEXP WRAPPER_NAME() { \
    int res = LOW_LEV_CALL(); \
    HANDLE_ERROR_INT(res, ERROR); \
    RETURN_INT_RESULT(res); \
  }

#define MAKE_WRAPPER_GETTER_STRING(LOW_LEV_CALL, WRAPPER_NAME, ERROR) \
  SEXP WRAPPER_NAME() { \
    const char* res = LOW_LEV_CALL(); \
    HANDLE_ERROR_OBJ(res, ERROR); \
    RETURN_STR_RESULT(res); \
  }

#define MAKE_WRAPPER_SETTER_INT(LOW_LEV_CALL, WRAPPER_NAME, ERROR) \
  void WRAPPER_NAME(SEXP s_arg) { \
    int arg = asInteger(s_arg); \
    int code = LOW_LEV_CALL(arg); \
    HANDLE_ERROR_INT(code, ERROR); \
  }
  
#define MAKE_WRAPPER_SETTER_STRING(LOW_LEV_CALL, WRAPPER_NAME, ERROR) \
  void WRAPPER_NAME(SEXP s_arg) { \
    UNPACK_STRING(s_arg, arg) \
    int code = LOW_LEV_CALL(arg); \
    HANDLE_ERROR_INT(code, ERROR); \
  }

#endif
