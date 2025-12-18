#include "debug.h"
#include "parse_flags.h"
#include "typedefs.h"
#include <execinfo.h>

using namespace __otfcpt;

std::atomic<uint32_t> current_verbosity{0};

void print_stack(void) {
  FILE *out =
      (get_otfcpt_flags()->output ? get_otfcpt_flags()->output : stderr);
#ifdef USE_STL
  std::stringstream buffer;
  print_stack(buffer);
  fprintf(out, "%s", buffer.str().c_str());
  fflush(out);
  return;
#endif
  int nptrs;
  void *buf[CALLSTACK_SIZE + 1];
  nptrs = backtrace(buf, CALLSTACK_SIZE);
  backtrace_symbols_fd(buf, nptrs, fileno(out));
}

#ifdef USE_STL
void print_stack(std::ostream &os) {
#ifdef USE_BACKWARD
  pretty_print_stack(os);
  return;
#endif
  int nptrs;
  void *buf[CALLSTACK_SIZE + 1];
  nptrs = backtrace(buf, CALLSTACK_SIZE);
  char **symbols = backtrace_symbols(buf, nptrs);
  if (!symbols) {
    os << "Stack trace failed\n";
    return;
  }
  for (int i = 0; i < nptrs; i++) {
    os << " " << symbols[i] << "\n";
  }
  free(symbols);
}

#ifdef USE_BACKWARD
void pretty_print_stack(std::ostream &os) {
  using namespace backward;
  StackTrace st;
  st.load_here(CALLSTACK_SIZE_PRETTY);
  st.skip_n_firsts(SKIP_FRAMES);
  Printer p;
  p.object = true;
  p.color_mode = ColorMode::always;
  p.address = true;
  p.print(st, os);
}
#endif
#endif

void NORETURN Die() {
  if (get_otfcpt_flags()->abort_on_error)
    abort();
  exit(get_otfcpt_flags()->exitcode);
}

#ifdef USE_STL
void CheckFailed(const char *file, int line, const char *cond, u64 v1, u64 v2,
                 std::initializer_list<const char *> msgs) {

  std::stringstream buffer;
  buffer << "\nCheck failed in " << file << ":" << line << " " << v1 << " "
         << cond << " " << v2 << "\n";

  for (auto &m : msgs) {
    buffer << m;
  }

  if (get_otfcpt_flags()->stacktrace) {
    print_stack(buffer);
  }

  FILE *out =
      (get_otfcpt_flags()->output ? get_otfcpt_flags()->output : stderr);
  std::string full_msg = buffer.str();

  fprintf(out, "%s", full_msg.c_str());
  fflush(out);
}
#else
void CheckFailed(const char *file, int line, const char *cond, u64 v1, u64 v2,
                 std::initializer_list<const char *> msgs) {
  char buffer[FB_BUFFER_SIZE];
  int offset = 0;

  offset += snprintf(buffer, FB_BUFFER_SIZE - offset,
                     "\nCheck failed in %s:%d %llu %s %llu\n", file, line, v1,
                     cond, v2);

  for (auto &m : msgs) {
    offset += snprintf(buffer, FB_BUFFER_SIZE, "%s", m);
  }

  FILE *out =
      (get_otfcpt_flags()->output ? get_otfcpt_flags()->output : stderr);

  fwrite(buffer, sizeof(char), offset, out);

  if (get_otfcpt_flags()->stacktrace) {
    print_stack();
  }

  if (!get_otfcpt_flags()->continue_on_error) {
    Die();
  }
}
#endif