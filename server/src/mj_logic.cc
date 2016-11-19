#include "mj_logic.h"

#include <cstring>
#include <cstdlib>
#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::vector;

namespace server {

const CardValue MJLogic::kCards[MAX_CARDS] = {
	// 万子
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						// 万子
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						// 万子
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						// 万子
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						// 万子
	// 索子
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						// 索子
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						// 索子
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						// 索子
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						// 索子
	// 同子
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						// 同子
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						// 同子
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						// 同子
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						// 同子
	// 风牌
	0x31,0x31,0x31,0x31,																		// 东
	0x32,0x32,0x32,0x32,																		// 西
	0x33,0x33,0x33,0x33,																		// 南
	0x34,0x34,0x34,0x34,																		// 北
	// 箭牌
	0x35,0x35,0x35,0x35,																		// 中
	0x36,0x36,0x36,0x36,																		// 发
	0x37,0x37,0x37,0x37,																		// 白
	// 花牌
	//0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48									// 春夏秋冬梅兰竹菊
};

const string MJLogic::kCards2Txt[MAX_CARDS_VALUE] = {
  "", "一万", "两万", "三万", "四万", "五万", "六万", "七万", "八万", "九万", "", "", "", "", "", "",
  "", "幺鸡", "二条", "三条", "四条", "五条", "六条", "七条", "八条", "九条", "", "", "", "", "", "",
  "", "一筒", "二筒", "三筒", "四筒", "五筒", "六筒", "七筒", "八筒", "九筒", "", "", "", "", "", "",
  "", "东", "西", "南", "北", "中", "发", "白", "", "", "", "", "", "", "", "", 
  "", "春", "夏", "秋", "冬", "梅", "兰", "竹", "菊", "", "", "", "", "", "", "",
};

const string MJLogic::kCardsIndex2Txt[MAX_CARDS_INDEX] = {
  "一万", "两万", "三万", "四万", "五万", "六万", "七万", "八万", "九万",
  "幺鸡", "二条", "三条", "四条", "五条", "六条", "七条", "八条", "九条",
  "一筒", "二筒", "三筒", "四筒", "五筒", "六筒", "七筒", "八筒", "九筒",
  "东", "西", "南", "北", "中", "发", "白"
};

void MJLogic::PrintCards(const CardValue* cards, int count) {
  int i = 0;
  while (i < count) {
    cout << MJLogic::kCards2Txt[static_cast<int>(cards[i++])];
    if (0 == i % 9 || i == count) {
      cout << endl;
    } else {
      cout << ",";
    }
  }
}

void MJLogic::PrintCards(const CardIndex* cards_index) {
  for (int i = 0; i < MAX_CARDS_INDEX; i++) {
    for (int j = 0; j < cards_index[i]; j++) {
      cout << MJLogic::kCardsIndex2Txt[i];
      cout << ",";
    }
  }
}

void MJLogic::PrintCardsWithIndex(const CardIndex* cards_index) {
  for (int i = 0; i < MAX_CARDS_INDEX; i++) {
    for (int j = 0; j < cards_index[i]; j++) {
      cout << MJLogic::kCardsIndex2Txt[i] << "-" << i;
      cout << ",";
    }
  }
  cout << endl;
}

int MJLogic::Shuffle(CardValue* cards, int count) {
  // TODO: Check Param

	// 将麻将卡牌数据拷贝到一个临时数组
	CardValue card_tmp[ARR_SIZE(kCards)];
	memcpy(card_tmp, kCards, sizeof(card_tmp));
	
  // 初始化
  int rand_pos = 0;
  int pos = 0;
  srand((int)time(0));                              // 设置随机种子

  // 洗牌
  while (pos < count) {
    rand_pos = rand() % (count - pos);              // 计算一个随机位置
    cards[pos++] = card_tmp[rand_pos];              // 将随机选择的牌放入数组
    card_tmp[rand_pos] = card_tmp[count - pos];     // 将最后一张牌填入随机位置
  }

  return 0;
}

CardIndex MJLogic::CardValue2Index(CardValue card) {
  // TODO: Check CardValue
  return ((card & MASK_COLOR) >> 4) * 9 + (card & MASK_VALUE) - 1;
}

CardIndex MJLogic::CardIndex2Value(CardIndex index) {
  return ((index / 9) << 4) | (index % 9 + 1);
}

int MJLogic::CardsValue2Index(CardValue* cards, int count, CardIndex index[MAX_CARDS_INDEX]) {
  // TODO: Check Param
  
  // 归零
  memset(index, 0, sizeof(CardIndex) * MAX_CARDS_INDEX);

  // 设置Index
  for (int i = 0; i < count; i++) {
    index[static_cast<int>(CardValue2Index(cards[i]))]++;
  }

  return count;
}

int MJLogic::EstimateChi(CardIndex cards_index[MAX_CARDS_INDEX], CardValue card) {
  // 过滤字牌、花牌
  if (card >= 0x31) {
    return CPG_TYPE_NULL;
  }

  // 三种吃类型
  const int kType[3] = {CPG_TYPE_CHI_LEFT, CPG_TYPE_CHI_CENTER, CPG_TYPE_CHI_RIGHT};
  
  CardIndex index = CardValue2Index(card);
  int offset = index % 9;
  int chi_type = CPG_TYPE_NULL;
  for (int i = 0; i < 3; i++) {
    // 判断三种类型的边界
    if (offset >= i && ((offset - i) <= 6)) {
      // 计算第吃牌第一张索引
      CardIndex first_index = index - i;

      if ((index != first_index) && (0 == cards_index[static_cast<int>(first_index)])) continue;
      if ((index != first_index + 1) && (0 == cards_index[static_cast<int>(first_index + 1)])) continue;
      if ((index != first_index + 2) && (0 == cards_index[static_cast<int>(first_index + 2)])) continue;

      chi_type |= kType[i];
    }
  }

  return chi_type;
}

int MJLogic::EstimatePeng(CardIndex cards_index[MAX_CARDS_INDEX], CardValue card) {
  return (cards_index[static_cast<int>(CardValue2Index(card))] < 2) ? CPG_TYPE_NULL : CPG_TYPE_PENG;
}

int MJLogic::EstimateGang(CardIndex cards_index[MAX_CARDS_INDEX], CardValue card) {
  return (cards_index[static_cast<int>(CardValue2Index(card))] < 3) ? CPG_TYPE_NULL : CPG_TYPE_GANG;
}

bool MJLogic::EstimateHu(CardIndex cards_index[MAX_CARDS_INDEX], CardValue card) {
  // TODO: 检查牌的个数
  

  // 将card转换为index
  CardIndex index = CardValue2Index(card);

  // 找出所有对子的index
  vector<int> two_cards_index;
  for (int i = 0; i < MAX_CARDS_INDEX; i++) {
    if (i != index) {
      if (2 == cards_index[i]) {
        two_cards_index.push_back(i);
      }
    } else {
      if (1 == cards_index[i]) {
        two_cards_index.push_back(i);
      }
    }
  }

  // 如果没有对子，说明没有胡
  if (0 == two_cards_index.size()) {
    return false;
  }

  bool hu = false;
  for (size_t i = 0; i < two_cards_index.size(); i++) {
    CardIndex temp_cards_index[MAX_CARDS_INDEX];
    memset(temp_cards_index, 0, sizeof(temp_cards_index));
    memcpy(temp_cards_index, cards_index, sizeof(temp_cards_index));
    temp_cards_index[static_cast<int>(index)]++;
    temp_cards_index[two_cards_index[i]] = 0;

    // 将胡的牌拆解出来
    for (int j = 0; j < MAX_CARDS_INDEX; j++) {
      if (temp_cards_index[j] >= 3) {
        temp_cards_index[j] -= 3;
      }

      if ((j < (MAX_CARDS_INDEX - 9)) && ((j % 9) < 7)) {
        if (temp_cards_index[j] >= 1 && temp_cards_index[j] < 3) {
          if (temp_cards_index[j + 1] < temp_cards_index[j]) {
            break;
          }
          if (temp_cards_index[j + 2] < temp_cards_index[j]) {
            break;
          }
          int c = temp_cards_index[j];
          temp_cards_index[j]     -= c;
          temp_cards_index[j + 1] -= c;
          temp_cards_index[j + 2] -= c;
        }
      }
    }

    // 如果所有组合都抽取出来，应该是一张牌不剩
    bool is_hu = true;
    for (int j = 0; j < MAX_CARDS_INDEX; j++) {
      // 剩一张牌就说明没有胡
      if (temp_cards_index[j] > 0) {
        is_hu = false;
      }
    }

    if (is_hu) {
      hu = true;
      break;
    }
  }

  return hu;
}

} // namespace server
