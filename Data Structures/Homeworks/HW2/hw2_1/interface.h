#pragma once
#include <string>
#include "tree.h"
#include "data_parser.h"

using std::string;

class Hierarchy
{
public:
    Hierarchy(Hierarchy&& r) noexcept;
    Hierarchy(const Hierarchy& r);
    Hierarchy(const string& data);
    ~Hierarchy() noexcept;
    void operator=(const Hierarchy&) = delete;

    string print() const;

    int longest_chain() const;
    bool find(const string& name) const;
    int num_employees() const;
    int num_overloaded(int level = 20) const;

    string manager(const string& name) const;
    int num_subordinates(const string& name) const;
    unsigned long getSalary(const string& who) const;

    bool fire(const string& who);
    bool hire(const string& who, const string& boss);

    void incorporate();
    void modernize();

    Hierarchy join(const Hierarchy& right) const;
    //If you need it - add more public methods here
private:
    tree<std::string> branch;
};

Hierarchy::Hierarchy(Hierarchy&& r) noexcept
    : branch(std::move(r.branch))
{}

Hierarchy::Hierarchy(const Hierarchy& r)
    : branch(r.branch)
{}

Hierarchy::Hierarchy(const string& data)
    : branch("Uspeshnia")
{
    data_parser parser;
    parser.parse_text(data);
    for(const auto& pair : parser.pairs())
    {
        if(branch.find(pair.manager)) {
            branch.insert_as_child(pair.manager, pair.subordinate);
        }
        else
            throw std::runtime_error("no such manager is employed in current branch");
    }

}

Hierarchy::~Hierarchy() noexcept
{}

string Hierarchy::print() const
{ return branch.print(); }

int Hierarchy::longest_chain() const
{
    return 0;
}
bool Hierarchy::find(const string& name) const
{
    return branch.find(name);
}
int Hierarchy::num_employees() const
{
    return branch.size();
}
int Hierarchy::num_overloaded(int level) const
{
    return 0;
}
string Hierarchy::manager(const string& name) const
{
    //std::cout << branch.size() << std::endl;
    if(name == "Uspeshnia")
        throw std::logic_error("how dare you, he is his own man!");
    if(!branch.find(name))
        throw std::runtime_error(name + " is not employed in current branch.");
    return branch.get_parent_key(name);
}
int Hierarchy::num_subordinates(const string& name) const
{
    return 0;
}
unsigned long Hierarchy::getSalary(const string& who) const
{
    return 0;
}

bool Hierarchy::fire(const string& who)
{
    return 0;
}
bool Hierarchy::hire(const string& who, const string& boss)
{
    return 0;
}

void Hierarchy::incorporate()
{

}
void Hierarchy::modernize()
{

}

Hierarchy Hierarchy::join(const Hierarchy& right) const
{
    return Hierarchy("");
}