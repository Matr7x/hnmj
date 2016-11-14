#ifndef SERVER_MJ_ROBOT_PLAYER_H_
#define SERVER_MJ_ROBOT_PLAYER_H_

#include "MJPlayer.h"

namespace server {

class MJRobotPlayer : public MJPlayer {

public:
  MJRobotPlayer(int index);

public:
  virtual bool IsRobot();

};

} // namespace server

#endif//SERVER_MJ_ROBOT_PLAYER_H_
