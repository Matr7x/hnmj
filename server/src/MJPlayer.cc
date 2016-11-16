#include "MJPlayer.h"

#include "MJTable.h"

#include <iostream>
#include <string.h>

using std::cin;
using std::cout;
using std::endl;

namespace server {

MJPlayer::MJPlayer(int index) : user_id_(0), index_(index) {
  Reset();
}
  
MJPlayer::MJPlayer(int index, long user_id) : user_id_(user_id), index_(index) {
  Reset();
}

//int MJPlayer::AcceptCards(CardValue cards*, int count) {
  //if (count > MAX_PLAYER_CARD_COUNT) {
    //return 255;
  //}

  //memcpy(cards_index_, cards, sizeof(CardValue) * count);
  //return 0;
//}

void MJPlayer::Reset() {
  // 复位手牌
  memset(cards_index_, 0, sizeof(cards_index_));
  accepted_card_ = 0;
  memset(play_records_, 0, sizeof(play_records_));
  play_records_count_ = 0;
  chi_peng_gang_ = CPG_TYPE_NULL;
  memset(card_groups_, 0, sizeof(card_groups_));
  card_groups_count_ = 0;
}

bool MJPlayer::IsRobot() {
  return false;
}

CardValue MJPlayer::RobotPlay() {
  if (0 != accepted_card_) {
    return accepted_card_;
  } else {
    for (int i = 0; i < MAX_CARDS_INDEX; i++) {
      if (cards_index_[i] > 0) {
        return MJLogic::CardIndex2Value(cards_index_[i]);
      }
    }
  }
  // TODO: 这里需要设置一个无效的牌
  return 0;
}

int MJPlayer::RobotChiPengGang() {
  if (chi_peng_gang_ & CPG_TYPE_PENG) return CPG_TYPE_PENG;
  if (chi_peng_gang_ & CPG_TYPE_GANG) return CPG_TYPE_GANG;
  if (chi_peng_gang_ & CPG_TYPE_CHI_LEFT) return CPG_TYPE_CHI_LEFT;
  if (chi_peng_gang_ & CPG_TYPE_CHI_CENTER) return CPG_TYPE_CHI_CENTER;
  if (chi_peng_gang_ & CPG_TYPE_CHI_RIGHT) return CPG_TYPE_CHI_RIGHT;
  return CPG_TYPE_NULL;
}

int MJPlayer::AcceptDeal(CardValue card) {
  accepted_card_ = card;
  return 0;
}

int MJPlayer::RemoveCard(CardValue card) {
  // TODO: Check if card is valid
  cout << "remove card" << endl;
  cout << static_cast<int>(MJLogic::CardValue2Index(card)) << endl;
  cout << cards_index_[static_cast<int>(MJLogic::CardValue2Index(card))] << endl;
  cards_index_[static_cast<int>(MJLogic::CardValue2Index(card))]--;
  return 0;
}

int MJPlayer::SendData(MJTable* table, int data_type) {
  switch (data_type) {
    case DATA_TYPE_ACCEPT_DEAL: {
      cout << "===== Beg 出牌 =====" << endl;
      cout << "玩家" << index_ << "出牌:";
      CardValue card = Play();
      cout << MJLogic::kCards2Txt[static_cast<int>(card)] << endl;
      cout << "===== End 出牌 =====" << endl;
      table->OnCurrentPlayerPlay(card);
      break;
    }
    case DATA_TYPE_PLAY_AFTER_CHI_PENG: {
      cout << "===== Beg 吃碰后出牌 =====" << endl;
      cout << "玩家" << index_ << "出牌:";
      CardValue card = Play();
      cout << MJLogic::kCards2Txt[static_cast<int>(card)] << endl;
      cout << "===== End 吃碰后出牌 =====" << endl;
      table->OnCurrentPlayerPlay(card);
      break;
    }
    case DATA_TYPE_CHI_PENG_GANG: {
      cout << "===== Beg 吃碰杠 =====" << endl;
      int action = ChiPengGang();
      cout << "玩家" << index_ << "吃碰杠:";
      cout << action << endl;
      cout << "===== End 吃碰杠 =====" << endl;
      table->OnChiPengGang(index_, action);
      break;
    }
    default:
      break;
  }
  return 0;
}

CardValue MJPlayer::Play() {
  // 出牌
  if (IsRobot()) {
    return RobotPlay();
  } else {
    return PlayerPlay();
  }
}

CardValue MJPlayer::PlayerPlay() {
  PrintCards();
  cout << "请出牌：" << endl;
  int idx = -1;
  do {
    cin >> idx;
    if (idx < 0 || idx > MAX_CARDS_INDEX) {
      cout << "请输入正确的牌下标" << endl;;
    }
    if (accepted_card_ != MJLogic::CardIndex2Value(idx) && 
        cards_index_[idx] <= 0) {
      cout << "您没有" << MJLogic::kCardsIndex2Txt[idx] << endl;
    }
  }
  while (idx < 0 || 
      idx > MAX_CARDS_INDEX || 
      (cards_index_[idx] <= 0 && accepted_card_ != MJLogic::CardIndex2Value(idx)));
  return MJLogic::CardIndex2Value(idx);
}

void MJPlayer::PrintCards() {
  if (0 != accepted_card_) {
    cards_index_[static_cast<int>(MJLogic::CardValue2Index(accepted_card_))]++;
  }
  cout << "玩家" << index_ << ":" << endl;
  MJLogic::PrintCardsWithIndex(cards_index_);
  if (0 != accepted_card_) {
    cards_index_[static_cast<int>(MJLogic::CardValue2Index(accepted_card_))]--;
  }
  cout << endl;
}

void MJPlayer::PrintChiPengGang() {
  if (chi_peng_gang_ & CPG_TYPE_PENG) {
    cout << "碰-1 ";
    for (int i = 0; i < 3; i++) {
      cout << MJLogic::kCards2Txt[static_cast<int>(action_card_)] << "|";
    }
    cout << endl;
  }

  if (chi_peng_gang_ & CPG_TYPE_GANG) {
    cout << "杠-2 ";
    for (int i = 0; i < 4; i++) {
      cout << MJLogic::kCards2Txt[static_cast<int>(action_card_)] << "|";
    }
    cout << endl;
  }

  if (chi_peng_gang_ & CPG_TYPE_CHI_LEFT) {
    cout << "吃-3 ";
    for (int i = 0; i < 3; i++) {
      cout << MJLogic::kCards2Txt[static_cast<int>(action_card_)] << "|";
    }
    cout << endl;
  }

  if (chi_peng_gang_ & CPG_TYPE_CHI_CENTER) {
    cout << "吃-4 ";
    for (int i = 0; i < 3; i++) {
      cout << MJLogic::kCards2Txt[action_card_ + i - 1] << "|";
    }
    cout << endl;
  }

  if (chi_peng_gang_ & CPG_TYPE_CHI_RIGHT) {
    cout << "吃-5 ";
    for (int i = 0; i < 3; i++) {
      cout << MJLogic::kCards2Txt[action_card_ + i - 2] << "|";
    }
    cout << endl;
  }
}

int MJPlayer::ChiPengGang() {
  // 吃碰杠
  if (IsRobot()) {
    return RobotChiPengGang();
  } else {
    return PlayerChiPengGang();
  }
}

int MJPlayer::PlayerChiPengGang() {
  if (CPG_TYPE_NULL == chi_peng_gang_) {
    cout << "玩家" << index_ << "没有吃碰杠，直接跳过" << endl;
    return CPG_TYPE_NULL;
  }
  PrintChiPengGang();
  cout << "请选择吃碰杠：" << endl;
  int action = -1;
  do {
    cin >> action;

    if (action < 0) action = -1;
    if (action > 5) action = -1;

    if (action < 0) {
      cout << "您选择的操作不存在" << endl;;
    }

    if (1 == action && (chi_peng_gang_ & CPG_TYPE_PENG)) return CPG_TYPE_PENG;
    if (2 == action && (chi_peng_gang_ & CPG_TYPE_GANG)) return CPG_TYPE_GANG;
    if (3 == action && (chi_peng_gang_ & CPG_TYPE_CHI_LEFT)) return CPG_TYPE_CHI_LEFT;
    if (4 == action && (chi_peng_gang_ & CPG_TYPE_CHI_CENTER)) return CPG_TYPE_CHI_CENTER;
    if (5 == action && (chi_peng_gang_ & CPG_TYPE_CHI_RIGHT)) return CPG_TYPE_CHI_RIGHT;
  }
  while (action < 0);

  return CPG_TYPE_NULL;
}

int MJPlayer::Play(CardValue card) {
  if (accepted_card_ == card) {
    accepted_card_ = 0;
    AddPlayRecord(card);
  } else {
    // 打出手牌
    CardIndex index = MJLogic::CardValue2Index(card);
    cards_index_[static_cast<int>(index)]--;

    // 将摸的牌放入手牌中
    if (0 != accepted_card_) {
      // TODO: Check
      CardIndex accepted_index = MJLogic::CardValue2Index(accepted_card_);
      cards_index_[static_cast<int>(accepted_index)]++;
      accepted_card_ = 0;
    }

    // 添加打牌记录
    AddPlayRecord(card);
  }
  return 0;
}

int MJPlayer::AddPlayRecord(CardValue card) {
  play_records_[play_records_count_++] = card;
  return 0;
}

void MJPlayer::ResetChiPengGang() {
  chi_peng_gang_ = CPG_TYPE_NULL;
}

void MJPlayer::SetChiPengGang(int chi_peng_gang) {
  chi_peng_gang_ |= chi_peng_gang;
}

void MJPlayer::SetActionCard(int card) {
  action_card_ = card;
}

int MJPlayer::GetChiPengGang() {
  return chi_peng_gang_;
}

bool MJPlayer::HasChiPengGang() {
  return CPG_TYPE_NULL != chi_peng_gang_;
}

int MJPlayer::ChiPengGang(int type, CardValue chi_peng_gang_card) {
  // 添加吃碰杠组合
  card_groups_[card_groups_count_].type = type;
  card_groups_[card_groups_count_].center_card = chi_peng_gang_card;
  card_groups_count_++;

  // 删除手牌
  switch (type) {
    case CPG_TYPE_PENG: {
      cout << "peng" << endl;
      RemoveCard(chi_peng_gang_card);
      RemoveCard(chi_peng_gang_card);
      break;
    }
    case CPG_TYPE_GANG: {
      cout << "gang" << endl;
      RemoveCard(chi_peng_gang_card);
      RemoveCard(chi_peng_gang_card);
      RemoveCard(chi_peng_gang_card);
      break;
    } 
    case CPG_TYPE_CHI_LEFT: {
      cout << "zuo chi" << endl;
      RemoveCard(chi_peng_gang_card + 1);
      RemoveCard(chi_peng_gang_card + 2);
      break;
    }
    case CPG_TYPE_CHI_CENTER: {
      cout << "zhong chi" << endl;
      RemoveCard(chi_peng_gang_card - 1);
      RemoveCard(chi_peng_gang_card + 1);
      break;
    }
    case CPG_TYPE_CHI_RIGHT: {
      cout << "you chi" << endl;
      RemoveCard(chi_peng_gang_card - 2);
      RemoveCard(chi_peng_gang_card - 1);
      break;
    }
  }
  return 0;
}

} // namespace server
