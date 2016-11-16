#include "MJLogic.h"

#include "MJTable.h"
#include "Socket.h"

#include <iostream>

using std::cout;
using std::endl;

using server::MJTable;
using server::MJLogic;
using server::CardValue;
using server::Socket;

int main(int argc, char* argv[]) {
  // 代码
  //CardValue cards[ARR_SIZE(MJLogic::kCards)];
  //MJLogic::Shuffle(cards, ARR_SIZE(MJLogic::kCards));
  //MJLogic::PrintCards(cards, ARR_SIZE(MJLogic::kCards));

  // 创建桌子
  //MJTable* table = new MJTable();
  //for (int i = 0; i < 3; i++) {
    //table->CreateRobotPlayer();
  //}

  //int user_id = 0;
  //table->CreatePlayer(user_id);

  //cout << "Player Count: " << table->GetPlayerCount() << endl;

  //// 设置庄家
  //table->SetBanker(0);

  //// 游戏开始
  //table->OnGameStart();
  
  int code = 0;

  Socket socket;
  code = socket.Init();
  if (0 != code) {
    cout << "Init socket failed" << endl;
    return 255;
  }
  code = socket.Run();
  if (0 != code) {
    cout << "Socket run failed" << endl;
    return 255;
  }
  
  // 设置庄家
  return 0;
}

