#pragma once

namespace __otfcpt {
/** Small struct that carries a width value – the analogue of std::setw. */
struct setw_t {
  int width;
};
inline setw_t setw(int w) { return setw_t{w}; }

/** Tag type for the “left‑adjust” manipulator – the analogue of std::left. */
struct left_t {};
inline constexpr left_t left{}; // constexpr so it can be used as a literal
} // namespace __otfcpt