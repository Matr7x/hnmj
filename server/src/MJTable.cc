#include "MJTable.h"

#include <iostream>

using std::cout;
using std::endl;

namespace server {

MJTable::MJTable() {
  Init();
}

void MJTable::Init() {
  ResetCards();
  ResetPlayers();
}

void MJTable::ResetCards() {
  // 复位牌
  memset(cards_, 0, sizeof(cards_));
}

void MJTable::ResetPlayers() {
  // Player设置
  memset(players_, 0, sizeof(players_));
  player_count_ = 0;
  banker_ = 0;
  current_player_ = 0;
}

void MJTable::ResetMultiWait() {
  // 重置Multi Wait
  multi_wait_mode_ = false;  
  for (int i = 0; i < MAX_PLAYERS; i++) {
    multi_wait_status_[i] = false;
  }
}

int MJTable::CreatePlayer(long user_id) {
  // 桌子满，创建失败
  if (IsFull()) {
    return 255;
  }

  // 创建Player
  players_[player_count_] = new MJPlayer(player_count_, user_id);
  player_count_++;
  return 0;
}

int MJTable::CreateRobotPlayer() {
  // 桌子满，创建失败
  if (IsFull()) {
    return 255;
  }

  // 创建机器人玩家
  players_[player_count_] = new MJRobotPlayer(player_count_);
  player_count_++;
  return 0;
}

int MJTable::DestroyPlayer(MJPlayer* player) {
  return 255;
}

int MJTable::GetPlayerCount() {
  return player_count_;
}

int MJTable::DealCards() {
  if (player_count_ < 4) {
    return 255;
  }

  // 洗牌
  MJLogic::Shuffle(cards_, ARR_SIZE(cards_));
  cards_left_ = MAX_CARDS;

  // 发牌数量：每人13张
  const int kCardsToDeal = MAX_PLAYER_CARD_COUNT - 1;

  // 发牌
  for (int i = 0; i < MAX_PLAYERS; i++) {
    cards_left_ -= kCardsToDeal;
    MJLogic::CardsValue2Index(&cards_[cards_left_], kCardsToDeal, players_[i]->cards_index_);
  }

  return 0;
}

int MJTable::DealCardToPlayer(int player) {
  // 荒庄
  if (0 == cards_left_) {
    // 游戏结束
    OnGameOver();
  }

  // 发牌
  CardValue card = cards_[--cards_left_];
  players_[player]->AcceptDeal(card);


  cout << "===== Beg 发牌 =====" << endl;
  cout << "玩家" << player << ": " << MJLogic::kCards2Txt[static_cast<int>(card)] << endl;
  players_[player]->PrintCards();
  cout << "===== End 发牌 =====" << endl;

  // 胡牌判断
  cout << "===== Beg 自摸胡？ =====" << endl;
  if (MJLogic::EstimateHu(players_[player]->cards_index_, card)) {
    players_[player]->PrintCards();
    cout << "玩家" << player << "自摸胡" << endl;
    OnGameOver();
  } else {
    cout << "玩家" << player << "没有自摸胡" << endl;
  }
  cout << "===== End 自摸胡？ =====" << endl;
  
  // TODO: 发送数据
  players_[player]->SendData(this, DATA_TYPE_ACCEPT_DEAL);
  return 0;
}

int MJTable::SetBanker(int banker) {
  banker_ = banker;
  return 0;
}

int MJTable::DealCardToGangPlayer(int player) {
  // 杠用户不可能抓到牌
  
  // 发牌
  CardValue card = cards_[--cards_left_];
  players_[player]->AcceptDeal(card);

  cout << "===== Beg 发牌 =====" << endl;
  cout << "玩家" << player << ": " << MJLogic::kCards2Txt[static_cast<int>(card)] << endl;
  players_[player]->PrintCards();
  cout << "===== End 发牌 =====" << endl;

  // 胡牌判断
  cout << "===== Beg 自摸胡？ =====" << endl;
  if (MJLogic::EstimateHu(players_[player]->cards_index_, card)) {
    cout << "玩家" << player << "自摸胡" << endl;
    OnGameOver();
  } else {
    cout << "玩家" << player << "没有自摸胡" << endl;
  }
  cout << "===== End 自摸胡？ =====" << endl;
  
  // TODO: 发送数据
  players_[player]->SendData(this, DATA_TYPE_ACCEPT_DEAL);
  return 0;
}

int MJTable::SetCurrentPlayer(int player) {
  current_player_ = player;
  return 0;
}

bool MJTable::IsFull() {
  return player_count_ >= 4;
}

int MJTable::OnGameStart() {
  // 每人发13张牌
  int code = DealCards();

  MJLogic::PrintCards(cards_, ARR_SIZE(cards_));

  cout << "Deal Code: " << code << endl;

  // 设置当前玩家为庄家
  SetCurrentPlayer(banker_);

  // TODO: 长沙麻将特有，判断是否小胡
  
  // TODO: 发送数据给客户端
  SendData();
  
  // 发牌
  DealCardToPlayer(current_player_);

  return 0;
}

int MJTable::OnGameOver() {
  return 0;
}

int MJTable::OnCurrentPlayerPlay(CardValue card) {
  players_[current_player_]->Play(card);
  players_[current_player_]->PrintCards();

  // 胡牌判断
  cout << "===== Beg 胡？ =====" << endl;
  for (int i = 0; i < MAX_PLAYERS; i++) {
    if (i != current_player_) {
      if (MJLogic::EstimateHu(players_[i]->cards_index_, card)) {
        cout << "玩家" << i << "胡" << endl;
        OnGameOver();
      } else {
        cout << "玩家" << i << "没有胡" << endl;
      }
    }
  }
  cout << "===== End 胡？ =====" << endl;

  // 判断是否可吃碰杠
  cout << "===== Beg 判断吃碰杠 =====" << endl;
  if (HasChiPengGang(card)) {
    // 设置吃碰杠牌
    chi_peng_gang_card_ = card;
    cout << "===== End 判断吃碰杠 =====" << endl;
    // 设置多人响应等待模式
    SetMultiWaitMode();
    for (int i = 0; i < MAX_PLAYERS; i++) {
      if (i != current_player_) {
        // 给玩家发送吃碰胡信息
        players_[i]->SendData(this, DATA_TYPE_CHI_PENG_GANG);
      }
    }
  } else {
    cout << "没有吃碰杠" << endl;
    cout << "===== End 判断吃碰杠 =====" << endl;
    // 轮转玩家
    CyclePlayer();
    // 发牌
    DealCardToPlayer(current_player_);
  }
  return 0;
}

int MJTable::OnChiPengGang(int player, int action) {
  if (!multi_wait_mode_) {
    return 255;
  }

  multi_wait_status_[player] = true;
  chi_peng_gang_[player] = action;

  bool ok = true;
  for (int i = 0; i < MAX_PLAYERS; i++) {
    if (i == current_player_) continue;

    if (!multi_wait_status_[i]) {
      ok = false;
    }
  }

  if (!ok) {
    return 0;
  }


  cout << "所有玩家均已响应" << endl;

  // 所有玩家均已响应, 处理吃碰杠结果
  for (int i = current_player_; 
      i != current_player_; 
      i = (current_player_ + 1) % MAX_PLAYERS) {
    // 碰
    if (players_[i]->GetChiPengGang() == CPG_TYPE_PENG) {
      players_[i]->ChiPengGang(CPG_TYPE_PENG, chi_peng_gang_card_);
      players_[i]->PrintCards();
      ResetMultiWait();
      SetCurrentPlayer(i);
      // 碰用户出牌
      players_[i]->SendData(this, DATA_TYPE_PLAY_AFTER_CHI_PENG);
      //DealCardToNextPlayer();
      // TODO: 发送吃杠碰结果
      return 0;
    }

    // 杠
    if (players_[i]->GetChiPengGang() == CPG_TYPE_GANG) {
      players_[i]->ChiPengGang(CPG_TYPE_GANG, chi_peng_gang_card_);
      players_[i]->PrintCards();
      ResetMultiWait();
      SetCurrentPlayer(i);
      // 给杠用户发牌
      DealCardToPlayer(i);
      return 0;
    }
  }

  // 吃，只允许下家吃
  int next_player = (current_player_ + 1) % MAX_PLAYERS;
  if (CPG_TYPE_CHI_LEFT ==  players_[next_player]->GetChiPengGang() ||
      CPG_TYPE_CHI_CENTER ==  players_[next_player]->GetChiPengGang() ||
      CPG_TYPE_CHI_RIGHT ==  players_[next_player]->GetChiPengGang()) {
    players_[next_player]->ChiPengGang(chi_peng_gang_[next_player], chi_peng_gang_card_);
    players_[next_player]->PrintCards();
    ResetMultiWait();
    SetCurrentPlayer(next_player);
    // 吃用户出牌
    players_[next_player]->SendData(this, DATA_TYPE_PLAY_AFTER_CHI_PENG);
    // TODO: 发送吃杠碰结果
    return 0;
  }

  // 没人吃胡碰杠，则继续轮转玩家
  CyclePlayer();
  // 发牌
  DealCardToPlayer(current_player_);

  return 0;
}

int MJTable::SendData() {
  cout << "====== Beg 玩家数据 =====" << endl;
  for (int i = 0; i < MAX_PLAYERS; i++) {
    players_[i]->PrintCards();
  }
  cout << "====== End 玩家数据 =====" << endl;

  return 0;
}

int MJTable::CyclePlayer() {
  cout << "====== Beg 轮转玩家=====" << endl;
  current_player_ = (current_player_ + 1) % MAX_PLAYERS;
  cout << "下一个玩家：" << current_player_ << endl;
  cout << "====== Beg 轮转玩家=====" << endl;
  return 0;
}

int MJTable::DealCardToNextPlayer(int player) {
  // 切换到下家
  cout << "====== Beg 切换玩家=====" << endl;
  current_player_ = (player + 1) % MAX_PLAYERS;
  cout << "下一个玩家：" << current_player_ << endl;
  cout << "====== Beg 切换玩家=====" << endl;

  DealCardToPlayer(current_player_);

  return 0;
}

int MJTable::SetMultiWaitMode() {
  ResetMultiWait();
  multi_wait_mode_ = true;
  return 0;
}

bool MJTable::HasChiPengGang(CardValue card) {
  // 返回值
  bool hasChiPengGang = false;

  // 重置吃碰杠类型
  memset(chi_peng_gang_, 0, sizeof(chi_peng_gang_));

  for (int i = 0; i < MAX_PLAYERS; i++) {
    // 自己过掉
    if (i == current_player_) continue;

    players_[i]->ResetChiPengGang();

    // 设置需要响应的牌
    players_[i]->SetActionCard(card);

    // 判断碰
    players_[i]->SetChiPengGang(MJLogic::EstimatePeng(players_[i]->cards_index_, card));
    
    // 判断杠
    if (cards_left_ > 0) {
      players_[i]->SetChiPengGang(MJLogic::EstimateGang(players_[i]->cards_index_, card));
    }

    // 判断下家吃
    if (i == (current_player_ + 1) % MAX_PLAYERS) {
      players_[i]->SetChiPengGang(MJLogic::EstimateChi(players_[i]->cards_index_, card));
    }

    if (players_[i]->HasChiPengGang()) {
      cout << "玩家" << i << "有吃碰杠" << endl;
      hasChiPengGang = true;
    }
  }

  return hasChiPengGang;
}

} // namespace server

