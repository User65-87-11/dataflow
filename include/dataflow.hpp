#include <vector>
#include <map>

#pragma once

enum class DATA_TYPE
{
    FLOAT,
    DOUBLE,
    INT,
    STRING,
    UNSET
};
enum class NODE_TYPE
{
    NODE_OF_SUM,
    NODE_OF_DOUBLE_OUT,
    NODE_OF_STRING_OUT,
    NODE_OF_STRING_CONCAT,

};

enum class PORT_TYPE
{
    INPUT,
    OUTPUT,
    UNSET
};

class Edge
{

public:
    Edge();
    int edge_id;
    int src_port_id;
    int dst_port_id;
    void print();
    // Port * src;
    // Port * dst;
};
class Node;
class NodeData;
class Port
{
public:
    PORT_TYPE port_type;
    DATA_TYPE data_type;
    Port *remote_port;
    NodeData *node_data;
    int port_id;
    int edge_id;

    Port();
    Port(PORT_TYPE port_type, NodeData *d);
    Port(PORT_TYPE port_type);
     Port(PORT_TYPE port_type,DATA_TYPE t);
    ~Port();
    void print();
    void setPortNodeData(NodeData *);
    NodeData *getPortNodeData();
};

class NodeData
{
public:
    void *data;
    DATA_TYPE data_type;

    NodeData();
    NodeData(void *d, DATA_TYPE t);
    ~NodeData();
};
class Node
{

public:
    int node_id;
    bool complete;
    NODE_TYPE node_type;
    //  Port port_out;

    std::vector<Port> ports_out;
    std::vector<Port> ports_in;
    std::vector<NodeData> node_data;

    Node();
    ~Node();
    void print();
    virtual void evaluate();
    Port *addOutputPort(DATA_TYPE type);
    Port *addOutputPort();
    NodeData *addNodeData(void *d, DATA_TYPE t);
    Port *addInputPort(DATA_TYPE type);
    Port *addInputPort();
    Port *getPortOutByPos(int pos);
    Port *getInputPortByPos(int pos);
};
class NodeOfDoubleOut : public Node
{
public:
    NodeOfDoubleOut();
    ~NodeOfDoubleOut();
    void evaluate() override;
};

class NodeOfStringOut : public Node
{
public:
    NodeOfStringOut();
    ~NodeOfStringOut();
    void evaluate() override;
};

class NodeOfSum : public Node
{

public:
    NodeOfSum();
    ~NodeOfSum();

    void evaluate() override;
};

class NodeOfStringConcat : public Node
{

public:
    NodeOfStringConcat();
    ~NodeOfStringConcat();

    void evaluate() override;
};

//-------GRAPH MANAGER
class GraphManager
{
public:
    GraphManager();
    ~GraphManager();
    std::map<int, Edge *> edges;
    std::map<int, Node *> nodes;
    std::map<int, Port *> ports;
    std::map<int, NodeData *> node_datas;

    Node *createNode(NODE_TYPE node_type);
    Edge *connectPorts(Port *src_port_id, Port *dest_port_id);
    void regNodeData(NodeData * nd);
    void disconnectPorts(Edge *edge);

private:
    void portAdd(Port *);
    void nodeAdd(Node *);
    Port *getPortIfExist(int port_id);
    Edge *getEdgeOrNull(int edge_id);
};

inline int genId()
{
    static int cnt = 0;
    cnt++;
    return cnt;
};