#include "interface.h"
#include <algorithm>
#include <queue>

// Rule of 5
Hierarchy::~Hierarchy() noexcept
{ free(root); }

Hierarchy::Hierarchy(Hierarchy&& r) noexcept
{
    std::swap(root, r.root);
    std::swap(m_size, r.m_size);
}

Hierarchy::Hierarchy(const Hierarchy& r)
        : root{copy(r.root)}, m_size(r.m_size)
{}

Hierarchy::Hierarchy(Node* root)
    : root(root)
{ m_size = weight(root); }

Hierarchy::Hierarchy(const string& data)
{
    if(data != "")
    {
        add_root();
        ++m_size;
        // can have a person only once as subordinate,
        // i.e. if they are in tree already, cannot add them again
        data_parser parser;
        parser.parse_text(data);
        //int i = 0;
        for(const auto& pair : parser.pairs())
        {
            Node* found_in_branch = find_by_key(pair.subordinate);
            if(found_in_branch)
            {
                free(root);// so as to prevent a memory leak
                throw std::invalid_argument(pair.subordinate
                                            + " is already a subordinate in the current branch.");
            }
            Node* manager = find_by_key(pair.manager);
    //std::cout << "man --- name: " << manager->name << std::endl;
            if(!manager)
            {
                free(root);// so as to prevent a memory leak
                throw std::runtime_error(pair.manager + " - no such manager is employed in current branch");
            }
    //std::cout << "here 0\n";
            add_as_child(manager, pair.subordinate);
            ++m_size;
    //std::cout << "here 1\n";
        }
    }

}

void Hierarchy::free(Node* root)
{
    if(!root) return;
    for(Node* direct_subordinate : root->subordinates)
        free(direct_subordinate);
    root->subordinates.clear();
    delete root;
    root = nullptr;
}
Hierarchy::Node* Hierarchy::copy(const Node* root)
{
    if(!root) return nullptr;
    Node* res = new Node{ root->name };
    for(const Node* direct_subordinate: root->subordinates)
        res->subordinates.emplace_back(copy(direct_subordinate));
    return res;
}
/// performs a BFS
string Hierarchy::print() const
{
    std::string result;
    if(!root) return result;
    std::queue<Node*> q;
    q.push(root);
    while(!q.empty())
    {
        Node* parent = q.front();
        q.pop();
        parent->subordinates.sort([](Node* a, Node* b){ return a->name < b->name; });
        for(Node* child : parent->subordinates)
        {
            if(child)
            {
                q.push(child);
                result += parent->name + "-" + child->name + "\n";
            }
        }
    }
    return result;
}
bool Hierarchy::find(const string& name) const
{ return find_by_key(name); }
int Hierarchy::num_employees() const
{ return m_size; }
int Hierarchy::num_overloaded(int level) const
{
    int result = 0;
    num_overloaded_helper(root, level, result);
    return result;
}
/// does not make unnecessary steps :D
/// returns the weight and accumulates number of overloaded employees in a parameter
int Hierarchy::num_overloaded_helper(Node* root, int N, int& overloaded)
{
    if(!root) return -1;
    int cnt = root->subordinates.size();
    for(auto& child : root->subordinates)
        cnt += num_overloaded_helper(child, N, overloaded);
    if(cnt > N) ++overloaded;
    return cnt;
}
string Hierarchy::manager(const string& name) const
{
    //std::cout << branch.size() << std::endl;
    if(name == "Uspeshnia")
    {
        throw std::logic_error("how dare you, he is the boss of the bosses!");
        return "";
    }
    Node* manager = find_parent(name).parent;
    if(!manager)
        throw std::runtime_error(name + " is not employed in current branch.");
    return manager->name;
}
int Hierarchy::num_subordinates(const string& name) const
{
    Node* root = find_by_key(name);
    if(!root) return -1;
    return root->subordinates.size();
}
unsigned long Hierarchy::getSalary(const string& who) const
{
    Node* temp = find_by_key(who);
    if(!temp)
    {
        throw std::logic_error(who + " is not employed in current branch.");
        return -1;
    }
    return salary(temp);
}
unsigned long Hierarchy::salary(const Node* root)
{
    assert(root);
    int direct = root->subordinates.size();
    int indirect = weight(root) - 1 - direct;
    return 500 * direct + 50 * indirect;
}
bool Hierarchy::fire(const string& who)
{
    if(who == "Uspeshnia")
    {
        throw std::logic_error("how dare you!");
        return false;
    }
    parent_child temp = find_parent(who);
    if(!temp.parent)
    {
        throw std::logic_error("no such person is employed in current branch");
        return false;
    }
    temp.parent->subordinates.splice(temp.parent->subordinates.end(), (*temp.child)->subordinates);
    delete *temp.child;
    temp.parent->subordinates.erase(temp.child);
    --m_size;
    return true;
}
bool Hierarchy::hire(const string& who, const string& boss)
{
    Node* manager = find_by_key(boss);
    if(!manager)
    {
        throw std::logic_error(boss + " is not employed in current branch");
        return false;
    }
    parent_child temp = find_parent(who);
    if(!temp.parent)
    {
        add_as_child(manager, who);
        ++m_size;
    }
    else
    {
        /// get the node from the list of subordinates of its father
        Node* child = *temp.child;
        /// remove it from the list of subordinates of its father
        temp.parent->subordinates.erase(temp.child);
        manager->subordinates.push_back(child);
    }
    return true;
}
/// TODO : implement
void Hierarchy::incorporate()
{ incorporate_helper(root); }
void Hierarchy::incorporate_helper(Node* root)
{
    if(!root) return;
    root->subordinates.sort([](Node* a, Node* b){ return a->name < b->name; });
    for(Node* subordinate : root->subordinates)
        incorporate_helper(subordinate);
    if(root->subordinates.size() <  2)
        return;
    std::list<Node*>::iterator new_boss = root->subordinates.begin();
    unsigned long new_boss_salary = salary(*new_boss);
    for(auto iter = root->subordinates.begin();
        iter != root->subordinates.end();
        ++iter)
    {
        unsigned long current_salary = salary(*iter);
        if(current_salary > new_boss_salary)
        {
            new_boss_salary = current_salary;
            new_boss = iter;
        }
    }
    std::list<Node*>::iterator temp = new_boss;
    (*new_boss)->subordinates.splice((*new_boss)->subordinates.begin(),
                                     root->subordinates,
                                     root->subordinates.begin(),
                                     new_boss);
    (*new_boss)->subordinates.splice((*new_boss)->subordinates.begin(),
                                     root->subordinates,
                                     ++temp,
                                     root->subordinates.end());

}
void Hierarchy::modernize()
{
    modernize_helper(root, true);
    m_size = weight(root);
}

void Hierarchy::modernize_helper(Node* root, bool even)
{
    assert(root);
    if(even)
    {
//std::cout << "before loop\n";
        for (Node *subordinate: root->subordinates)
            modernize_helper(subordinate, false);
        /// the subordinates of its subordinates become its subordinates
        std::size_t num_subordinates = root->subordinates.size();
        std::size_t i = 0;
        for(std::list<Node*>::iterator iter = root->subordinates.begin();
            iter != root->subordinates.end()  && i < num_subordinates; ++i)
        {
//std::cout << "in loop" << root->name << "\n";
//std::cout << "iter: " << (*iter)->name << "\n";
            if(((*iter)->subordinates.empty())) { ++iter; continue; }
            root->subordinates.splice(root->subordinates.end(), (*iter)->subordinates);
            std::list<Node*>::iterator temp = iter;
            ++temp;
            delete (*iter);
            root->subordinates.erase(iter);
            iter = temp;
        }
    }
    else
    {
        for(Node* subordinate : root->subordinates)
            modernize_helper(subordinate, true);
    }
}

Hierarchy Hierarchy::join(const Hierarchy& right) const
{
    return join_helper(root, right.root); // uses converting ctor
}

Hierarchy::Node* Hierarchy::generate_root(const std::string& name)
{
    return new Node{ name };
}
/// performs a real time BFS traversal of result tree
Hierarchy::Node* Hierarchy::join_helper(const Node* root1, const Node* root2)
{
    Node* result = generate_root();
    std::queue<Node*> q;
    q.push(result);
    while(!q.empty())
    {
        Node* current = q.front();
        q.pop();
        node_level in_first_tree = find_level(root1, current->name);
        node_level in_second_tree = find_level(root2, current->name);
        // check_for_conflict(in_first_tree.node, root2)
        // check_for_conflict(in_second_tree.node, root1)
        if(in_first_tree.node)
        {
            for(Node* child : in_first_tree.node->subordinates)
                if(!find_rec(result, child->name))
                    q.push(add_as_child(current, child->name));
        }
        if(in_second_tree.node)
        {
            for(Node* child : in_second_tree.node->subordinates)
                if(!find_rec(result, child->name))
                    q.push(add_as_child(current, child->name));
        }
    }
    return result;
}
Hierarchy::node_level Hierarchy::find_level(const Node* root, const std::string& key)
{
    assert(root);
    std::queue<const Node*> q;
    q.push(root);
    q.push(nullptr);
    int level = 0;
    while(!q.empty())
    {
        const Node* top = q.front();
        q.pop();
        if(!top)
        {
            if(q.empty()) return node_level{};
            ++level;
            q.push(nullptr);
        }
        else
        {
            if(top->name == key)
                return node_level{ top, level };
            for(Node* child : top->subordinates)
                q.push(child);
        }
    }
    return node_level{};
}
Hierarchy::Node* Hierarchy::is_subordinate(const Node* root, const std::string& key)
{
    assert(root);
    for(Node* child : root->subordinates)
    {
        if(child->name == key)
            return child;
        Node* depth = is_subordinate(child, key);
        if(depth) return depth;
    }
    return nullptr;
}

const Hierarchy::Node* Hierarchy::find_rec(const Node* root, const std::string& name)
{
//std::cout << "find_rec called\n";
    if(!root || root->name == name) return root;
    for(Node* direct_subordinate : root->subordinates) {
        const Node* find_res = find_rec(direct_subordinate, name);
        if(find_res) return find_res;
    }
    return nullptr;
}
Hierarchy::Node* Hierarchy::find_rec(Node* root, const std::string& name)
{
    return const_cast<Node*>(find_rec(const_cast<const Node*>(root), name));
}
Hierarchy::parent_child Hierarchy::find_parent(Node* root, const std::string& child_value)
{
    assert(root);
    for(auto iter = root->subordinates.begin();
        iter!=root->subordinates.end();
        ++iter)
    {
        Node* child = *iter;
        if(child->name == child_value)
            return { root, iter };
        parent_child res = find_parent(child, child_value);
        if(res.parent) return res;
    }
    return { nullptr, root->subordinates.end() };
}
std::size_t Hierarchy::weight(const Node* root)
{
    if(!root) return 0;
    std::size_t cnt = 1;
    for(Node* child : root->subordinates)
        cnt += weight(child);
    return cnt;
}
std::size_t Hierarchy::height(Node* root)
{
    if(!root) return 0;
    std::size_t cnt = 1;
    std::size_t best_child_height = 0;
    for(Node* child : root->subordinates)
    {
        std::size_t temp = height(child);
        if(temp > best_child_height) best_child_height = temp;
    }
    return best_child_height + 1;
}
Hierarchy::parent_child Hierarchy::find_parent(const std::string& child)  const
{ return find_parent(root, child); }
Hierarchy::parent_child Hierarchy::find_parent(const std::string& child)
{ return find_parent(root, child); }
Hierarchy::Node* Hierarchy::find_by_key(const std::string& key) const
{
//std::cout << "find_by_key called\n";
    return find_rec(root, key);
}
int Hierarchy::longest_chain() const
{
    if(!root) return 0;
    return height(root);
}
Hierarchy::Node* Hierarchy::add_as_child(Hierarchy::Node* parent, const std::string& name)
{
    assert(parent);
    Node* new_node = new Node{ name };
    parent->subordinates.push_back(new_node);
    return new_node;
}
int Hierarchy::num_all_subordinates(const string& name) const
{
    Node* root = find_by_key(name);
    if(!root) return -1;
    return weight(root) - 1;
}
void Hierarchy::add_root(const std::string& name)
{ root = generate_root(name); }