#' @title Login on server.
#'
#' @description
#' This call results in communication with the competition server.
#' It validates the user's credentials. Multiple concurrent logins with the same credentials are
#' possible to allow for parallel evaluation of problems on different cores/machines,
#' but not from multiple threads within the same process.
#' The account becomes a persistent property of the library state; subsequent function calls refer
#' to this account.
#'
#' @param username [\code{character(1)}]\cr
#'   Username for login.
#' @param password [\code{character(1)}]\cr
#'   Password for login.
#' @return [\code{invisible(NULL)}].
#' @useDynLib bbcompr c_bbcompr_login
#' @export
login = function(username, password) {
  assertString(username)
  assertString(password)
  messagef("Logging in with account: %s", username)
  .Call(c_bbcompr_login, username, password)
  invisible(NULL)
}

#' @title Return number of tracks.
#'
#' @description
#' Return the number N of tracks available to the user.
#' Valid track indices range from 0 to N-1.
#' @return [\code{integer(1)}].
#' @useDynLib bbcompr c_bbcompr_numberOfTracks
#' @export
getNumberOfTracks = function() {
  .Call(c_bbcompr_numberOfTracks)
}

#' @title Return the name of the track.
#'
#' @description
#' Tracks are identified by their name, not by their index (which may change depending on user rights).
#'
#' @param index [\code{integer(1)}]\cr
#'   Index of selected track.
#' @return [\code{character(1)}].
#' @useDynLib bbcompr c_bbcompr_trackName
#' @export
getTrackName = function(index) {
  trackindex = asInt(index, lower = 0)
  .Call(c_bbcompr_trackName, index)
}

#' @title Set a track.
#'
#' @description
#' Select a track. Subsequent function calls refer to this track.
#'
#' @param track [\code{character(1)}]\cr
#'   Name of selected track.
#' @return [\code{invisible(NULL)}].
#' @useDynLib bbcompr c_bbcompr_setTrack
#' @export
setTrack = function(track) {
  assertString(track)
  messagef("Setting track: %s", track)
  .Call(c_bbcompr_setTrack, track)
  invisible(NULL)
}

#' @title Return number of problems in selected track.
#'
#' @description
#' Return the number of problems M in the current track. Valid problem IDs range from 0 to M-1.
#' @return [\code{integer(1)}].
#' @useDynLib bbcompr c_bbcompr_numberOfProblems
#' @export
getNumberOfProblems = function() {
  .Call(c_bbcompr_numberOfProblems)
}

#' @title Select a problem within the chosen track for optimization.
#'
#' Subsequent function calls refer to this problem.
#'
#' @param id [\code{integer(1)}]\cr
#'   ID of selected problem.
#' @return [\code{invisible(NULL)}].
#' @useDynLib bbcompr c_bbcompr_setProblem
#' @export
setProblem = function(id) {
  id = asInt(id, lower = 0L)
  messagef("Setting problem: %i", id)
  .Call(c_bbcompr_setProblem, id)
  invisible(NULL)
}


#' @title Return the dimension of the search space of the current problem.
#' @return [\code{integer(1)}].
#' @useDynLib bbcompr c_bbcompr_dimension
#' @export
getDimension = function() {
  .Call(c_bbcompr_dimension)
}

#' @title Return the number of evaluations so far for the current problem.
#' @return [\code{integer(1)}].
#' @useDynLib bbcompr c_bbcompr_evaluations
#' @export
getEvaluations = function() {
  .Call(c_bbcompr_evaluations)
}

#' @title Return the evaluation budget for the current problem.
#'
#' @description
#' This is the total available budget, not the remaining budget (which can be obtained by
#' subtracting the result of the \code{\link{getEvaluations}} function).
#' Calling \code{\link{evalPoint}} more than this number of times on the current problem will fail.
#' @return [\code{integer(1)}].
#' @useDynLib bbcompr c_bbcompr_budget
#' @export
getBudget = function() {
  .Call(c_bbcompr_budget)
}

#' @title Evaluate the objective function.
#'
#' @description
#' This call results in communication with the competition server.
#' This function fails if the predefined budget of function evaluations is used up.
#' It is assumed that all variables are within the unit hypercube, i.e., the box constraint
#' for all i:  0 ≤ point[i] ≤ 1, is respected.
#' Evaluation of an infeasible point, i.e., violation of the constraint results in an error;
#' the objective function is not evaluated.
#' @param point [\code{numeric}]\cr
#'   Point to evaluate. Length is expected to coincide with the problem dimension.
#' @return [\code{numeric(1)}]. Objective value at \code{point}.
#' @useDynLib bbcompr c_bbcompr_evaluate
#' @export
evalPoint = function(point) {
  assertNumeric(point, any.missing = FALSE, lower = 0, upper = 1)
  .Call(c_bbcompr_evaluate, point)
}


#FIXME: this does not work if we refer to different problem in indices.
# then points can have different dimensions

#' @title Evaluate the objective function.
#'
#' @description
#' Recover a previous evaluation from the log file.
#' If logging was disabled through \code{\link{configureBBComp}} or if the log file is removed
#' or corrupted then this function will fail.
#'
#' @param indices [\code{integer(n)}]\cr
#'   Time indices of the evaluations. If T queries to evaluate were already made then these
#'   can be any number in the range 0 to T-1.
#'   NOTE THAT THEREFORE AN INDEX IS ZERO-BASED UNLIKE USUALLY IN R!
#' @return [\code{matrix(n, d+1)}]. Matrix of evaluations. Each row is one point plus its
#'   objective value in the last column.
#' @useDynLib bbcompr c_bbcompr_history
#' @export
getEvalHistory = function(indices) {
  indices = asInteger(indices, any.missing = FALSE, lower = 0)
  res = matrix(NA_real_, length(indices), )
  .Call(c_bbcompr_history, indices, res)
}
