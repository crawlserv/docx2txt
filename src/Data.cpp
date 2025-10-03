//
// Created by ans on 02.10.25.
//

#include "Data.hpp"

namespace docx2txt {

    Data::Data(void * zipData, std::size_t uncompressedSize) : data(zipData), size(uncompressedSize) {}

    Data::~Data() {
        if(this->data == nullptr) {
            return;
        }

        mz_free(this->data);

        this->data = nullptr;
    }

    std::string Data::ToString() const {
        return std::string(static_cast<char *>(this->data), this->size);
    }

} // namespace docx2txt