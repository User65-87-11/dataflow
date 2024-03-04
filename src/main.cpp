#include <iostream>

#include "dataflow.hpp"

int main(void)
{
    try
    {
        // Code that may throw an exception

        std::cout << "---hello dataflow------" << std::endl;

        GraphManager gm;

        Node *a = gm.createNode(NODE_TYPE::NODE_OF_DOUBLE_OUT);
        Node *b = gm.createNode(NODE_TYPE::NODE_OF_DOUBLE_OUT);
        Node *c = gm.createNode(NODE_TYPE::NODE_OF_SUM);

        a->port("out")->connect(c->port("in_a"));
        b->port("out")->connect(c->port("in_b"));

        c->evaluate();

        double *result = static_cast<double *>(c->port("out")->property()->data);
    }
    catch (const std::exception &e)
    {
        // Handle the exception
        std::cerr << "Exception caught: " << e.what() << std::endl;
    }
    return 0;
}