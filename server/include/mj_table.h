#ifndef SERVER_MJ_TABLE_H_
#define SERVER_MJ_TABLE_H_

#include "mj_logic.h"
#include "mj_player.h"
#include "mj_robot_player.h"

#define MAX_PLAYERS 4

namespace server {
 
class MJTable {
public:
  MJTable();

public:
  // 玩家坐下
  int Sit(MJPlayer* player);
  int CreatePlayer(long user_id);
  int CreateRobotPlayer();
  int DestroyPlayer(MJPlayer* player);
  int DestoryAllPalyers();
  int GetPlayerCount();
  int DealCards();
  int DealCardToPlayer(int player);
  int DealCardToGangPlayer(int player);
  int SetBanker(int banker);
  int SetCurrentPlayer(int player);
  int CyclePlayer();
  int SetMultiWaitMode();
  int DealCardToNextPlayer(int player);

private:
  void Init();
  void ResetCards();
  void ResetPlayers();
  void ResetMultiWait();

  bool IsFull();

public:
  int OnGameStart();
  int OnGameOver();
  int OnCurrentPlayerPlay(CardValue card);  // 当前用户出牌行为
  int OnActionPlayerPlay(CardValue card);   // 吃碰杠用户出牌行为
  int OnChiPengGang(int player, int action);// 吃碰杠行为

  int SendData();                           // 发送数据给客户端

  bool HasChiPengGang(CardValue card);                    // 判断是否有吃碰胡

// TODO: private
public:
  CardValue cards_[MAX_CARDS];
  MJPlayer* players_[MAX_PLAYERS];
  int       player_count_;                  // 玩家数
  int       cards_left_;                    // 当前剩余牌数
  int       banker_;                        // 庄家
  int       current_player_;                // 当前出牌玩家
  bool      multi_wait_mode_;               // 多人等待模式
  bool      multi_wait_status_[MAX_PLAYERS];// 多人等待状态
  int       chi_peng_gang_[MAX_PLAYERS];    // 吃碰杠胡操作
  CardValue chi_peng_gang_card_;            // 吃碰杠对应的牌


};

}

#endif//SERVER_MJ_TABLE_H_
