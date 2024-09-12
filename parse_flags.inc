#ifndef PARSE_FLAG
#error "Define PARSE_FLAG prior to including this file!"
#endif

// PARSE_FLAG(Type, Name, DefaultValue, Description)
// Supported types: bool, const char *, int, uptr.
// Default value must be a compile-time constant.
// Description must be a string literal.

PARSE_FLAG(bool, print_max_rss, false,
           "Print memory usage information at the end of execution.")

PARSE_FLAG(bool, verbose, false, "Provide verbose output.")

PARSE_FLAG(bool, enabled, true,
           "Inject OpenMP synchronization semantics to TSan.")

PARSE_FLAG(bool, report_data_leak, false, "Report tool internal data leaks.")

PARSE_FLAG(bool, stopped, false, "Delay start of measurement.")

PARSE_FLAG(bool, colorize, true, "Colorize output.")

PARSE_FLAG(bool, help, false, "Print the flag descriptions.")

PARSE_FLAG(bool, abort_on_error, false, "Abort instead of exit on error.")

PARSE_FLAG(int, exitcode, 23, "Exit code for errors.")

PARSE_FLAG(const char *, log_path, "stdout",
           "Write logs to \"log_path.pid\". The special values are \"stdout\" "
           "and \"stderr\".")

PARSE_FLAG(const char *, data_path, "stdout",
           "Write metric data to \"<data_path>-<#procs>x<#threads>.txt\". "
           "The special values are \"stdout\" and \"stderr\".")
