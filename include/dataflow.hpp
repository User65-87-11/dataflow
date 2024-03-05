#include <vector>
#include <map>
#include <string>
#include <unordered_map>
#include <set>
#include <memory>
#include <list>
#include <stdexcept>

#pragma once
namespace bo
{

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
        NODE_OF_UNSET,

    };

    enum class PORT_TYPE
    {
        INPUT,
        OUTPUT,
        UNSET,
        INPUT_OPT
    };

    class Connection
    {

    public:
        int id = 0;
        int src_port_id = 0;
        int dst_port_id = 0;
        int src_node_id = 0;
        int dst_node_id = 0;

        Connection();
        void print();
    };
    class Node;
    class Property;
    class GraphManager;
    class Port
    {
    public:
        int id = 0;
        PORT_TYPE port_type = PORT_TYPE::UNSET;
        DATA_TYPE data_type = DATA_TYPE::UNSET;
        int prop_id = 0;
        GraphManager *gmr = nullptr;

        Port();
        Port(PORT_TYPE port_type, DATA_TYPE t);
        ~Port();
        void setPortPropId(int id);
        Node *node();
        Connection *connect(Port *dst);
        void disconnect();
        Property *property();

        void print();
    };

    class Property
    {
    public:
        int id = 0;
        DATA_TYPE type = DATA_TYPE::UNSET;
        void *data = nullptr;

        Property(void *data, DATA_TYPE type);
        virtual ~Property();
    };
    class Node
    {

    public:
        int id = 0;
        NODE_TYPE type = NODE_TYPE::NODE_OF_UNSET;
        Property *main = nullptr;
        std::set<Node *> dest_nodes;
        std::vector<Property *> original_prop;
        std::vector<Property *> actual_props;

        //-------------
        GraphManager *mgr = nullptr;

        Node();
        virtual ~Node();
        void print();
        virtual void evaluate();

        void setMainProperty(Property *prop);
        void setOriginalProperty(int idx, Property *prop);
        void setRemoteProperty(int idx, Property *prop);
        void restorOriginalProperty(int idx);
        int propertyIndex(Property *prop);
        bool isOriginalProp(Property *prop);

        Port *port(const std::string &name);
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
        std::unordered_map<int, Node *> nodes;
        std::unordered_map<int, Port *> ports;
        std::unordered_map<int, Property *> properties;
        std::unordered_map<int, Connection *> connections;
        std::unordered_map<int, std::string> port_id_port_name;
        std::unordered_map<int, int> port_id_conn_id;
        std::unordered_map<int, int> port_id_node_id;

        // node_id + port_name concat

        Node *createNode(NODE_TYPE type);
        Port *createPort(const std::string &name, PORT_TYPE port_type, DATA_TYPE data_type);
        Property *createProperty(void *data, DATA_TYPE);
        void attachPortProp(Port *, Property *prop);
        void attachNodePort(Node *node, Port *port);
        Connection *findConnByPortId(int id);
        Node *findNodeByNodeId(int id);
        Node *findNodeByPortId(int id);

        Port *findPortByPortId(int id);
        Property *getPropById(int id);
        void setPortName(const std::string &name, int id);
        void printPortNodeIds();
        Port *findPortByNameAndNode(const std::string &name, Node *node);
        Connection *connect(Node *src_node, Port *src_port, Node *dst_node, Port *dst_port);

        void disconnect(Node *src_node, Port *src_port, Node *dst_node, Port *dst_port);
        void disconnect(Connection *conn);
    };

    template <typename T>
    class DataHandler
    {
    public:
        static T data(void *data)
        {
            if (data == nullptr)
            {
                throw std::runtime_error("null data");
            }
            else
            {
                return static_cast<T>(data);
            }
        }
    };

    inline int genId()
    {
        static int cnt = 0;
        cnt++;
        return cnt;
    };
}