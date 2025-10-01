//
// Created by ans on 02.10.25.
//

#include "../include/docx2txt.hpp"

#include "Document.hpp"

namespace docx2txt {

    std::string DOCX2TXT_API doc2txt(const std::string& path) {
        return Document(path).ToText();
    }

}
