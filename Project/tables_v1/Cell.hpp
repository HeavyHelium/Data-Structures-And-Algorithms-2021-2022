#pragma once
#ifndef TABLES_V1_CELL_HPP
#define TABLES_V1_CELL_HPP
#include <string>

class Cell {
    virtual std::string as_string() const = 0;
    virtual void get_value() const;
    virtual void set_value();

};


#endif //TABLES_V1_CELL_HPP
