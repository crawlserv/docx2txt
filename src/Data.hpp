//
// Created by ans on 02.10.25.
//

#pragma once

#include "miniz.h"

#include <string>

namespace docx2txt {

    class Data {
    public:
        Data(void * zipData, std::size_t uncompressedSize);
        ~Data();

        [[nodiscard]] std::string ToString() const;

    private:
        void * data = nullptr;
        std::size_t size = 0;
    };

} // docx2txt