#include "debug.h"
#include "parse_flags.h"
#include "typedefs.h"
#include <execinfo.h>

using namespace __otfcpt;

std::atomic<uint32_t> current_verbosity{0};

void print_stack(void) {
#ifdef USE_BACKWARD
  pretty_print_stack();
  return;
#endif
  int nptrs;
  void *buf[CALLSTACK_SIZE + 1];
  nptrs = backtrace(buf, CALLSTACK_SIZE);
  backtrace_symbols_fd(buf, nptrs, STDOUT_FILENO);
}

void pretty_print_stack() {
#ifdef USE_BACKWARD
  using namespace backward;
  StackTrace st;
  st.load_here(CALLSTACK_SIZE_PRETTY);
  st.skip_n_firsts(SKIP_FRAMES);
  Printer p;
  p.object = true;
  p.color_mode = ColorMode::always;
  p.address = true;
  p.print(st,
          (get_otfcpt_flags()->output ? get_otfcpt_flags()->output : stderr));
#endif
}

void NORETURN Die() {
  if (get_otfcpt_flags()->abort_on_error)
    abort();
  exit(get_otfcpt_flags()->exitcode);
}

void CheckFailed(const char *file, int line, const char *cond, u64 v1, u64 v2) {
  fprintf((get_otfcpt_flags()->output ? get_otfcpt_flags()->output : stderr),
          "\nCheck failed in %s:%d %llu %s %llu\n\n", file, line, v1, cond, v2);
  if (get_otfcpt_flags()->stacktrace) {
    print_stack();
  }
  fflush((get_otfcpt_flags()->output ? get_otfcpt_flags()->output : stderr));
  if (!get_otfcpt_flags()->continue_on_error) {
    Die();
  }
}
