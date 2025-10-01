//
// Created by ans on 01.10.25.
//

#pragma once

#include "Reader.hpp"

#include "pugixml.hpp"
#include "miniz.h"

#include <format>
#include <stdexcept>

namespace docx2txt {

    class Document {
    public:
        Document(const std::string& path);

        [[nodiscard]] std::string ToText();

    private:
        Reader reader;

        [[nodiscard]] static std::string XmlDocToText(const pugi::xml_document& doc, std::size_t maxSize);
    };

}
