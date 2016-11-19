#ifndef SERVER_ROOM_MANAGER_H_
#define SERVER_ROOM_MANAGER_H_

#include <map>

namespace server {

class RoomManager {

public:
  int CreateRoom(int& room_num);                            // 创建房间
  int JoinRoom(int room_num, long user_id);                 // 加入房间
  int LeaveRoom(int room_num, long user_id);                // 离开房间
  int DestroyRoom(int room_num);                            // 销毁房间房间

private:
  map<int, Room*> room_map_;                                // 房间列表
  
};
  
}

#endif//SERVER_ROOM_MANAGER_H_
