#include "tester.h"

#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <iostream>
#include <istream>
#include <locale>
#include <ostream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unistd.h>
#include <utility>

namespace {

void fail(const std::string &correct, const std::string &got) {
  std::cout << "INCORRECT\n=== Expected ===" << '\n';
  std::cout << correct << '\n';
  std::cout << "=== Got: =======" << '\n';
  std::cout << got << '\n';
  exit(1);
}

void trim(std::string &str) {
  auto whitespace = " \n\t";
  auto begin_whitespace = str.find_first_not_of(whitespace);
  auto end_whitespace = str.find_last_not_of(whitespace);
  str.erase(end_whitespace + 1);
  if (begin_whitespace)
    str.erase(0, begin_whitespace - 1);
}

void open_channel(int &read_fd, int &write_fd) {
  int vals[2];
  int errc = pipe(vals);
  if (errc) {
    read_fd = -1;
    write_fd = -1;
    throw std::runtime_error("Bad pipe");
  } else {
    read_fd = vals[0];
    write_fd = vals[1];
  }
}

void write_string(int write_fd, const std::string_view &message) {
  //   std::cerr << message; // Emulate actually writing to console
  ssize_t n_written = write(write_fd, message.data(), message.size());
  if (n_written != message.size()) {
    throw std::runtime_error("Error writing to stdin");
  }
  close(write_fd);
};

std::string read_string(int read_fd) {
  std::string ret;
  constexpr int buffer_size = 1000;
  char buffer[buffer_size + 1];
  ssize_t amnt_read;
  fflush(stdout);
  do {
    amnt_read = read(read_fd, &buffer[0], buffer_size);
    buffer[amnt_read] = 0;
    ret += buffer;
  } while (amnt_read == buffer_size);
  return ret;
};

}; // namespace

test::test(const char *example_in, const char *_out) : example_out(_out) {
  int in_read_fd;
  int in_write_fd;
  const std::string_view in(example_in);

  // STD_IN
  open_channel(in_read_fd, in_write_fd);
  dup2(in_read_fd, STDIN_FILENO);
  close(STDIN_FILENO);
  if (dup(in_read_fd) == -1)
    throw std::runtime_error("Failed to spoof standard input");

  write_string(in_write_fd, in.data());

  fflush(stdout);
  open_channel(out_read_fd, out_write_fd);
  tmpSTDOUT = dup(STDOUT_FILENO);
  fcntl(out_write_fd, F_SETPIPE_SZ, 1048576); // Set to max value
  dup2(out_write_fd, STDOUT_FILENO);
}

test::~test() {
  std::string out = read_string(out_read_fd);
  dup2(tmpSTDOUT, STDOUT_FILENO);
  trim(out);

  std::string example_str(example_out);

  trim(example_str);
  if (out != example_str) {
    fail(example_str, out);
  }
}
