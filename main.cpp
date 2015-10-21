#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>

#define NUM_GLACIERS 30
#define X   300
#define Y   300
#define NTOTAL 4920154.0

#include "data.h"
//defines densites

int dsquares[X][Y];
double sqrts[(X*X + Y*Y)];

using namespace std;

void init_data() {
    for (int i = 0; i < X; i ++) {
        for (int j = 0; j < Y; j ++) {
            dsquares[i][j] = (i - j) * (i - j);
        }
    }
    for (int i = 0; i < (X*X + Y*Y); i ++) {
        sqrts[i] = sqrt(i);
    }
}

double distance(int x1, int y1, int x2, int y2) {
    return sqrts[dsquares[x1][x2] + dsquares[y1][y2]];
}



inline double rand01() {
    return ((double) rand() / (RAND_MAX));
}

//inclusive
inline int rand_range(int min,int max) {
    return min + (rand() % (int)(max - min + 1));
}

class Representation {
  public:
    double _value;
    int positions[NUM_GLACIERS][2];

    Representation() {
        for(int i = 0; i < NUM_GLACIERS; i ++) {
            positions[i][0] = rand_range(0,X-1);
            positions[i][1] = rand_range(0,Y-1);
        }
        _value = -1.;
    }

    double value() {
        if (_value < 1) {
            _value = 0;
            for (int i = 0; i < X; i ++) {
                for (int j = 0; j < Y; j ++) {
                    double dmin = distance(i,j,positions[0][0],positions[0][1]);
                    for (int k = 1; k < NUM_GLACIERS; k++) {
                        double d = distance(i,j,positions[k][0],positions[k][1]);
                        if (d < dmin) {
                            dmin = d;
                        }
                    }
                    _value += densites[i][j] * dmin;
                }
            }
            _value /= NTOTAL;
        }
        return _value;
    }

    void modified() {
        _value = -1.;
    }

    void mutate_hard() {
        int changing = rand_range(0,NUM_GLACIERS-1);
        positions[changing][0] = rand_range(0,X-1);
        positions[changing][1] = rand_range(0,Y-1);
        modified();
    }

    void mutate_soft() {
        positions[rand_range(0,NUM_GLACIERS-1)][rand_range(0,1)] += rand_range(-1,1);
        modified();
    }

    void copy(Representation &r1) {
        memcpy(positions,r1.positions,sizeof(positions));
        _value = r1.value();
    }

    void fromParents(Representation *r1,Representation *r2) {
        modified();
        int b1 = rand_range(0,NUM_GLACIERS/2);
        int b2 = rand_range(b1,NUM_GLACIERS-1);
        for (int i = 0; i < b1; i ++) {
            positions[i][0] = r1->positions[i][0];
            positions[i][1] = r1->positions[i][1];
        }
        for (int i = b1; i < b2; i ++) {
            positions[i][0] = r2->positions[i][0];
            positions[i][1] = r2->positions[i][1];
        }
        for (int i = b2; i < NUM_GLACIERS; i ++) {
            positions[i][0] = r1->positions[i][0];
            positions[i][1] = r1->positions[i][1];
        }
    }

};

bool repr_sorter(Representation& lhs,Representation& rhs) {
    return lhs.value() < rhs.value();
}

void print_values(const Representation &r) {
    cout << '{';
    for (int i = 0; i < NUM_GLACIERS; i ++) {
        cout << '{' <<  r.positions[i][0] << ',' << r.positions[i][1] << "},";
    }
}

#define GEN_SIZE 512
#define NUM_GEN 5000
#define MUT1_PROBA 0.01
#define MUT2_PROBA 0.1

#define MIN(A,B) (A <= B ? A : B)

int main()
{
    srand(time(0));
    init_data();
    std::vector<Representation> generation;
    for (int i = 0; i < GEN_SIZE; i ++) {
        generation.push_back(Representation());
    }

    sort(generation.begin(),generation.end(),repr_sorter);
    cout << "Best at initial generation: value " << generation[0].value() << endl;

    for (int g = 0; g < NUM_GEN; g ++) {
        //selection
        int num_updated = GEN_SIZE / 4;
        for (int i = GEN_SIZE / 4; i < (3*GEN_SIZE) / 4; i ++) {
            if (rand01() > 2 * ((3*GEN_SIZE)/4 - i) / GEN_SIZE) {
                generation[num_updated].copy(generation[i]);
                num_updated ++;
            }
        }
        int kept = num_updated-1;
        // crossover
        while (num_updated < GEN_SIZE) {
            int r1 = rand_range(0,kept);
            int r2 = rand_range(0,kept);
            generation[num_updated].fromParents(&generation[r1],&generation[r2]);
            num_updated ++;
        }
        // mutation
        for (int i = 0; i < GEN_SIZE; i ++) {
            if (rand01() < MUT1_PROBA) {
                generation[i].mutate_hard();
            }
            if (rand01() < MUT2_PROBA) {
                generation[i].mutate_soft();
            }
        }
        // evaluation
        sort(generation.begin(),generation.end(),repr_sorter);
        cout << "Best at generation " << g << " : value " << generation[0].value() << endl;
    }
    cout << "Final best: " << endl;
    for (int i = 0; i < NUM_GLACIERS; i ++) {
        cout << generation[0].positions[i][1] << "," << generation[0].positions[i][0] << endl;
    }

    return 0;
}

