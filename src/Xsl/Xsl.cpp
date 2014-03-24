//penser à garder la racine qqpart
//demander si besoin de regex


std::map<std::string, Xsl::Instruction*> xslInstructions;

Xml::Document xslTransform(Xml::Document& xmlDoc, Xml::Document& xslDoc ) {
    Xml::Document result = new Xml::Document();
    Vector <Xml::Node*> resultNodes;
    applyDefaultTemplate(xmlDoc.root(), resultNodes);
    if (resultNodes.size() == 1) {
        result.setRoot(resultNodes[0]);
        return result;
    }
    else {
        throw Exception();
    }

}

Vector <Xml::Node*> applyDefaultTemplate(Xml::Node* context, Vector<Node*> resultNodes) {
    // TODO : faire ça autrement...
    if (isinstance(context, Text)) {
        return [context];
    }

    for (auto child : context.children()) {
        Xsl::Element xslTemplate = this.getTemplate(child);
        applyTemplate(xslTemplate, child, resultNodes);
    }
    return resultNodes;
}

Vector <Xml::Node*> applyTemplate (const Xml::Element& xslTemplate, const Xml::Node* context, Vector<Node*> resultNodes) {
    if (xslTemplate == null) {
        return applyDefaultTemplate(context, resultNodes);
    }

    // Attention, ici on parcours des éléments XSL, et pas le document XML qu'on transforme
    for (auto node : xslTemplate.children()) {
        if (node.iselement() && node.namespace() != "xsl") {
            resultNodes.push(node);
        }
        else {
            xslInstructions[node->name()]->(context, node, resultNodes);
        }
    }
}

void Xsl::ValueOf::operator () (Xml::Node* context, Xml::Element xslElement, Vector <Xml::Node*> resultNodes) {
    resultNodes.push(context.select(xslElement.attr("select").text()));
}