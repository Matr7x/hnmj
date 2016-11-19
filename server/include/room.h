#ifndef SERVER_ROOM_H_
#define SERVER_ROOM_H_

#include "mj_player.h"

#include <map>

using std::map;

namespace server {

class Room {

public:
  Room();                                                 // 构造函数
  ~Room();                                                // 析构函数

public:
  int Join(int user_id);                                  // 加入房间
  int Leave(int user_id);                                 // 离开房间
  bool IsFull();                                          // 房间是否满了
  void OnStart(int user_id);                              // 游戏开始事件
  void OnDestroy(int user_id);                            // 解散房间事件

private:
  // 房间号
  int                 room_num_;
  // 用户Map，key为用户ID，value为MJPlayer指针
  map<int, MJPlayer*> player_map_;
  // 用户数，最多为4
  int                 player_count_;
  // 房间状态：等待加入，游戏前，游戏中状态，游戏结束
  int                 status_;
  // 桌子类
  MJTable*            table_;

  // 局数控制
  int                 current_round_;
  // 总局数
  int                 round_num_;

  // 游戏开始状态
  map<int, bool>      start_status_;

  // 房间解散状态
  map<int, bool>      destory_status_;
};
  
}

#endif//SERVER_ROOM_H_
