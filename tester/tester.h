#pragma once

struct test {
  int out_read_fd, out_write_fd, tmpSTDOUT;
  const char *example_out;

  test(const char *example_in, const char *example_out);
  ~test();
};