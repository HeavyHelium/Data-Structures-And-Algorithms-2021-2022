#include "interface.h"
#include <algorithm>
#include <queue>

// Rule of 5(minus operator=)
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
            if(!manager)
            {
                free(root);// so as to prevent a memory leak
                throw std::runtime_error(pair.manager + " - no such manager is employed in current branch");
            }
            add_as_child(manager, pair.subordinate);
            ++m_size;
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
/// does not make unnecessary steps
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
    if(name == boss_name)
    {
        return "";
    }
    Node* manager = find_parent(name).parent;
    if(!manager)
        throw std::runtime_error("There is no " + name);
        //return "";
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
//throw std::logic_error(who + " is not employed in current branch.");
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
    if(who == boss_name)
    {
//throw std::logic_error("how dare you!");
        return false;
    }
    parent_child temp = find_parent(who);
    if(!temp.parent)
    {
//throw std::logic_error("no such person is employed in current branch");
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
    if(who == boss) return false;
    Node* manager = find_by_key(boss);
    if(!manager)
    {
    //throw std::logic_error(boss + " is not employed in current branch");
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
        // get the node from the list of subordinates of its father
        Node* child = *temp.child;
        // remove it from the list of subordinates of its father
        temp.parent->subordinates.erase(temp.child);
        manager->subordinates.push_back(child);
    }
    return true;
}

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
        for (Node *subordinate: root->subordinates)
            modernize_helper(subordinate, false);
        std::size_t num_subordinates = root->subordinates.size();
        std::size_t i = 0;
        for(std::list<Node*>::iterator iter = root->subordinates.begin();
            iter != root->subordinates.end()  && i < num_subordinates; ++i)
        {
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
{ return new Node{ name }; }
/// performs a real time BFS traversal of the resulting tree
Hierarchy::Node* Hierarchy::join_helper(const Node* root1, const Node* root2)
{
    Node* result = generate_root();
    std::queue<left_right> q;
    q.push({ root1, result, root2, 0 });
    while(!q.empty())
    {
        left_right front = q.front();
        q.pop();

        if((front.left_tree && check_for_conflict(front.left_tree, root2))
            || (front.right_tree && check_for_conflict(front.right_tree, root1)))
        {
            free(result); // so as to prevent a memory leak
            std::cerr << "error: joning of the two hierarchies is impossible\n";
            return nullptr;
//throw std::runtime_error("Joining of the two hierarchies is impossible");
        }

        if(front.left_tree)
        {
            for(Node* child : front.left_tree->subordinates)
            {
                node_level in_other = find_level(root2, child->name);
                if (!find_rec(result, child->name))
                {
//std::cout << in_other.level << " " << front.level + 1 << std::endl;
//std::cout << "here " + front.current_node->name + child->name + "\n";
                    if (in_other.node && in_other.level == front.level + 1)
                    {
//std::cout << "same level both trees " + child->name + "\n";
                        if (front.current_node->name < in_other.parent->name) {
                            std::cout << front.current_node->name + " " + in_other.parent->name + "\n";
                            q.push({child,
                                    add_as_child(front.current_node, child->name),
                                    in_other.node,
                                    front.level + 1}); // aggregate initialization
                        }

                        else {
//std::cout << "here " + child->name + "\n";
                            q.push({ child,
                                     add_as_child(find_rec(result, in_other.parent->name), child->name),
                                     in_other.node,
                                     front.level + 1 });
                        }
                    }
                    else q.push({ child,
                                  add_as_child(front.current_node, child->name),
                                  in_other.node, front.level + 1 });
                }
            }
        }
        if(front.right_tree)
        {
            for(Node* child : front.right_tree->subordinates)
                if(!find_rec(result, child->name))
                    q.push({ child,
                             add_as_child(front.current_node, child->name),
                             find_rec(root1, child->name),
                             front.level + 1} );
        }
    }
    return result;
}
bool Hierarchy::check_for_conflict(const Node* node, const Node* other_tree)
{
    for(const Node* subordinate : node->subordinates)
    {
        if(check_for_conflict(subordinate, other_tree))
            return true;
        const Node* in_other_tree = find_rec(other_tree, subordinate->name);
        if(in_other_tree &&
           is_subordinate(in_other_tree, node->name))
            return true;
    }
    return false;
}

Hierarchy::node_level Hierarchy::find_level(const Node* root, const std::string& key)
{
    assert(root);
    std::queue<parent_child_ptrs> q;
    q.push(parent_child_ptrs{ nullptr, root });
    q.push(parent_child_ptrs{nullptr, nullptr});
    int level = 0;
    while(!q.empty())
    {
        parent_child_ptrs top = q.front();
        q.pop();
        if(!top.parent && !top.child)
        {
            if(q.empty()) return node_level{};
            ++level;
            q.push(parent_child_ptrs{nullptr, nullptr});
        }
        else
        {
            if(top.child->name == key)
                return node_level{ top.parent, top.child, level };
            for(Node* child : top.child->subordinates)
                q.push( parent_child_ptrs{ top.child, child });
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
    if(!root || root->name == name) return root;
    for(Node* direct_subordinate : root->subordinates) {
        const Node* find_res = find_rec(direct_subordinate, name);
        if(find_res) return find_res;
    }
    return nullptr;
}

Hierarchy::Node* Hierarchy::find_rec(Node* root, const std::string& name)
{ return const_cast<Node*>(find_rec(const_cast<const Node*>(root), name)); }

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
{ return find_rec(root, key); }

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