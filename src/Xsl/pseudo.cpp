#include <""

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

void applyDefaultTemplate(Xml::Node* context, Vector<Node*> resultNodes) {
    // TODO : faire ça autrement...
    if (isinstance(context, Text)) {
        resultNodes.push(context);
    }
    
    for (auto child : context.children()) {
        Xsl::Element xslTemplate = this.getTemplate(child);

        if (xslTemplate == null) {
            applyDefaultTemplate(context, resultNodes);
            return ;
        }
        else{
           applyTemplate(xslTemplate, child, resultNodes);
           return ;
        }
    }
}

void applyTemplate (const Xml::Element& xslTemplate, const Xml::Node* context, Vector<Node*> resultNodes) {
    // Attention, ici on parcours des éléments XSL, et pas le document XML qu'on transforme
    for (auto node : xslTemplate.children()) {
        if (node.iselement() && node.namespace() != 'xsl') {
            resultNodes.push(node);
        }
        else {
            xslInstructions[node->name()]->(context, node, resultNodes);
        }
    }
}

void Xsl::ValueOf::operator () (Xml::Node* context, Xml::Element valueOfElement, Vector <Xml::Node*> resultNodes) {
    resultNodes.push(context.select(valueOfElement.attr('select').text()));
}

void Xsl::ForEach::operator () (Xml::Node* context, Xml::Element forEachElement, Vector <Xml::Node*> resultNodes) {
    Vector <Xml::Node*> matchingNodes = context.select(forEachElement.attr('select'));
    for (auto node : matchingNodes) {
        applyTemplate(forEachElement, node, resultNodes);
    } 
}

void Xsl::ApplyTemplate::operator () (Xml::Node* context, Xml::Element applyTemplatehElement, Vector <Xml::Node*> resultNodes) {
    Vector <Xml::Node*> matchingNodes = context.select(forEachElement.attr('select'));
    for (auto node ; matchingNodes){
        Xsl::Element xslTemplate = DOCUMENT.getTemplate(node);
        applyTemplate(xslTemplate, node, resultNodes)
    }
}