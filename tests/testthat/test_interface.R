context("interface")

test_that("interface", {
  login("demoaccount", "demopassword")
  n = numberOfProblems()
  expect_is(n, "integer")
  expect_that(n > 1L)
})
