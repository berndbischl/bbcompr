% Generated by roxygen2 (4.0.2): do not edit by hand
\name{evalPoint}
\alias{evalPoint}
\title{Evaluate the objective function.}
\usage{
evalPoint(point)
}
\arguments{
\item{point}{[\code{numeric}]\cr
Point to evaluate. Length is expected to coincide with the problem dimension.}
}
\value{
[\code{numeric(1)}]. Objective value at \code{point}.
}
\description{
This call results in communication with the competition server.
This function fails if the predefined budget of function evaluations is used up.
It is assumed that all variables are within the unit hypercube, i.e., the box constraint
for all i:  0 ≤ point[i] ≤ 1, is respected.
Evaluation of an infeasible point, i.e., violation of the constraint results in an error;
the objective function is not evaluated.
}

