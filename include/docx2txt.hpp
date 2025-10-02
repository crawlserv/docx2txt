#pragma once

#include <string>

#if (defined(_WIN32) || defined(__CYGWIN__)) && defined(DOCX2TXT_SHARED)
  #if defined(DOCX2TXT_BUILDING_LIBRARY)
    #define DOCX2TXT_API __declspec(dllexport)
  #else
    #define DOCX2TXT_API __declspec(dllimport)
  #endif
#else
  #define DOCX2TXT_API
#endif

namespace docx2txt {
  [[nodiscard]] std::string docx2txt(const std::string& path);
}