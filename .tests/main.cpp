#include <iostream>
#include <memory>
#include "dataflow.hpp"

void test2()
{
    try
    {

        std::cout << "TEST 02" << std::endl;

        GraphManager gm;

        Node *a = gm.createNode(NODE_TYPE::NODE_OF_DOUBLE_OUT);
        Node *b = gm.createNode(NODE_TYPE::NODE_OF_DOUBLE_OUT);
        Node *c = gm.createNode(NODE_TYPE::NODE_OF_SUM);

        // gm.connect(a, gm.findPortByNameAndNode("out", a), c, gm.findPortByNameAndNode("in_a", c));
        // gm.connect(b, gm.findPortByNameAndNode("out", b), c, gm.findPortByNameAndNode("in_b", c));

        a->port("out")->connect(c->port("in_a"));
        b->port("out")->connect(c->port("in_b"));

        // gm.printPortNodeIds();
        c->evaluate();

        Node *d = gm.createNode(NODE_TYPE::NODE_OF_DOUBLE_OUT);
        Node *e = gm.createNode(NODE_TYPE::NODE_OF_SUM);

        c->port("out")->connect(e->port("in_a"));
        d->port("out")->connect(e->port("in_b"));

        e->evaluate();

        // // gm.connectPorts(c->getPortOutByPos(0), a->getInputPortByPos(0));
        // // gm.connectPorts(b->getPortOutByPos(0), a->getInputPortByPos(1));

        // gm.connect(c, gm.findPortByNameAndNode("out", c), d, gm.findPortByNameAndNode("in_a", d));
        // gm.connect(e, gm.findPortByNameAndNode("out", e), d, gm.findPortByNameAndNode("in_b", d));

        // a->evaluate();

        double *result = static_cast<double *>(e->port("out")->property()->data);

        std::cout << " res:" << *result << std::endl;
        if (*result == 50)
        {
            std::cout << "SUCCESS" << std::endl;
        }
        else
        {
            std::cout << "FAIL with: " << result << std::endl;
        }

        std::cout << "END" << std::endl;
    }
    catch (const std::exception &e)
    {
        // Handle the exception
        std::cerr << "Exception caught: " << e.what() << std::endl;
    }
}
int main(void)
{
    test2();
    return 0;
}