#include <iostream>

#include "dataflow.hpp"

int main(void)
{

    std::cout << "---hello dataflow------" << std::endl;

    GraphManager gm;

    Node *a = gm.createNode(NODE_TYPE::NODE_OF_DOUBLE_OUT);
    Node *b = gm.createNode(NODE_TYPE::NODE_OF_DOUBLE_OUT);
    Node *c = gm.createNode(NODE_TYPE::NODE_OF_SUM);

    gm.connectPorts(a->getPortOutByPos(0), c->getInputPortByPos(0));
    gm.connectPorts(b->getPortOutByPos(0), c->getInputPortByPos(1));

    c->evaluate();

    // a = gm.createNode(NODE_TYPE::NODE_OF_SUM);

    // gm.connectPorts(c->getPortOutByPos(0), a->getInputPortByPos(0));
    // gm.connectPorts(b->getPortOutByPos(0), a->getInputPortByPos(1));

    // a->evaluate();
    // //--------- STRINGS-------------

    // a = gm.createNode(NODE_TYPE::NODE_OF_STRING_OUT);
    // b = gm.createNode(NODE_TYPE::NODE_OF_STRING_OUT);
    // c = gm.createNode(NODE_TYPE::NODE_OF_STRING_CONCAT);

    // gm.connectPorts(a->getPortOutByPos(0), c->getInputPortByPos(0));
    // gm.connectPorts(b->getPortOutByPos(0), c->getInputPortByPos(1));

    // c->evaluate();

    return 0;
}