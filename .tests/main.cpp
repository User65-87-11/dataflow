#include <iostream>
#include <memory>
#include "dataflow.hpp"

struct NodeData2
{
    void *data;
    int data_type;
};
void test01()
{
    std::cout << "TEST01" << std::endl;
    std::vector<std::unique_ptr<NodeData2>> node_data;

    // Add first element
    int data1 = 10;
    std::unique_ptr<NodeData2> nd1(new NodeData2({&data1, 0}));
    node_data.push_back(std::move(nd1));
    NodeData2 *ref = node_data.back().get();

    std::cout << "ref: " << ref << std::endl;
    std::cout << "ref->data: " << ref->data << std::endl;
    // Add second element
    int data2 = 20;


    std::unique_ptr<NodeData2> nd2(new NodeData2({&data2, 0}));
    node_data.push_back(std::move(nd2));

    // Check ref
    std::cout << "ref: " << ref << std::endl;
    std::cout << "ref->data: " << ref->data << std::endl;
}
void test2()
{
    std::cout << "TEST02" << std::endl;

    GraphManager gm;

    Node *a = gm.createNode(NODE_TYPE::NODE_OF_DOUBLE_OUT);
    Node *b = gm.createNode(NODE_TYPE::NODE_OF_DOUBLE_OUT);
    Node *c = gm.createNode(NODE_TYPE::NODE_OF_SUM);

    gm.connectPorts(a->getPortOutByPos(0), c->getInputPortByPos(0));
    gm.connectPorts(b->getPortOutByPos(0), c->getInputPortByPos(1));

    c->evaluate();

    a = gm.createNode(NODE_TYPE::NODE_OF_SUM);

    gm.connectPorts(c->getPortOutByPos(0), a->getInputPortByPos(0));
    gm.connectPorts(b->getPortOutByPos(0), a->getInputPortByPos(1));

    a->evaluate();

    double *d = static_cast<double *>(a->getPortOutByPos(0)->prop->own_data->data);

    if (*d == 30)
    {
        std::cout << "SUCCESS" << std::endl;
    }
    else
    {
        std::cout << "FAIL" << std::endl;
    }
}
int main(void)
{
    test2();
    return 0;
}