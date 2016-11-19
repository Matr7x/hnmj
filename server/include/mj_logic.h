#ifndef SERVER_MJ_LOGIC_H_
#define SERVER_MJ_LOGIC_H_

#include <string>

#define MASK_COLOR 0xF0
#define MASK_VALUE 0x0F
#define MAX_CARDS 136
#define MAX_CARDS_INDEX 34
#define MAX_CARDS_VALUE 80
#define ARR_SIZE(array) sizeof(array)/sizeof(array[0])

// 吃牌类型
#define CPG_TYPE_NULL             0x00        // 没有吃牌
#define CPG_TYPE_CHI_LEFT         0x01        // 左边空缺的吃牌
#define CPG_TYPE_CHI_CENTER       0x02        // 中间空缺的吃牌
#define CPG_TYPE_CHI_RIGHT        0x04        // 右边空缺的吃牌

// 碰牌类型
#define CPG_TYPE_PENG             0X08        // 有碰牌

// 杠牌类型
#define CPG_TYPE_GANG             0X10        // 有杠牌

using std::string;

namespace server {

typedef char CardValue;
typedef char CardIndex;

class MJLogic {

public:
  static void PrintCards(const CardValue* cards, int count);
  static void PrintCards(const CardIndex* cards_index);
  static void PrintCardsWithIndex(const CardIndex* cards_index);
	static int Shuffle(CardValue* cards, int count);
  static CardIndex CardValue2Index(CardValue card);
  static CardValue CardIndex2Value(CardIndex index);
  static int CardsValue2Index(CardValue* cards, int count, CardIndex* index);
  static int EstimateChi(CardIndex cards_index[MAX_CARDS_INDEX], CardValue card);
  static int EstimatePeng(CardIndex cards_index[MAX_CARDS_INDEX], CardValue card);
  static int EstimateGang(CardIndex cards_index[MAX_CARDS_INDEX], CardValue card);
  static bool EstimateHu(CardIndex cards_index[MAX_CARDS_INDEX], CardValue card);

public:
  static const CardValue  kCards[MAX_CARDS];
  static const string     kCards2Txt[MAX_CARDS_VALUE];
  static const string     kCardsIndex2Txt[MAX_CARDS_INDEX];
};

} //namespace server

#endif//SERVER_MJ_LOGIC_H_

