#ifndef SERVER_I_ROBOT_H_
#define SERVER_I_ROBOT_H_

namespace server {

class IRobot {
public:
  virtual bool IsRobot() = 0;
  virtual CardValue RobotPlay() = 0;
  virtual int RobotChiPengGang() = 0;
};

}

#endif//SERVER_I_ROBOT_H_
