// consistent hash
#include <iostream>
#include <string>
#include <functional>
#include <map>

using namespace std;

class Node {
private:
    friend class hash<Node>;
    string name_;
    size_t id_;
public:
    Node()=default;
    Node(string nm, size_t id)
        :name_(nm),
         id_(id){};
    string get_name() {return name_;}
    size_t get_id() {return id_;}
};

namespace std {
template<>
struct hash<Node> 
{
    typedef size_t result_type;
    typedef Node arguement_type;
    
    size_t operator()(const Node& nd) const;
}; // struct

size_t
hash<Node>::operator()(const Node& nd) const
{
    return hash<string>()(nd.name_);
}

} // close std

template<typename Node, typename Key>
class VirtualRing {
public:
    VirtualRing(uint64_t num_replicas)
            :num_replicas_(num_replicas){};
    size_t AddNode(Node& nd); 
    // bool RemoveNode(const Node& nd); 
    Node GetNode(Key& ky);

private:
    map<size_t, Node> ring_; 
    size_t num_replicas_; 
}; 

template<typename Node, typename Key>
size_t 
VirtualRing<Node, Key>::AddNode(Node& nd)
{
    size_t h;
    for(auto i = 0; i < num_replicas_; i++){
        h = hash<string>()(nd.get_name() + to_string(i));
        ring_[h] = nd;
        // cout << "Node Name:" << nd.get_name() << " hash:" << h << endl;
    }
    return h;
}

template<typename Node, typename Key>
Node VirtualRing<Node, Key>::GetNode(Key& ky)
{
/*
    Error handling
    if(ring_.empty()) {
        return  ;
    }
*/
    size_t h = hash<string>()(ky);
    typename map<size_t, Node>::const_iterator it;
    it = ring_.lower_bound(h);
    if(it == ring_.end()){
       it = ring_.begin(); 
    }
    // cout << "Node Name:" << it->second << " hash:" << h << endl;
    // cout << "hash: " << h << endl;
    return it->second;
}

int 
main() {
    VirtualRing <Node, string> vr(3);
    Node n1("Node1", 11);
    Node n2("Node2", 22);
    Node n3("Node3", 33);
    Node n4("Node4", 44);
    Node n5("Node5", 55);
    size_t nh = vr.AddNode(n1);
    nh = vr.AddNode(n2);
    nh = vr.AddNode(n3);
    nh = vr.AddNode(n4);
    nh = vr.AddNode(n5);
    string k("3werddll");
    Node mnd = vr.GetNode(k);
    cout << " Key: " << k << " inserted in ";
    cout << "Node:" << mnd.get_name() << endl;
}
