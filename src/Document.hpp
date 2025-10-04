//
// Created by ans on 01.10.25.
//

#pragma once

#include "Reader.hpp"
#include "Type.hpp"

#include "pugixml.hpp"
#include "miniz.h"

#include <format>
#include <iostream>
#include <stdexcept>
#include <string_view>

namespace docx2txt {

    class Document {
    public:
        Document(const std::string& path);

        [[nodiscard]] std::string ToText();

    private:
        Reader reader;
        Type type = TYPE_UNKNOWN;

        [[nodiscard]] static std::string OdtXmlToText(const pugi::xml_document& doc, std::size_t maxSize);
        [[nodiscard]] static std::string DocxXmlToText(const pugi::xml_document& doc, std::size_t maxSize);
        static void AppendOdtText(const pugi::xml_node& n, std::string& out);
    };

} // namespace docx2txt
