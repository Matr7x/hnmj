#include "MJRobotPlayer.h"

namespace server {

MJRobotPlayer::MJRobotPlayer(int index) : MJPlayer(index) {
}

bool MJRobotPlayer::IsRobot() {
  return true;
}

}
