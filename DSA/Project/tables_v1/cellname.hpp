#pragma once
#ifndef TABLES_V1_CELLNAME_HPP
#define TABLES_V1_CELLNAME_HPP
#include <cstddef>
#include <string>

/// an abstract class to represent cellnames
class cellname {
protected:
    int m_column;
    int m_row;
public:
    cellname() = default;
    cellname(int r, int c);
    virtual int row() const = 0;
    virtual int column() const = 0;
    ///@return a stringified representation of the cellname
    virtual std::string to_name() const = 0;
    virtual ~cellname() = default;
};

class absolute_cellname : public cellname {
public:
    ///@brief can be constructed with a string value
    ///@throws when text is not a valid representation of an absolute cellname
    absolute_cellname(const std::string& text);
    ///@brief can be constructed with coordinates
    ///@throws when r or c is negative
    absolute_cellname(int r, int c);
    int row() const override;
    int column() const override;
    std::string to_name() const override;
    bool operator==(const absolute_cellname& other) const;
};

class relative_cellname : public cellname {
public:
    ///@brief can be constructed with a string value
    ///@throws when text is not a valid representation of a relative cellname
    relative_cellname(const std::string& text);
    ///@brief can be constructed with coordinates
    relative_cellname(int r, int c);
    int row() const override;
    int column() const override;
    std::string to_name() const override;
};


#endif //TABLES_V1_CELLNAME_HPP
