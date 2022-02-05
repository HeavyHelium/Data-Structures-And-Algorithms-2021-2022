#include <iostream>
#include "numeric_value.hpp"
#include <cmath>

std::string numeric_value::print_value() const {
    switch(T) {
        case type::Int : {
            return std::to_string(V.i_val);
        }
        case type::Double : {
            return std::to_string(V.d_val);
        }
    }
}

void numeric_value::increment() {
    switch(T) {
        case type::Int : {
            V.i_val += 1;
            break;
        }
        case type::Double : {
            V.d_val += 1;
            break;
        }
    }
}

void numeric_value::decrement() {
    switch(T) {
        case type::Int : {
            V.i_val -= 1;
            break;
        }
        case type::Double : {
            V.d_val -= 1;
            break;
        }
    }
}

numeric_value& numeric_value::operator+=(const numeric_value& other) {
    switch(T) {
        case Int : {
            switch(other.T) {
                case Int : {
                    V.i_val += other.V.i_val;
                    break;
                }
                case Double : {
                    int temp = V.i_val;
                    V.d_val = temp;
                    V.d_val += other.V.d_val;
                    T = other.T;
                    break;
                }
            }
            break;
        }
        case Double : {
            switch(other.T) {
                case Int : {
                    V.d_val += other.V.i_val;
                    break;
                }
                case(Double) : {
                    V.d_val += other.V.d_val;
                    break;
                }
            }
            break;
        }
    }
    return *this;
}

numeric_value &numeric_value::operator-=(const numeric_value &other) {
    switch(T) {
        case Int : {
            switch(other.T) {
                case Int : {
                    V.i_val -= other.V.i_val;
                    break;
                }
                case Double : {
                    int temp = V.i_val;
                    V.d_val = temp;
                    V.d_val -= other.V.d_val;
                    T = other.T;
                    break;
                }
            }
            break;
        }
        case Double : {
            switch(other.T) {
                case Int : {
                    V.d_val -= other.V.i_val;
                    break;
                }
                case(Double) : {
                    V.d_val -= other.V.d_val;
                    break;
                }
            }
            break;
        }
    }
    return *this;
}

numeric_value &numeric_value::operator*=(const numeric_value &other) {
    switch(T) {
        case Int : {
            switch(other.T) {
                case Int : {
                    V.i_val *= other.V.i_val;
                    break;
                }
                case Double : {
                    int temp = V.i_val;
                    V.d_val = temp;
                    V.d_val *= other.V.d_val;
                    T = other.T;
                    break;
                }
            }
            break;
        }
        case Double : {
            switch(other.T) {
                case Int : {
                    V.d_val *= other.V.i_val;
                    break;
                }
                case(Double) : {
                    V.d_val *= other.V.d_val;
                    break;
                }
            }
            break;
        }
    }
    return *this;
}

numeric_value &numeric_value::operator/=(const numeric_value &other) {
    switch(T) {
        case Int : {
            switch(other.T) {
                case Int : {
                    V.i_val /= other.V.i_val;
                    break;
                }
                case Double : {
                    int temp = V.i_val;
                    V.d_val = temp;
                    V.d_val /= other.V.d_val;
                    T = other.T;
                    break;
                }
            }
            break;
        }
        case Double : {
            switch(other.T) {
                case Int : {
                    V.d_val /= other.V.i_val;
                    break;
                }
                case(Double) : {
                    V.d_val /= other.V.d_val;
                    break;
                }
            }
            break;
        }
    }
    return *this;
}

numeric_value numeric_value::operator==(const numeric_value &other) const {
    bool res = false;
    switch(T) {
        case Int : {
            switch(other.T) {
                case Int : {
                    res = V.i_val == other.V.i_val;
                    break;
                }
                case Double : {
                    res = V.d_val == other.V.d_val;
                    break;
                }
            }
            break;
        }
        case Double : {
            switch(other.T) {
                case Int : {
                    res = V.d_val == other.V.i_val;
                    break;
                }
                case(Double) : {
                    res = V.d_val == other.V.d_val;
                    break;
                }
            }
            break;
        }
    }
    value val;
    val.i_val = res;
    return { type::Int, res };
}

numeric_value numeric_value::operator!=(const numeric_value &other) const {
    bool res = false;
    switch(T) {
        case Int : {
            switch(other.T) {
                case Int : {
                    res = V.i_val != other.V.i_val;
                    break;
                }
                case Double : {
                    res = V.d_val != other.V.d_val;
                    break;
                }
            }
            break;
        }
        case Double : {
            switch(other.T) {
                case Int : {
                    res = V.d_val != other.V.i_val;
                    break;
                }
                case(Double) : {
                    res = V.d_val != other.V.d_val;
                    break;
                }
            }
            break;
        }
    }
    value val;
    val.i_val = res;
    return { type::Int, res };
}

numeric_value numeric_value::operator>(const numeric_value &other) const {
    bool res = false;
    switch(T) {
        case Int : {
            switch(other.T) {
                case Int : {
                    res = V.i_val > other.V.i_val;
                    break;
                }
                case Double : {
                    res = V.d_val > other.V.d_val;
                    break;
                }
            }
            break;
        }
        case Double : {

            switch(other.T) {
                case Int : {
                    res = V.d_val > other.V.i_val;
                    break;
                }
                case(Double) : {
                    res = V.d_val > other.V.d_val;
                    break;
                }
            }
            break;
        }
    }
    value val;
    val.i_val = res;
    return { type::Int, res };
}

numeric_value numeric_value::operator<(const numeric_value &other) const {
    bool res = false;
    switch(T) {
        case Int : {
            switch(other.T) {
                case Int : {
                    res = V.i_val < other.V.i_val;
                    break;
                }
                case Double : {
                    res = V.d_val < other.V.d_val;
                    break;
                }
            }
            break;
        }
        case Double : {
            switch(other.T) {
                case Int : {
                    res = V.d_val < other.V.i_val;
                    break;
                }
                case(Double) : {
                    res = V.d_val < other.V.d_val;
                    break;
                }
            }
            break;
        }
    }
    value val;
    val.i_val = res;
    return { type::Int, res };
}

numeric_value& numeric_value::operator+() {
    return *this;
}
numeric_value& numeric_value::operator-() {
    switch(T) {
        case type::Int: {
            V.i_val *= -1;
            break;
        }
        case type::Double: {
            V.d_val *= -1;
            break;
        }
    }
    return *this;
}

numeric_value numeric_value::operator+(int) {
    return *this;
}
numeric_value numeric_value::operator-(int) {
    numeric_value copy(*this);
    return -copy;
}
numeric_value::operator bool() const {
    switch(T) {
        case Int : {
            return V.i_val;
        }
        case Double : {
            return V.d_val;
        }
    }
}

numeric_value numeric_value::operator!() const {
    numeric_value temp = { type::Int };
    if((bool)*this) {
        temp.V.i_val = 0;
    } else {
        temp.V.i_val = 1;
    }
    return temp;
}

numeric_value numeric_value::sin(const numeric_value &v1) {
    numeric_value res { type::Double };
    switch(v1.T) {
        case type::Int : {
            res.V.d_val = std::sin(v1.V.i_val);
            break;
            }
        case type::Double : {
            res.V.d_val = std::sin(v1.V.d_val);
            break;
        }
    }
    return res;
}

numeric_value numeric_value::cos(const numeric_value &v1) {
    numeric_value res { type::Double };
    switch(v1.T) {
        case type::Int : {
            res.V.d_val = std::cos(v1.V.i_val);
            break;
        }
        case type::Double : {
            res.V.d_val = std::cos(v1.V.d_val);
            break;
        }
    }
    return res;
}

numeric_value numeric_value::pow(const numeric_value &v1, const numeric_value& v2) {
    numeric_value res{ type::Int };
    switch(v1.T) {
        case Int : {
            switch(v2.T) {
                case Int : {
                    res.V.i_val = std::pow(v1.V.i_val, v2.V.i_val);
                    break;
                }
                case Double : {
                    res.T = type::Double;
                    res.V.d_val = std::pow(v1.V.i_val, v2.V.d_val);
                    break;
                }
            }
            break;
        }
        case Double : {
            switch(v2.T) {
                case Int : {
                    res.T = type::Double;
                    res.V.d_val = std::pow(v1.V.d_val, v2.V.i_val);
                    break;
                }
                case(Double) : {
                    res.T = type::Double;
                    res.V.d_val = std::pow(v1.V.d_val, v2.V.d_val);
                    break;
                }
            }
            break;
        }
    }
    return res;
}


