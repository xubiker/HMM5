#include <algorithm>
#include <cmath>
#include "fields.h"
#include "generators.h"

namespace fields {
    enum {
        DISTANCE = 1,
        PATH = 2,
        ENEMY = 3
    };

    Field::Field (unsigned r, unsigned c) : rows(r), cols(c), max_radius(0), matrix(0), wave_matrix(0) {
        matrix = new shared_ptr<units::ProxyUnit>* [rows];
        for (unsigned i = 0; i < rows; i++) {
            matrix[i] = new shared_ptr<units::ProxyUnit> [cols];
        }
        wave_matrix = new int* [DEF_FIELD_ROWS];
        for (unsigned i = 0; i < DEF_FIELD_ROWS; i++) {
            wave_matrix[i] = new int[DEF_FIELD_COLS];
        }
        max_radius = unsigned(sqrt(double(rows*rows+cols*cols+1)));
        cout << "Field " << rows << "x" << cols << " created" << endl;
    }
    void Field::PrintField (void) {
        for (unsigned i = 0; i < rows; i++) {
            for (unsigned j = 0; j < cols; j++) {
                cout << matrix[i][j] << " ";
            }
            cout << endl;
        }
    }
    void Field::PrintWave (void) {
        for (unsigned i = 0; i < rows; i++) {
            for (unsigned j = 0; j < cols; j++) {
                cout << wave_matrix[i][j] << " ";
            }
            cout << endl;
        }
    }
    unsigned Field::get_rows (void) { return rows; }
    unsigned Field::get_cols (void) { return cols; }
    shared_ptr<units::ProxyUnit> Field::get_cell (coords::coord crd) {
        return matrix[crd.row][crd.col];
    }
    bool Field::invalid_cell (coords::coord crd) {
        if (crd.col < 0 || crd.col >= int(cols) || crd.row < 0 || crd.row >= int(rows)) {
            return true;
        }
        return false;
    }
    void Field::set_cell (coords::coord crd, shared_ptr<units::ProxyUnit> pu) {
        matrix[crd.row][crd.col] = pu;
    }
    void Field::reset_cell (coords::coord crd) {
        matrix[crd.row][crd.col] = 0;
    }
    coords::coord Field::get_new_cell (void) {
        coords::coord crd;
        shared_ptr<generators::RandomGen> rgen = generators::RandomGen::Instance();
        for (unsigned i = 0; i < rows * cols; i++) {
            crd.row = rgen->get_random(0, rows);
            crd.col = rgen->get_random(0, cols);
            if (is_empty(crd)) {
                return crd;
            }
        }
        for (unsigned i = 0; i < rows; i++) {
            for (unsigned j = 0; j < cols; j++) {
                crd.row = i;
                crd.col = j;
                if (is_empty(crd)) {
                   return crd;
                }
            }
        }
        return coords::coord();
    }
    bool Field::is_empty (coords::coord crd) {
        if (matrix[crd.row][crd.col] == 0) return true;
        if (matrix[crd.row][crd.col]->IsDead()) return true;
        return false;
    }
    vector<coords::coord> Field::get_neighbor (coords::coord crd) {
        vector<coords::coord> res;
        coords::coord tmp;
        if (crd.row < 0 || crd.row >= DEF_FIELD_ROWS) { throw "out of field"; }
        if (crd.col < 0 || crd.col >= DEF_FIELD_COLS) { throw "out of field"; }

        tmp.row = crd.row;
        tmp.col = crd.col - 1;
        res.push_back(tmp);
        tmp.col = crd.col + 1;
        res.push_back(tmp);

        tmp.col = crd.col - crd.row % 2;
        tmp.row = crd.row - 1;
        res.push_back(tmp);
        tmp.row = crd.row + 1;
        res.push_back(tmp);

        tmp.col = crd.col + (crd.row + 1) % 2;
        tmp.row = crd.row - 1;
        res.push_back(tmp);
        tmp.row = crd.row + 1;
        res.push_back(tmp);

        for (unsigned i = 0; i < res.size(); i++) {
            if (res[i].row < 0 || res[i].row >= DEF_FIELD_ROWS ||
                res[i].col < 0 || res[i].col >= DEF_FIELD_COLS) {
                    res.erase(res.begin() + i--);
            }
        }
        stable_sort (res.begin(), res.end(), coords::compare_cords);
        return res;
    }
    vector< vector<coords::coord> > Field::wave (coords::coord crd, unsigned radius, int type) {
        vector< vector<coords::coord> > _wave;
        vector<coords::coord> init;
        // reset the "used" matrix
        for (unsigned i = 0; i < DEF_FIELD_ROWS; i++)
            for (unsigned j = 0; j < DEF_FIELD_COLS; j++)
                wave_matrix[i][j] = -1;
        init.push_back(crd);
        _wave.push_back(init);
        wave_matrix[crd.row][crd.col] = 0;
        // create a path wave up to the radius value
        for (unsigned r = 0; r < radius; r++) {
            bool any_new = false;
            // create (r+1) wave iteration from (r) wave iteration
            for (unsigned i = 0; i < _wave[r].size(); i++) {
                vector<coords::coord> neig = get_neighbor(_wave[r][i]);
                for (unsigned k = 0; k < neig.size(); k++) {
                    coords::coord possible = neig[k];
                    if (wave_matrix[possible.row][possible.col] == -1) {
                        wave_matrix[possible.row][possible.col] = r + 1;
                        if ((type == PATH) && (!is_empty(possible))) { continue; }
                        any_new = true;
                        if (_wave.size() == r + 1) {
                            vector<coords::coord> new_vctr;
                            new_vctr.push_back(possible);
                            _wave.push_back(new_vctr);
                        } else {
                            _wave[r+1].push_back(possible);
                        }
                    }
                }
            }
            if (!any_new) { break; }
            stable_sort (_wave[r+1].begin(), _wave[r+1].end(), coords::compare_cords);
        }
        return _wave;
    }
    vector<coords::coord> Field::get_path (shared_ptr<units::ProxyUnit> p, coords::coord crd, string type) {
        // check if the path exists
        vector<coords::coord> res;
        vector< vector<coords::coord> > vwave;
        bool path_exists = false;
        unsigned rad_success = 0;
        vwave = wave (p->GetLocation(), p->GetSpeed(), PATH);
        if (wave_matrix[crd.row][crd.col] != -1) {
            rad_success = unsigned (wave_matrix[crd.row][crd.col]);
            path_exists = true;
        }
        if (!path_exists) {
            if (type == "direct") {
                cout << "there is no direct path" << endl;
                return res;
            } else if (type == "nearest") {
                unsigned min_dist = -1;
                coords::coord new_aim;
                for (unsigned i = 1; i < vwave.size(); i++) {
                    for (unsigned j = 0; j < vwave[i].size(); j++) {
                        if (get_distance(vwave[i][j], crd) < min_dist) {
                            new_aim = vwave[i][j];
                            rad_success = i;
                            break;
                        }
                    }
                }
                crd = new_aim;
            }
        }
        res.push_back(crd);
        coords::coord _crd = crd;
        for (unsigned k = rad_success; k > 0; k--) {
            vector<coords::coord> neig = get_neighbor(_crd);
            for (unsigned j = 0; j < neig.size(); j++) {
                bool break_flg = false;
                for (unsigned i = 0; i < vwave[k-1].size(); i++) {
                    if (vwave[k-1][i] == neig[j]) {
                        _crd = neig[j];
                        res.push_back(_crd);
                        break_flg = true;
                        break;
                    }
                }
                if (break_flg) { break; }
            }
        }
        // reverse the result vector
        for (unsigned i = 0; i < res.size() / 2; i++) {
            swap(res[i], res[res.size()-i-1]);
        }
        return res;
    }
    vector<coords::coord> Field::best_path (shared_ptr<units::ProxyUnit> p, coords::coord crd, string type) {
        if (p->GetLocation() == crd) {
            cout << "the begin is actually the end if the pass" << endl;
            return vector<coords::coord>();
        }
        if (type == "direct") {
            return get_path(p, crd, "direct");
        } else if (type == "adjacent") {
            // backup the aim cell unit
            shared_ptr<units::ProxyUnit> backup = get_cell(crd);
            reset_cell(crd);
            vector<coords::coord> res = get_path(p, crd, "direct");
            set_cell(crd, backup);
            if (res.size() > 0) {
                res.pop_back();
            }
            return res;
        } else if (type == "nearest") {
            // backup the aim cell unit
            shared_ptr<units::ProxyUnit> backup = get_cell(crd);
            reset_cell(crd);
            vector<coords::coord> res = get_path(p, crd, "nearest");
            set_cell(crd, backup);
            if ((res.size() > 0) && (res.back() == crd)) {
                res.pop_back();
            }
            return res;
        } else {
            cout << "wrong parameter for calc_path function" << endl;
            return vector<coords::coord>();
        }
    }
    unsigned Field::get_distance (coords::coord crd1, coords::coord crd2) {
        unsigned d =  abs(crd1.row - crd2.row);
        int right = crd1.col + (d + (crd1.row + 1) % 2) / 2;
        int left = right - d;
        if (crd2.col < left)  { return unsigned (d + left - crd2.col);  }
        if (crd2.col > right) { return unsigned (d + crd2.col - right); }
        return d;
    }
    Field::~Field () {
        for (unsigned i = 0; i < DEF_FIELD_ROWS; i++) {
            delete [] wave_matrix[i];
        }
        delete wave_matrix;
        for (unsigned i = 0; i < DEF_FIELD_ROWS; i++) {
            delete [] matrix[i];
        }
        delete matrix;
        cout << "Field deleted" << endl;
    }
}
