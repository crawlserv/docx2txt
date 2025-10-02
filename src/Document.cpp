//
// Created by ans on 01.10.25.
//

#include "Document.hpp"

namespace docx2txt {

    Document::Document(const std::string& path) : reader(path) {}

    std::string Document::ToText() {
        auto xml = this->reader.Extract("word/document.xml");

        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_buffer_inplace(xml.data(), xml.size());

        if(!result) {
            throw std::runtime_error(
                std::format(
                    "{}@{}: {}",
                    this->reader.GetPath(),
                    result.offset,
                    result.description()
                )
            );
        }

        return XmlDocToText(doc, xml.size());
    }

    std::string Document::XmlDocToText(const pugi::xml_document& doc, std::size_t maxSize) {
        const auto paragraphs = doc.select_nodes("//w:p");
        std::string text;

        text.reserve(maxSize);

        for(const auto& paragraph : paragraphs) {
            for(const auto& run : paragraph.node().children("w:r")) {
                const auto textNode = run.child("w:t");

                if(textNode) {
                    text += textNode.text().get();
                }

                text += '\n';
            }
        }

        if(!text.empty()) {
            text.pop_back();
        }

        text.shrink_to_fit();

        return text;
    }

}