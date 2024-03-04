#include <iostream>
#include <memory>
#include "dataflow.hpp"

 
void test2()
{
    std::cout << "TEST02" << std::endl;

    GraphManager gm;

    Node *a = gm.createNode(NODE_TYPE::NODE_OF_DOUBLE_OUT);
    Node *b = gm.createNode(NODE_TYPE::NODE_OF_DOUBLE_OUT);
    Node *c = gm.createNode(NODE_TYPE::NODE_OF_SUM);

    gm.connect(a, gm.findPortByNameAndNode("out", a), c, gm.findPortByNameAndNode("in_a", c));
    gm.connect(b, gm.findPortByNameAndNode("out", b), c, gm.findPortByNameAndNode("in_b", c));
    //gm.printPortNodeIds();
    c->evaluate();
  
   

    // Node *d = gm.createNode(NODE_TYPE::NODE_OF_SUM);

    // Node *e = gm.createNode(NODE_TYPE::NODE_OF_DOUBLE_OUT);
    // // gm.connectPorts(c->getPortOutByPos(0), a->getInputPortByPos(0));
    // // gm.connectPorts(b->getPortOutByPos(0), a->getInputPortByPos(1));

    // gm.connect(c, gm.findPortByNameAndNode("out", c), d, gm.findPortByNameAndNode("in_a", d));
    // gm.connect(e, gm.findPortByNameAndNode("out", e), d, gm.findPortByNameAndNode("in_b", d));

    // a->evaluate();

    double *result = static_cast<double *>(gm.getPropById(gm.findPortByNameAndNode("out", c)->prop_id)->data);
     std::cout << " res:" << result<< std::endl;
    if (*result == 30)
    {
        std::cout << "SUCCESS" << std::endl;
    }
    else
    {
        std::cout << "FAIL with: " << result << std::endl;

    }

      std::cout << "END" << std::endl;
}
int main(void)
{
    test2();
    return 0;
}