//
// Created by ans on 02.10.25.
//

#pragma once

#include "Data.hpp"

#include "miniz.h"

#include <format>
#include <stdexcept>
#include <string>

namespace docx2txt {

    class Reader {
    public:
        Reader(const std::string& path);
        ~Reader();

        [[nodiscard]] std::string Extract(const std::string& pathInZip);
        [[nodiscard]] std::string GetPath() const;

    private:
        bool loaded = false;
        std::string file;
        mz_zip_archive zip{};

        [[nodiscard]] int LocateFile(const std::string& pathInZip);
    };

} // namespace docx2txt
