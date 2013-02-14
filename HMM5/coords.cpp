#include <iostream>
#include "coords.h"

namespace coords {
    std::ostream& operator << (std::ostream& out, const coord& crd) {
        out << '(' <<crd.row << ',' << crd.col << ')';
        return out;
    }
    bool coord::operator == (const coord& crd) {
        return (crd.row == row) && (crd.col == col);
    }
    coord& coord::operator = (const coord& crd) {
        row = crd.row;
        col = crd.col;
        return *this;
    }
    bool compare_cords (coord crd1, coord crd2) {
      if (crd1.row < crd2.row) return true;
      if (crd1.row > crd2.row) return false;
      return (crd1.col < crd2.col);
    }
};
