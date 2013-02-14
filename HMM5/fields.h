#pragma once

#include <memory>
#include <vector>
#include <string>
#include "coords.h"
#include "units.h"

namespace fields {
    class Field {
        private:
            unsigned rows, cols, max_radius;
            shared_ptr<units::ProxyUnit> ** matrix;
            int** wave_matrix;
        public:
            Field (unsigned, unsigned);
#ifdef ejudge_compile
            Field (const Field&) = delete;
            Field& operator = (const Field&) = delete;
#endif
            void PrintField (void);
            void PrintWave (void);
            unsigned get_rows (void);
            unsigned get_cols (void);
            shared_ptr<units::ProxyUnit> get_cell (coords::coord);
            bool invalid_cell (coords::coord);
            bool is_empty (coords::coord);
            void set_cell (coords::coord, shared_ptr<units::ProxyUnit>);
            void reset_cell (coords::coord);
            coords::coord get_new_cell (void);
            vector<coords::coord> get_neighbor (coords::coord);
            vector< vector<coords::coord> > wave (coords::coord, unsigned r, int type);
            vector<coords::coord> get_path (shared_ptr<units::ProxyUnit>, coords::coord, string type);
            vector<coords::coord> best_path (shared_ptr<units::ProxyUnit>, coords::coord, string type);
            unsigned get_distance (coords::coord, coords::coord);
            ~Field ();
    };
};


