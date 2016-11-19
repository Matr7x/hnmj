#ifndef SERVER_MJ_PLAYER_H_
#define SERVER_MJ_PLAYER_H_

#include "mj_logic.h"
#include "i_robot.h"
#include "card_group.h"

#define MAX_PLAYER_CARD_COUNT 14

#define DATA_TYPE_ACCEPT_DEAL                         1
#define DATA_TYPE_CHI_PENG_GANG                       2
#define DATA_TYPE_PLAY_AFTER_CHI_PENG                 3

namespace server {

class MJTable;

class MJPlayer : public IRobot {

public:
  friend class MJTable;

public:
  MJPlayer(int index);
  MJPlayer(int index, long user_id);

public:
  /*int AcceptCards(CardValue cards*, int count);*/
  int AcceptDeal(CardIndex index);
  int RemoveCard(CardValue card);

public:
  virtual bool IsRobot();
  virtual CardValue RobotPlay();                      // 机器人出牌
  virtual int RobotChiPengGang();                     // 机器人吃碰杠

public:
  int SendData(MJTable* table, int data_type);        // 模拟数据
  CardValue Play();                                   // 出牌
  CardValue PlayerPlay();                             // 玩家出牌
  int Play(CardValue);                                // 由框架执行的出牌
  int AddPlayRecord(CardValue card);                  // 添加一条弃牌记录
  void ResetChiPengGang();                            // 重置吃碰杠
  void SetChiPengGang(int chi_peng_gang);             // 设置吃碰杠
  void SetActionCard(int card);                       // 设置吃碰杠牌
  int GetChiPengGang();                               // 获取吃碰杠
  bool HasChiPengGang();                              // 判断是否有吃碰杠
  int ChiPengGang();                                  // 吃碰杠
  int PlayerChiPengGang();                            // 玩家吃碰杠
  int ChiPengGang(int type, CardValue chi_peng_gang_card);
  void PrintCards();
  void PrintChiPengGang();

private:
  void Reset();

  // TODO: To be removed
  void UseUserId() {
    user_id_ = 0;
  }

// TODO: private
public:
  CardIndex cards_index_[MAX_CARDS_INDEX];        // 手牌 
  CardValue accepted_card_;                       // 刚刚接收的发牌
  CardValue play_records_[55];                    // TODO: 最大弃牌数目还需计算一下
  int       play_records_count_;                  // 弃牌数目
  long      user_id_;                             // 用户ID
  int       index_;                               // 下标
  CardValue action_card_;                         // 需要作出反应的牌
  int       chi_peng_gang_;                       // 吃碰杠标识
  CardGroup card_groups_[4];                      // 吃碰杠组合
  int       card_groups_count_;                   // 吃碰杠组合个数
};

}

#endif//SERVER_MJ_PLAYER_H_
