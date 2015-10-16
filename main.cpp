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

double distance(double x1,int y1,int x2,int y2) {
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
                    double dmin = distance(i,j,positions[0][0],positions[0][0]);
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
        int changing = rand_range(0,NUM_GLACIERS);
        positions[changing][0] = rand_range(0,X-1);
        positions[changing][1] = rand_range(0,Y-1);
        modified();
    }

    void mutate_soft() {
        positions[rand_range(0,NUM_GLACIERS)][rand_range(0,1)] += rand_range(-1,1);
        modified();
    }

    void copy(Representation &r1) {
        memcpy(positions,r1.positions,sizeof(positions));
        _value = r1.value();
    }

    void fromParents(Representation *r1,Representation *r2) {
        modified();
        int b1 = rand_range(0,NUM_GLACIERS/2);
        int b2 = rand_range(b1,NUM_GLACIERS);
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
#define NUM_GEN 1000
#define MUT1_PROBA 0.01
#define MUT2_PROBA 0.1

#define MIN(A,B) (A <= B ? A : B)

int s1[30][2] = {{141,21},{124,33},{53,107},{254,287},{45,18},{33,285},{203,231},{285,98},{263,172},{96,182},{26,61},{13,237},{83,23},{200,269},{271,141},{229,113},{125,236},{111,73},{12,110},{103,116},{187,182},{250,27},{292,236},{78,266},{191,44},{17,146},{153,93},{171,242},{47,202},{69,136}};
int s2[30][2] = {{141,21},{133,30},{199,76},{267,283},{42,22},{31,282},{206,231},{277,86},{264,168},{99,176},{38,67},{24,239},{87,19},{200,274},{279,137},{233,122},{113,228},{84,70},{19,107},{104,114},{196,180},{273,27},{289,235},{83,271},{195,24},{19,153},{152,85},{163,221},{49,202},{66,134}};
int s3[30][2] = {{261,47},{124,32},{53,111},{253,290},{46,18},{110,145},{203,80},{291,95},{260,175},{96,182},{24,59},{7,236},{82,23},{198,270},{271,140},{191,272},{94,124},{82,295},{123,80},{158,55},{184,181},{189,183},{294,236},{78,266},{190,46},{12,148},{38,291},{170,242},{46,203},{70,134}};
int s4[30][2] = {{216,134},{267,75},{147,60},{202,271},{205,59},{239,135},{61,198},{110,229},{135,141},{288,220},{105,174},{54,68},{195,18},{197,175},{277,159},{24,160},{112,99},{84,23},{271,280},{22,102},{83,274},{163,224},{126,21},{27,232},{282,20},{73,130},{281,112},{177,98},{35,26},{28,279}};
int s5[30][2] = {{140,20},{166,23},{186,85},{271,280},{43,22},{28,279},{203,232},{258,74},{278,157},{106,171},{31,77},{28,233},{104,20},{202,277},{282,105},{239,136},{109,229},{71,58},{21,121},{108,106},{201,179},{283,21},{287,219},{83,274},{208,27},{27,172},{139,67},{157,222},{65,200},{70,131}};
int s6[30][2] = {{262,46},{124,33},{53,108},{253,287},{45,18},{33,285},{203,233},{287,99},{263,171},{96,182},{25,61},{12,237},{83,23},{200,268},{271,140},{228,113},{127,236},{80,295},{290,297},{103,117},{185,181},{247,26},{293,236},{78,266},{191,45},{17,146},{152,94},{176,242},{47,203},{69,134}};
int s7[30][2] = {{140,22},{156,26},{187,84},{271,280},{42,22},{28,279},{203,232},{260,73},{277,157},{106,171},{32,76},{28,233},{101,20},{202,277},{282,107},{238,135},{109,229},{71,58},{21,119},{104,109},{201,179},{283,20},{287,219},{84,274},{205,25},{26,172},{137,74},{157,222},{65,199},{68,133}};

int main()
{
    srand(time(0));
    init_data();
    std::vector<Representation> generation;
    for (int i = 0; i < GEN_SIZE; i ++) {
        generation.push_back(Representation());
    }
/*    memcpy(generation[0].positions,s1,sizeof(s1));
    memcpy(generation[1].positions,s2,sizeof(s1));
    memcpy(generation[2].positions,s3,sizeof(s1));
    memcpy(generation[3].positions,s4,sizeof(s1));
    memcpy(generation[4].positions,s5,sizeof(s1));
    memcpy(generation[5].positions,s6,sizeof(s1));
    memcpy(generation[6].positions,s7,sizeof(s1));
*/
    sort(generation.begin(),generation.end(),repr_sorter);
    cout << "Best at initial generation: value " << generation[0].value() << " data "; print_values(generation[0]); cout << endl;

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

