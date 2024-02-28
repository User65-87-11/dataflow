#include "dataflow.hpp"
#include <iostream>

/*
    output ports have own value allocated

    input ports receive poitner but dont deallocate values;

*/

std::string nodeTypeToString(NODE_TYPE type)
{
    switch (type)
    {
    case NODE_TYPE::NODE_OF_DOUBLE_OUT:
        return "NODE_OF_DOUBLE_OUT";

    case NODE_TYPE::NODE_OF_STRING_OUT:
        return "NODE_OF_STRING_OUT";

    case NODE_TYPE::NODE_OF_STRING_CONCAT:
        return "NODE_OF_STRING_CONCAT";
    case NODE_TYPE::NODE_OF_SUM:
        return "NODE_OF_SUM";
    default:
        return "NODE_OF_?";
    }
}
std::string dataTypeToString(DATA_TYPE type)
{
    switch (type)
    {
    case DATA_TYPE::DOUBLE:
        return "DATA_TYPE DOUBLE";

    case DATA_TYPE::STRING:
        return "DATA_TYPE STRING";

    case DATA_TYPE::INT:
        return "DATA_TYPE INT";

    case DATA_TYPE::FLOAT:
        return "DATA_TYPE FLOAT";

    default:
        return "DATA_TYPE?";
    }
}

inline void print1(int i)
{
    std::cout << "dbg: " << i << std::endl;
}

//---------NODE_DATA---------
NodeData::NodeData()
{
    this->data = nullptr;
    this->data_type = DATA_TYPE::UNSET;
}
NodeData::NodeData(void *d, DATA_TYPE t)
{

    this->data = d;
    this->data_type = t;
}
NodeData::~NodeData()
{
}

//---------Edge

Edge::Edge()
{
    edge_id = genId();
    src_port_id = 0;
    dst_port_id = 0;
};
void Edge::print()
{
    std::cout << "edge_id: " << edge_id << std::endl;
    std::cout << " src_port_id: " << src_port_id << std::endl;
    std::cout << " dst_port_id: " << this->dst_port_id << std::endl;
}
//--------PORT
Port::Port()
{
    port_id = genId();
    edge_id = 0;
    remote_port = nullptr;
    node_data = nullptr;
    port_type = PORT_TYPE::UNSET;
    data_type = DATA_TYPE::UNSET;
}

Port::Port(PORT_TYPE port_type, NodeData *d) : Port(port_type)
{
    node_data = d;
    data_type = d->data_type;
}
Port::Port(PORT_TYPE port_type) : Port()
{
    port_type = port_type;
}
Port::Port(PORT_TYPE port_type, DATA_TYPE t) : Port(port_type)
{
    data_type = t;
}
void Port::setPortNodeData(NodeData *d)
{
    node_data = d;
    data_type = d->data_type;
}
NodeData *Port::getPortNodeData()
{
    return node_data;
}
void Port::print()
{
    std::cout << "portid: " << port_id << std::endl;
    std::cout << " input?: " << (port_type == PORT_TYPE::INPUT ? true : false) << std::endl;
    std::cout << " edgeid: " << this->edge_id << std::endl;

    std::cout << " remote_port: " << this->remote_port << std::endl;
}
Port::~Port() {}

//---------NODE START--------------
Node::Node()
{
    node_id = genId();
    complete = false;
    //  port_out = nullptr;
}
NodeData *Node::addNodeData(void *d, DATA_TYPE t)
{
    NodeData nd(d, t);

    this->node_data.push_back(nd);

    return &this->node_data.back();
}
Port *Node::addOutputPort()
{

    Port new_port(PORT_TYPE::OUTPUT); // = new Port;
    this->ports_out.push_back(new_port);
    return &this->ports_out.back();
}

Port *Node::getPortOutByPos(int pos)
{
    if (pos < this->ports_out.size())
    {
        return &this->ports_out[pos];
    }
    else
    {
        std::string str = "getPortOutByPos pos is bad:" + std::to_string(pos);

        throw std::runtime_error(str);
    }

    // return &this->ports_out[pos];
}
Port *Node::getInputPortByPos(int pos)
{
    if (pos < this->ports_in.size())
    {
        return &this->ports_in[pos];
    }
    else
    {
        std::string str = "getPortByPos pos is bad:" + std::to_string(pos);

        throw std::runtime_error(str);
    }
}
Port *Node::addInputPort()
{
    Port new_port(PORT_TYPE::INPUT); // = new Port;
    this->ports_in.push_back(new_port);

    return &this->ports_in.back();
}
Port *Node::addInputPort(DATA_TYPE t)
{
    Port new_port(PORT_TYPE::INPUT, t); // = new Port;
    this->ports_in.push_back(new_port);

    return &this->ports_in.back();
}

void Node::print()
{
    std::cout << "NodeId: " << node_id << std::endl;
    std::cout << " NodeType: " << nodeTypeToString(node_type) << std::endl;
}
Node::~Node()
{

    ports_out.clear();
    ports_in.clear();
    node_data.clear();
};
void Node::evaluate() {}

//-----------NODE_OF_DOUBLE_OUT-------
NodeOfDoubleOut::NodeOfDoubleOut()
{
    this->node_type = NODE_TYPE::NODE_OF_DOUBLE_OUT;
    this->node_id = genId();
}
NodeOfDoubleOut::~NodeOfDoubleOut() {}
void NodeOfDoubleOut::evaluate()
{
}

//-----------NODE_OF_STRING_OUT-------
NodeOfStringOut::NodeOfStringOut()
{
    this->node_type = NODE_TYPE::NODE_OF_STRING_OUT;
    this->node_id = genId();
}
NodeOfStringOut::~NodeOfStringOut() {}
void NodeOfStringOut::evaluate() {}

//-----------NodeOfSum---------
NodeOfSum::NodeOfSum()
{
    this->node_type = NODE_TYPE::NODE_OF_SUM;
    this->node_id = genId();
}
NodeOfSum::~NodeOfSum() {}

void NodeOfSum::evaluate()
{

    if (this->ports_in[0].remote_port != nullptr && this->ports_in[1].remote_port != nullptr)
    {

        double *d = static_cast<double *>(this->node_data[0].data);

        double *op1 = static_cast<double *>(this->ports_in[0].remote_port->node_data->data);

        double *op2 = static_cast<double *>(this->ports_in[1].remote_port->node_data->data);

        *d = *op1 + *op2;

    }
    else
    {
        std::cout << "operands null" << std::endl;
    }
}

//-----------NodeOfStringConcat---------
NodeOfStringConcat::NodeOfStringConcat()
{
    this->node_type = NODE_TYPE::NODE_OF_STRING_CONCAT;
    this->node_id = genId();
}
NodeOfStringConcat::~NodeOfStringConcat() {}
void NodeOfStringConcat::evaluate()
{

    if (this->ports_in[0].remote_port != nullptr && this->ports_in[1].remote_port != nullptr)
    {

        std::string *d = static_cast<std::string *>(this->node_data[0].data);
        std::string *op1 = static_cast<std::string *>(this->ports_in[0].remote_port->node_data->data);
        std::string *op2 = static_cast<std::string *>(this->ports_in[1].remote_port->node_data->data);

        *d = *op1 + *op2;

        std::cout << "NodeOfStringConcat::evaluates to: " << *d << std::endl;
    }
    else
    {
        std::cout << "operands null" << std::endl;
    }
}

//---------------GRAPH MANAGER
GraphManager::GraphManager() {}
GraphManager::~GraphManager()
{
    // std::cout << "~GraphManager " << std::endl;

    // std::cout << " Nodes:" << nodes.size() << std::endl;

    // std::cout << " Edge:" << edges.size() << std::endl;

    // std::cout << " Ports:" << ports.size() << std::endl;

    for (auto it = nodes.begin(); it != nodes.end(); ++it)
    {

        for (Port &p : it->second->ports_in)
        {
            disconnectPorts(getEdgeOrNull(p.edge_id));
        }
        for (Port &p : it->second->ports_out)
        {
            disconnectPorts(getEdgeOrNull(p.edge_id));
        }
    }

    nodes.clear();

    for (auto it = node_datas.begin(); it != node_datas.end(); ++it)
    {
        NodeData *nd = it->second;
        switch (nd->data_type)
        {
        case DATA_TYPE::STRING:
        {
            std::string *d = static_cast<std::string *>(nd->data);
            delete d;
        }
        break;
        case DATA_TYPE::DOUBLE:
        {
            double *d = static_cast<double *>(nd->data);
            delete d;
        }
        break;
        case DATA_TYPE::UNSET:
        {
        }
        break;
        }
    }
    node_datas.clear();
}
void GraphManager::nodeAdd(Node *n)
{
    nodes[n->node_id] = n;
}
void GraphManager::regNodeData(NodeData *nd)
{
    int id = genId();
    node_datas[id] = nd;
}
Node *GraphManager::createNode(NODE_TYPE node_type)
{
    Node *ret = nullptr;
    switch (node_type)
    {
    case NODE_TYPE::NODE_OF_STRING_OUT:
    {
        NodeOfStringOut *n = new NodeOfStringOut();

        portAdd(n->addOutputPort());
        n->getPortOutByPos(0)->setPortNodeData(n->addNodeData(new std::string("Hello"), DATA_TYPE::STRING));
        nodeAdd(n);

        ret = n;
    }
    break;

    case NODE_TYPE::NODE_OF_DOUBLE_OUT:
    {
        NodeOfDoubleOut *n = new NodeOfDoubleOut();

        portAdd(n->addOutputPort());
        n->getPortOutByPos(0)->setPortNodeData(n->addNodeData(new double(10), DATA_TYPE::DOUBLE));

        nodeAdd(n);
        ret = n;
    }
    break;
    case NODE_TYPE::NODE_OF_STRING_CONCAT:
    {
        NodeOfStringConcat *n = new NodeOfStringConcat();

        portAdd(n->addOutputPort());
        portAdd(n->addInputPort(DATA_TYPE::STRING));
        portAdd(n->addInputPort(DATA_TYPE::STRING));

        n->getPortOutByPos(0)->setPortNodeData(n->addNodeData(new std::string(""), DATA_TYPE::STRING));

        nodeAdd(n);

        ret = n;
    }
    break;

    case NODE_TYPE::NODE_OF_SUM:
    {
        NodeOfSum *n = new NodeOfSum();

        portAdd(n->addOutputPort());
        portAdd(n->addInputPort(DATA_TYPE::DOUBLE));
        portAdd(n->addInputPort(DATA_TYPE::DOUBLE));

        n->getPortOutByPos(0)->setPortNodeData(n->addNodeData(new double(5.0), DATA_TYPE::DOUBLE));

        nodeAdd(n);

        ret = n;
    }
    break;

    default:

        std::cout << "createNode " << nodeTypeToString(node_type) << std::endl;
        break;
    }

    return ret;
}
Edge *GraphManager::getEdgeOrNull(int edge_id)
{
    Edge *e = nullptr;
    if (edges.find(edge_id) != edges.end())
    {
        e = edges[edge_id];
    }
    return e;
}

Edge *GraphManager::connectPorts(Port *src, Port *dst)
{
    Edge *ret = new Edge();
    src->edge_id = ret->edge_id;
    dst->edge_id = ret->edge_id;
    src->remote_port = dst;
    dst->remote_port = src;
    ret->src_port_id = src->port_id;
    ret->dst_port_id = dst->port_id;

    edges[ret->edge_id] = ret;

    return ret;
}
void GraphManager::portAdd(Port *port)
{

    if (ports.find(port->port_id) == ports.end())
    {
        ports[port->port_id] = port;
    }
}
Port *GraphManager::getPortIfExist(int port_id)
{
    Port *ret = nullptr;
    if (ports.find(port_id) != ports.end())
    {
        ret = ports[port_id];
    }
    return ret;
}
void GraphManager::disconnectPorts(Edge *edge)
{
    if (edge != nullptr)
    {

        Port *src = getPortIfExist(edge->src_port_id);
        if (src != nullptr)
        {
            src->remote_port = nullptr;
            src->edge_id = 0;
        }

        Port *dst = getPortIfExist(edge->dst_port_id);
        if (dst != nullptr)
        {
            dst->remote_port = nullptr;
            dst->edge_id = 0;
        }

        edges.erase(edge->edge_id);

        delete edge;
    }
}