//
// Created by ans on 01.10.25.
//

#include "Document.hpp"

namespace docx2txt {

    Document::Document(const std::string& path) : reader(path) {
        if(path.size() > 3 && path.substr(path.size() - 4, 4) == ".odt") {
            this->type = TYPE_ODT;

            return;
        }

        if(path.size() > 4 && path.substr(path.size() - 5, 5) == ".docx") {
            this->type = TYPE_DOCX;

            return;
        }

        std::cerr << "WARNING: Unknown extension of '" << path << "', treating it as *.docx.\n" << std::endl;
    }

    std::string Document::ToText() {
        std::string xml;

        if(this->type == TYPE_ODT) {
            xml = this->reader.Extract("content.xml");
        }
        else {
            xml = this->reader.Extract("word/document.xml");
        }

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

        if(this->type == TYPE_ODT) {
            return OdtXmlToText(doc, xml.size());
        }

        return DocxXmlToText(doc, xml.size());
    }

    std::string Document::OdtXmlToText(const pugi::xml_document& doc, std::size_t maxSize) {
        const auto paragraphs = doc.select_nodes("//text:p");
        std::string text;

        text.reserve(maxSize);

        for(const auto& paragraph: paragraphs) {
            AppendOdtText(paragraph.node(), text);

            text += '\n';
        }

        if(!text.empty()) {
            text.pop_back();
        }

        text.shrink_to_fit();

        return text;
    }

    std::string Document::DocxXmlToText(const pugi::xml_document& doc, std::size_t maxSize) {
        const auto paragraphs = doc.select_nodes("//w:p[not(ancestor::mc:Fallback)]");
        std::string text;

        text.reserve(maxSize);

        for(const auto& paragraph: paragraphs) {
            for(const auto& run: paragraph.node().children("w:r")) {
                const auto textNode = run.child("w:t");

                if(textNode) {
                    text += textNode.text().get();
                }
            }

            text += '\n';
        }

        if(!text.empty()) {
            text.pop_back();
        }

        text.shrink_to_fit();

        return text;
    }

    void Document::AppendOdtText(const pugi::xml_node& n, std::string& out) {
        switch(n.type()) {
            case pugi::node_pcdata:
            case pugi::node_cdata:
                out.append(n.value());

                return;

            case pugi::node_element: {
                std::string_view name = n.name();

                if(name == "text:s") {
                    // <text:s text:c="N"/> means N spaces (default 1)
                    const auto count = std::max(1, n.attribute("text:c").as_int(1));

                    out.append(static_cast<std::size_t>(count), ' ');

                    return;
                }

                if(name == "text:tab") {
                    out.push_back('\t');

                    return;
                }

                if(name == "text:line-break" || name == "text:soft-page-break") {
                    out.push_back('\n');

                    return;
                }

                if(name.rfind("draw:", 0) == 0) {
                    return; // avoid duplication of text box content, etc.
                }

                for(pugi::xml_node child : n.children()) {
                    AppendOdtText(child, out);
                }

                return;
            }

            default:
                return;
        }
    }

} // namespace docx2txt