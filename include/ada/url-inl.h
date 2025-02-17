/**
 * @file url-inl.h
 * @brief Definitions for the URL
 */
#ifndef ADA_URL_INL_H
#define ADA_URL_INL_H

#include "ada/checkers.h"
#include "ada/url.h"

namespace ada {
  [[nodiscard]] ada_really_inline bool url::includes_credentials() const noexcept {
    return !username.empty() || !password.empty();
  }
  [[nodiscard]] ada_really_inline bool url::is_special() const noexcept {
    return type != ada::scheme::NOT_SPECIAL;
  }
  [[nodiscard]] inline uint16_t url::get_special_port() const {
    return ada::scheme::get_special_port(type);
  }
  [[nodiscard]] ada_really_inline ada::scheme::type url::get_scheme_type() const noexcept {
    return type;
  }
  [[nodiscard]] ada_really_inline uint16_t url::scheme_default_port() const noexcept {
    return scheme::get_special_port(type);
  }
  [[nodiscard]] inline bool url::cannot_have_credentials_or_port() const {
    return !host.has_value() || host.value().empty() || type == ada::scheme::type::FILE;
  }
  ada_really_inline size_t url::parse_port(std::string_view view, bool check_trailing_content) noexcept {
    ada_log("parse_port('", view, "') ", view.size());
    uint16_t parsed_port{};
    auto r = std::from_chars(view.data(), view.data() + view.size(), parsed_port);
    if(r.ec == std::errc::result_out_of_range) {
      ada_log("parse_port: std::errc::result_out_of_range");
      is_valid = false;
      return 0;
    }
    ada_log("parse_port: ", parsed_port);
    const size_t consumed = size_t(r.ptr - view.data());
    ada_log("parse_port: consumed ", consumed);
    if(check_trailing_content) {
      is_valid &= (consumed == view.size() || view[consumed] == '/' || view[consumed] == '?' || (is_special() && view[consumed] == '\\'));
    }
    ada_log("parse_port: is_valid = ", is_valid);
    if(is_valid) {
      port = (r.ec == std::errc() && scheme_default_port() != parsed_port) ?
        std::optional<uint16_t>(parsed_port) : std::nullopt;
    }
    return consumed;
  }
  [[nodiscard]] inline std::string_view url::get_scheme() const noexcept {
    if(is_special()) { return ada::scheme::details::is_special_list[type]; }
    // We only move the 'scheme' if it is non-special.
    return non_special_scheme;
  }
  inline void url::set_scheme(std::string&& new_scheme) noexcept {
    type = ada::scheme::get_scheme_type(new_scheme);
    // We only move the 'scheme' if it is non-special.
    if(!is_special()) {
      non_special_scheme = new_scheme;
    }
  }
  inline void url::copy_scheme(ada::url&& u) noexcept {
    non_special_scheme = u.non_special_scheme;
    type = u.type;
  }
  inline void url::copy_scheme(const ada::url& u) {
    non_special_scheme = u.non_special_scheme;
    type = u.type;
  }

  inline std::ostream& operator<<(std::ostream& out, const ada::url& u) {
    return out << u.to_string();
  }
} // namespace ada

#endif // ADA_URL_H
