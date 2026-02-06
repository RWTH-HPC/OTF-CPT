#include "debug.h"
#include "parse_flags.h"
#include "typedefs.h"
#include <execinfo.h>

using namespace __otfcpt;

std::atomic<uint32_t> current_verbosity{0};

void print_stack(CptStreamBuffer &stream) {
#ifdef USE_BACKWARD
  using namespace backward;
  StackTrace st;
  st.load_here(CALLSTACK_SIZE);
  st.skip_n_firsts(SKIP_FRAMES);
  Printer p;
  p.object = true;
  p.color_mode = ColorMode::always;
  p.address = true;

  std::stringstream stringbuffer;
  p.print(st, stringbuffer);
  stream << stringbuffer.str().c_str() << "\n";
#else
  int nptrs;
  void *buf[CALLSTACK_SIZE + 1];
  nptrs = backtrace(buf, CALLSTACK_SIZE);
  char **symbols = backtrace_symbols(buf, nptrs);
  if (!symbols) {
    stream << "Stack trace failed\n";
    return;
  }
  for (int i = 0; i < nptrs; i++) {
    stream << symbols[i] << "\n";
  }
  free(symbols);
#endif
}

void print_stack() {
  char buffer[DBG_BUFFER_SIZE];
  CptStreamBuffer stream(buffer, DBG_BUFFER_SIZE);
  FILE *out =
      (get_otfcpt_flags()->output ? get_otfcpt_flags()->output : stderr);
  print_stack(stream);
  stream.fflush(out);
}

void NORETURN Die() {
  if (get_otfcpt_flags()->abort_on_error)
    abort();
  exit(get_otfcpt_flags()->exitcode);
}

void CheckFailed(const char *file, int line, const char *cond, u64 v1, u64 v2,
                 std::initializer_list<const char *> msgs) {
  char buffer[DBG_BUFFER_SIZE];
  CptStreamBuffer stream(buffer, DBG_BUFFER_SIZE);
  FILE *out =
      (get_otfcpt_flags()->output ? get_otfcpt_flags()->output : stderr);

  stream << "\nCheck failed in " << file << ":" << line << " "
         << (unsigned long long)v1 << " " << cond << " "
         << (unsigned long long)v2 << "\n";

  for (auto &m : msgs) {
    stream << m;
  }

  print_stack(stream);
  stream.fflush(out);

  if (!get_otfcpt_flags()->continue_on_error) {
    Die();
  }
}
