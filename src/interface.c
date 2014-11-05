#include "macros.h"
#include "bbcomplib.h"
DLL_DECLARATIONS

void c_bbcompr_init() {
  DLL_LOAD
}

/* int configure(int history, const char* logfilepath); */
/* Set global configuration options. This call is optional, however, it can only occur before login. */
/* history: Setting this value to zero disables the history, any other value enables the history (the default). Enabling the history has the effect that each single function evaluation is logged to local files. This means that this information is available even after a program crash or power outage, possibly allowing an implementation to recover gracefully and continue the optimization run. The downside is that the files may grow as large as 30 GB, which may be an issue in some restricted environments. Also, the history information may not be of value to all optimizers. This is why it can be turned off. */
/* logfilepath: This path is prepended to file names of log files. This gives the user control over where to store these files. The default value is ".", referring to the current directory. */
/* The function returns 0 on error. */
void c_bbcompr_configure(SEXP s_history, SEXP s_logfilepath) {
  int history = asInteger(s_history);
  UNPACK_STRING(s_logfilepath, logfilepath)
  int code = configure(history, logfilepath);
  HANDLE_ERROR_INT(code, 0);
}


/* int login(const char* username, const char* password); */
/* Login to the system. This call results in communication with the competition server. */
/* It validates the user's credentials. Multiple concurrent logins with the same */
/* credentials are possible to allow for parallel evaluation of problems on different cores/machines, */
/* but not from multiple threads within the same process. */
/* The account becomes a persistent property of the library state; */
/* subsequent function calls refer to this account. */
/* The function returns 0 on error. */
void c_bbcompr_login(SEXP s_username, SEXP s_password) {
  UNPACK_STRING(s_username, username)
  UNPACK_STRING(s_password, password)
  int code = login(username, password);
  HANDLE_ERROR_INT(code, 0);
}

/* int numberOfTracks(); */
/* Return the number N of tracks available to the user. */
/* Valid track indices range from 0 to N-1. */
/* The function returns 0 on error. */
MAKE_WRAPPER_GETTER_INT(numberOfTracks, c_bbcompr_numberOfTracks, 0)

/* const char* trackName(int trackindex); */
/* Return the name of the track. Tracks are identified by their name, not by their index */
/* (which may change depending on user rights). */
/* The function returns a NULL pointer on error. */
SEXP c_bbcompr_trackName(SEXP s_trackindex) {
  int trackindex = asInteger(s_trackindex);
  const char* res = trackName(trackindex);
  HANDLE_ERROR_OBJ(res, 0);
  RETURN_STR_RESULT(res);
}

/* int setTrack(const char* trackname); */
/* Select a track. Subsequent function calls refer to this track. */
/* The function returns 0 on error. */
MAKE_WRAPPER_SETTER_STRING(setTrack, c_bbcompr_setTrack, 0)

/* int numberOfProblems(); */
/* Return the number of problems M in the current track. */
/* Valid problem IDs range from 0 to M-1. */
/* The function returns 0 on error. */
MAKE_WRAPPER_GETTER_INT(numberOfProblems, c_bbcompr_numberOfProblems, 0)

/* int setProblem(int problem_id); */
/* Select a problem within the chosen track for optimization. */
/* Subsequent function calls refer to this problem. */
/* The function returns 0 on error. */
MAKE_WRAPPER_SETTER_INT(setProblem, c_bbcompr_setProblem, 0)

  /* int dimension(); */
/* Return the dimension of the search space of the current problem. */
/* The function returns 0 on error. */
MAKE_WRAPPER_GETTER_INT(dimension, c_bbcompr_dimension, 0)

/* int budget(); */
/* Return the evaluation budget for the current problem. */
/* This is the total available budget, not the remaining budget */
/* (which can be obtained by subtracting the result of the evaluations function). */
/* Calling evaluate more than this number of times on the current problem will fail. */
/* The function returns 0 on error. */
MAKE_WRAPPER_GETTER_INT(budget, c_bbcompr_budget, 0)

/* int evaluations(); */
/* Return the number of evaluations so far for the current problem. */
/* The function returns -1 on error. */
MAKE_WRAPPER_GETTER_INT(evaluations, c_bbcompr_evaluations, -1)

/* int evaluate(double* point, double* value); */
/* Evaluate the objective function. */
/* point: Pointer to a linear memory array of double numbers. The size of the array is expected */
/*   to coincide with the problem dimension. */
/* value: Pointer to a double number. On success, the number is filled in with the value of the */
/*   objective function at the given point.  */
/* This call results in communication with the competition server. */
/* Note: This function fails if the predefined budget of function evaluations is used up. */
/* Note: It is assumed that all variables are within the unit hypercube, i.e., the box constraint */
/*   for all i:  0 ≤ point[i] ≤ 1 is respected. Evaluation of an infeasible point, i.e., */
/*   violation of the constraint results in an error; the objective function is not evaluated. */
/* The function returns 0 on error. */
SEXP c_bbcompr_evaluate(SEXP s_point) {
  double* point= REAL(s_point);
  double y = 0;
  int code = evaluate(point, &y);
  HANDLE_ERROR_INT(code, 0);
  RETURN_REAL_RESULT(y);
}


/* int history(int index, double* point, double* value); */
/* Recover a previous evaluation from the log file. Note: if logging was disabled through */
/* configure or if the log file is removed or corrupted then this function will fail. */
/* index: Time index of the evaluation. If T queries to evaluate were already made then the */
/*   index can be any number in the range 0 to T-1. */
/* point: Pointer to a linear memory array of double numbers. The size of the array is expected */
/*   to coincide with the problem dimension. Memory management is up to the caller, i.e., */
/*   the memory must be allocated before the call and freed afterwards. */
/* value: Pointer to a double number. The number is filled in with the value of the objective function. */
/*   Memory management is up to the caller, i.e., the memory must be allocated before the call and */
/*   freed afterwards. */
/* The function returns 0 on error. */
void c_bbcompr_history(SEXP s_indices, SEXP s_res) {
  int* indices = INTEGER(s_indices);
  int n = LENGTH(s_indices);
  UNPACK_REAL_MATRIX(s_res, res, res_nrow, res_ncol)

  double y = 0;
  int dim = res_ncol - 1;
  double point[dim];

  for (R_len_t i = 0; i < n; i++) {
    int index = indices[i];
    int code = history(index, point, &y);
    HANDLE_ERROR_INT(code, 0)
    for (R_len_t j = 0; j < dim; j++) {
      res[i + j*res_nrow] = point[j];
    }
    res[i + dim * res_nrow] = y;
  }
}


