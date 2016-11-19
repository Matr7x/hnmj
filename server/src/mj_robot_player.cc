#include "mj_robot_player.h"

namespace server {

MJRobotPlayer::MJRobotPlayer(int index) : MJPlayer(index) {
}

bool MJRobotPlayer::IsRobot() {
  return true;
}

}
