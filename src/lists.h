#include "main.h"
#include "ball.h"
// #include <list>

// using namespace std;

// list <Ball> ball_list;
// list <Firebeam> firebeam_list;
// list <Fireline> fireline_list;
// list <Boomerang> boomerang_list;
// list <Powerup> powerup_list;
// list <WaterBalloon> waterballoon_list;

void add_coin(list <Ball> &l) {
    for(int i = 0 ; i < 1 ; ++i) {
        int random = rand();
        random = -4000 + random % (7999);
        if(random%7 == 0){
            // std::cout << "yes" << j++ << std::endl;
            l.push_back(Ball(screen_center_x + 2*horizontal_float, (float)(random)/1000.0f ,COLOR_FIREYELLOW));
        }
    }
}


