#' @import BBmisc
#' @import checkmate
#' @useDynLib bbcompr c_bbcompr_init
NULL

.onAttach = function(libname, pkgname) {
  # .Call(c_bbcompr_init)
}

