#include "room.h"

namespace server{

Room::Room() player_count_(0), status_(0) {
  MJTable* table = new MJTable();
}

Room::~Room() {
  if (null != table) delete table;
  // TODO: delete player
}

int Room::Join(int user_id) {
  if (IsFull()) {
    return 1;
  }
  MJPlayer* player = new MJPlayer(user_id);
  player_map_(user_id) = paleyr;
  player_count_++;
  table->Sit(player);
  return 0;
}

int Room::Leave(int user_id) {
  // 查找Player
  map<int, MJPlayer*>::iterator it = player_map_.find(user_id);

  // 删除Player
  if (it != player_map_.end()) {
    delete it->second;
    player_map_->erase(it);
    return 0;
  }

  // 没找到，返回错误
  return 255;
}

void OnStart(int user_id) {
}

void OnDestory(int user_id) {
  
}

bool Room::IsFull() {
  return 4 == player_count_;
}

} // namespace server
