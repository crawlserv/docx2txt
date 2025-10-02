//
// Created by ans on 02.10.25.
//

#include "Reader.hpp"

namespace docx2txt {

    Reader::Reader(const std::string& path) : file(path) {
        if(!mz_zip_reader_init_file(&this->zip, path.c_str(), 0)) {
            throw std::runtime_error(
                std::format(
                    "'{}': {}",
                    path,
                    mz_zip_get_error_string(mz_zip_get_last_error(&this->zip))
                )
            );
        }

        this->loaded = true;
    }

    Reader::~Reader() {
        if(!this->loaded) {
            return;
        }

        mz_zip_reader_end(&this->zip);

        this->loaded = false;
    }

    std::string Reader::Extract(const std::string& pathInZip) {
        const auto location = this->LocateFile(pathInZip);
        std::size_t size = 0;
        void * data = mz_zip_reader_extract_to_heap(&this->zip, location, &size, 0);

        return Data(data, size).ToString();
    }

    std::string Reader::GetPath() const {
        return this->file;
    }

    int Reader::LocateFile(const std::string& pathInZip) {
        const auto location = mz_zip_reader_locate_file(&this->zip, pathInZip.c_str(), nullptr, 0);

        if(location == -1) {
            throw std::runtime_error(
                std::format(
                    "'{}/{}': {}",
                    this->file,
                    pathInZip,
                    mz_zip_get_error_string(mz_zip_get_last_error(&this->zip))
                )
            );
        }

        return location;
    }

}
