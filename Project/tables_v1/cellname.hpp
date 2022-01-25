#pragma once
#ifndef TABLES_V1_CELLNAME_HPP
#define TABLES_V1_CELLNAME_HPP
#include <cstddef>
#include <string>

class cellname {
protected:
    int m_column;
    int m_row;
public:
    cellname() = default;
    cellname(int r, int c);
    virtual int row() const = 0;
    virtual int column() const = 0;
};

class absolute_cellname : public cellname {
public:
  absolute_cellname(const std::string& text);
  absolute_cellname(int r, int c);
  int row() const override;
  int column() const override;
  std::string to_name() const;

};

class relative_cellname : public cellname {
public:
    relative_cellname(const std::string& text);
    int row() const override;
    int column() const override;
};


#endif //TABLES_V1_CELLNAME_HPP
