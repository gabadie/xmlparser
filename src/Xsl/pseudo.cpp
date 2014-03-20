Xsl::XslDocument::render(Xml::Document xmlDoc) {
    Xml::Document result = new Xml::Document();
    Xsl::Template rootTemplate = this.getTemplate('/');
    rootTemplate.apply(xmlDoc.root());
}

Xsl::DefaultTemplate::apply(Xml::Node context) {
    // TODO : faire ça autrement...
    if (isinstance(context, Text)) {
        return [context];
    }

    Vector<Xml::Node> nodes;
    foreach (Xml::Node child : context.mChildren) {
        Xsl::Template xslTemplate = this.getTemplate(child.getName());
        nodes += xslTemplate.apply(child);
    }

    return nodes;
}

Xsl::Template::apply(Xml::Node context) {
    Vector<Xml::Node> nodes;

    // Attention, ici on parcours des éléments XSL, et pas le document XML qu'on transforme
    foreach (Xml::Node node : this.mChildren) {
        if (node is not un élément XSL) {
            nodes.push(node);
        }
        else {
            nodes += node.apply(context);
        }
    }

}

