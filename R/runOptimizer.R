#' Runs an optimizer on a problem.
#'
#' @description
#' It is assumed that you are logged in.
#'
#' @param track [\code{character(1)}]\cr
#'   Selected track.
#'   Default is to use the last selected one.
#' @param problem [\code{integer(1)}]\cr
#'   Selected problem.
#'   Default is to use the last selected one.
#' @param optfun [\code{fun(dimension, budget, obj, start)}]\cr
#'   Wrapper to start the optimizer.
#' @return [\code{invisible(NULL)}].
#' @export
runOptimizer = function(track = NULL, problem = NULL, optfun) {
  if (!is.null(track))
    setTrack(track)
  if (!is.null(problem))
    setProblem(problem)
  assertFunction(optfun, c("dimension", "budget", "obj", "start"))

  dimension = getDimension()
  budget = getBudget()
  obj = function(x) {
    evalPoint(x)
  }
  start = runif(0, 1, dimension)
  optfun(dimension, budget, obj, start, lower, upper)
}


