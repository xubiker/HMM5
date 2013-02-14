#pragma once

namespace coords {
    class coord {
        private:
            //
        public:
            int row, col;
            coord (int r, int c) : row(r), col(c) {};
            coord () : row(-1), col(-1) {};
            bool operator == (const coord& crd);
            coord& operator = (const coord& crd);
            friend std::ostream& operator << (std::ostream& out, const coord& crd);
            ~coord() {};
    };
    bool compare_cords (coord, coord);
};
