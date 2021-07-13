#define _CRT_SECURE_NO_WARNINGS

/*** インクルード宣言 ***/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <errno.h>
#include <math.h>
#include <string.h>
#include <unistd.h>


/*** グローバル定数の宣言 ***/
// カードの種類
const int CARD_TYPE = 10 + 1;   // カードナンバーと要素を合わせるために＋1
// 全カード枚数
const int NUMBER_OF_CARD = 18;
// 手札のカード枚数
const int NUMBER_OF_HAND = 2;
// 捨て札のカード枚数
const int NUMBER_OF_DISCARDAREA = 13;
// カードの名前
const char CARD_NAME[CARD_TYPE][30] = { "  ", "少年", "兵士", "占い師", "女神", "死神", "貴族", "賢者", "精霊", "皇帝", "英雄" };
// カードの効果名
const char CARD_EFFECT[CARD_TYPE][30] = { "  ", "革命", "捜査", "透視", "守備", "疫病", "対面／対決", "選択", "交換", "公開処刑", "転生" };
// computerのパラメーター（最初の要素の0は、意味なし）
// computerの手札のカードを捨て札に移す時（ゲーム前半／後半）
const int COMPUTER_DISCARD_FIRST[CARD_TYPE]  = { 0, 9, 3, 7, 7, 6, 5, 6, 2, 1, 0 };
const int COMPUTER_DISCARD_LATTER[CARD_TYPE] = { 0, 9, 9, 8, 7, 6, 5, 3, 2, 1, 0 };
// humanの手札のカードを捨て札に移す時(ゲーム前半／後半)
const int HUMAN_DISCARD_FIRST[CARD_TYPE]     = { 0, 1, 7, 4, 2, 3, 5, 8, 8, 9, 6 };
const int HUMAN_DISCARD_LATTER[CARD_TYPE]    = { 0, 1, 7, 2, 2, 3, 5, 8, 8, 9, 10 };
// 賢者の効果『選択』によってカードを選ぶ時（ゲーム前半／後半）
const int WISEMAN_EFFECT_FIRST[CARD_TYPE]    = { 0, 1, 5, 4, 2, 4, 2, 8, 8, 9, 3 };
const int WISEMAN_EFFECT_LATER[CARD_TYPE]    = { 0, 1, 3, 2, 3, 4, 5, 7, 8, 9, 9 };
// 一時停止タイマー
int PAUSE_TIMER = 2;

/*** 構造体型宣言 ***/
// ゲーム管理
typedef struct GAMEMANAGE {
  int deck[18];             // 山札
  int reincarnationCard;    // 転生札
  int wiseManSelectCard[3]; // 賢者の効果『選択』のカード
  int draw_win_lose;        // 引き分け：0 プレイヤーの勝ち：1 プレイヤーの負け：2 （初期値：-1）
  int countJuvenile;        // 場に出た少年のカード（1）をカウント
  int countAristocrat;      // 場に出た貴族のカード（6）をカウント
} GameManage;
// プレイヤー
typedef struct PLAYER {
  char* name;                             // プレイヤー名
  bool computer;                          // プレイヤーがcomputerかどうか
  int hand[NUMBER_OF_HAND];               // 手札のカード
  int discardArea[NUMBER_OF_DISCARDAREA]; // 捨て札のカード
  bool maidenEffect;                      // 女神の効果『守備』
  bool wiseManEffect;                     // 賢者の効果『選択』
} Player;


/*** プロトタイプ宣言 ***/
// game flow
void playingGame(GameManage* pGameManage, Player* pHumanPlayer, Player* pComputerPlayer);
// player turn
bool playerTurn(GameManage* pGameManage, Player* pAttackPlayer, Player* pDefensePlayer);
// draw card
void deckCard_to_hand(GameManage* pGameManage, Player* pPlayer);
int randomDraw(GameManage* pGameManage, bool wiseManEffect);
int deckZeroCount(GameManage* pGameManage);
void deckSort(GameManage* pGameManage);
// add card to handCard
void addCard_to_hand(Player* pPlayer, int cardNum);
// handCard to discardArea
void handCard_to_discardArea(Player* pPlayer, int num, bool cardNum);
int addDiscardArea_Index(Player* pPlayer);
// select card
int selectCardNum(GameManage* pGameManage, Player* pPlayer, int selectType, bool selectOpenCard, bool wiseManEffect);
int selectCardIndex(GameManage* pGameManage, Player* pPlayer);
// commandOK
int commandInput(GameManage* pGameManage, Player* pPlayer, int selectType, bool selectOpenCard, bool wiseManEffect);
bool commandCheck_handCard(Player* pPlayer, int commandNum);
bool commandCheck_wiseManSelectCard(GameManage* pGameManage, int commandNum);
// card effect
void cardEffect(GameManage* pGameManage, Player* pAttackPlayer, Player* pDefensePlayer, int cardNum);
void juvenile(GameManage* pGameManage, Player* pAttackPlayer, Player* pDefensePlayer, int cardNum);
void soldier(GameManage* pGameManage, Player* pAttackPlayer, Player* pDefensePlayer, int cardNum);
void fortunTeller(GameManage* pGameManage, Player* pAttackPlayer, Player* pDefensePlayer, int cardNum);
void maiden(GameManage* pGameManage, Player* pAttackPlayer, Player* pDefensePlayer, int cardNum);
void reaper(GameManage* pGameManage, Player* pAttackPlayer, Player* pDefensePlayer, int cardNum);
void aristocrat(GameManage* pGameManage, Player* pAttackPlayer, Player* pDefensePlayer, int cardNum);
void wiseMan(GameManage* pGameManage, Player* pAttackPlayer, Player* pDefensePlayer, int cardNum);
void spirit(GameManage* pGameManage, Player* pAttackPlayer, Player* pDefensePlayer, int cardNum);
void emperor(GameManage* pGameManage, Player* pAttackPlayer, Player* pDefensePlayer, int cardNum);
void hero(GameManage* pGameManage, Player* pDefensePlayer, int cardNum);
// game set
void gameSet(GameManage* pGameManage, Player* pAttackPlayer);
void compareCardNum(GameManage* pGameManage, Player* pPlayer_1, Player* pPlayer_2);
int handCardNumber(Player* pPlayer);
// computer random operation
int randomHandCard(GameManage* pGameManage, Player* pPlayer);
int randomWiseManSelect(GameManage* pGameManage, Player* pPlayer, int drawCount);
int randomSoldierSelect(GameManage* pGameManage, Player* pHumanPlayer, Player* pComputerPlayer);
// print game field
void printGameField(GameManage* pGame, Player* pHumanPlayer, Player* pComputerPlayer);

/*** 関数宣言 ***/
// ************************ main(initial setting) ******************************
// ゲーム開始準備から終了までの流れ
int main(void){
  srand((unsigned)time(NULL));
  printf("*** XENO ***\n");

  char game;
  do{
    // プレイヤー名の入力
    printf("プレイヤー名を入力してください：");
    char name[30];
    scanf("%s", name);
    // プレイヤーの準備
    Player humanPlayer = { name, false, {0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, false, false };
    Player computerPlayer = { "computer", true, {0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, false, false };
    // ゲームフィールドの準備
    GameManage gameManage = { {1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 10}, 0, {0, 0, 0}, -1, 0, 0 };
    // カードを1枚ずつプレイヤーに配る
    deckCard_to_hand(&gameManage, &humanPlayer);
    deckCard_to_hand(&gameManage, &computerPlayer);
    // 転生札をセットする
    gameManage.reincarnationCard = randomDraw(&gameManage, false);

    // ゲーム開始
    playingGame(&gameManage, &humanPlayer, &computerPlayer);

    // ゲーム継続？
    do{
      printf("もう一度しますか？ [yes:1  no:0]");
      scanf("%c", &game);
    } while(!(game == '0' || game == '1'));
  } while(game == '1');
  printf("ゲームを終了します\n");
  return 0;
}

// ************************ game flow ******************************************
// ゲーム開始から終了するまでを管理
void playingGame(GameManage* pGameManage, Player* pHumanPlayer, Player* pComputerPlayer){
  // 決着が着くまでループ
  while(true){
    // humanPlayerのターン
    if(playerTurn(pGameManage, pHumanPlayer, pComputerPlayer))
      break;
    // computerPlayerのターン
    if(playerTurn(pGameManage, pComputerPlayer, pHumanPlayer))
      break;
  }
  printf("\n\n");
  printf("<<<<<<<<<<<<<<<<<<<<<<< ゲーム終了 >>>>>>>>>>>>>>>>>>>>>>>\n");
  // humanPlayerとcomputerPlayerの手持ちカードを表示
  int humanCard = handCardNumber(pHumanPlayer);
  int computerCard = handCardNumber(pComputerPlayer);
  printf("  %sのカード -> [%d:%s]      %sのカード -> [%d:%s]\n\n",
      pHumanPlayer->name, humanCard, CARD_NAME[humanCard], pComputerPlayer->name, computerCard, CARD_NAME[computerCard]);
  // 結果発表
  if(pGameManage->draw_win_lose == 0){
    printf("                      引き分け\n");
  } else if(pGameManage->draw_win_lose == 1){
    printf("                      %sの勝ち\n", pHumanPlayer->name);
  } else if(pGameManage->draw_win_lose == 2){
    printf("                      %sの負け\n", pHumanPlayer->name);
  }
  printf("<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");

}

// ************************ player turn ****************************************
// プレイヤーのターン開始から終了までを管理
bool playerTurn(GameManage* pGameManage, Player* pAttackPlayer, Player* pDefensePlayer){
  printf("\n\n　　　　　　　       　【%sのターン】\n", pAttackPlayer->name);
  sleep(PAUSE_TIMER);
  // 女神の効果を解除
  pAttackPlayer->maidenEffect = false;
  // 賢者の効果を保有している場合
  if(pAttackPlayer->wiseManEffect == true){
    // ゲーム状況を出力
    printGameField(pGameManage, pAttackPlayer, pDefensePlayer);
    // 賢者の効果発動
    cardEffect(pGameManage, pAttackPlayer, pDefensePlayer, 7);
  } else {
    // 山札からカードを引く
    deckCard_to_hand(pGameManage, pAttackPlayer);
  }
  // ゲーム状況を出力
  printGameField(pGameManage, pAttackPlayer, pDefensePlayer);
  // 捨てるカードの選択
  int cardNum;
  printf("  %s -> 場に出すカードを手札から選択\n", pAttackPlayer->name);
  // 攻撃側がcomputerの場合 -> 設定したパラメータを元に捨てるカードをランダムに選択
  if(pAttackPlayer->computer){
    sleep(PAUSE_TIMER);
    cardNum = randomHandCard(pGameManage, pAttackPlayer);
  // 攻撃側がhumanの場合 -> 捨てるカードを選択する（コマンド入力）
  } else {
    cardNum = selectCardNum(pGameManage, pAttackPlayer, 1, true, false);
  }
  // カードを捨てる処理
  handCard_to_discardArea(pAttackPlayer, cardNum, true);
  // カードの効果を発動
  cardEffect(pGameManage, pAttackPlayer, pDefensePlayer, cardNum);
  // 勝敗がついていたらtrueを返す
  if(pGameManage->draw_win_lose >= 0)
    return true;
  // カード枚数が0の場合、カードナンバーの大きさで勝敗を決める
  if(deckZeroCount(pGameManage) == NUMBER_OF_CARD){
    compareCardNum(pGameManage, pAttackPlayer, pDefensePlayer);
    return true;
  }
  return false;
}

// ************************ draw card  *****************************************
// カードを1枚引く（山札から手札にカードを1枚移動する）
void deckCard_to_hand(GameManage* pGameManage, Player* pPlayer){
  // 山札から引いたカードを取得
  int drawCardNum = randomDraw(pGameManage, false);
  // 引いたカードナンバーを手札に加える
  addCard_to_hand(pPlayer, drawCardNum);
}

// 山札からランダムにカードを取得
int randomDraw(GameManage* pGameManage, bool wiseManEffect){
  // 山札の0の個数をカウント
  int zeroCount = deckZeroCount(pGameManage);
  // 山札の配列の要素をランダムに取得（要素の値！＝0）
  int randIndex = zeroCount + rand() % (NUMBER_OF_CARD - zeroCount);
  // 山札から引いたカードナンバー
  int drawCardNum = pGameManage->deck[randIndex];
  // 7：賢者の効果『選択』でない場合
  if(!wiseManEffect){
    // インデックスから引いたカードナンバーを0に変換（山札）
    pGameManage->deck[randIndex] = 0;
    // 山札を照準に並べる
    deckSort(pGameManage);
  }
  return drawCardNum;
}

// 山札の0の個数をカウント
int deckZeroCount(GameManage* pGameManage){
  int zeroCount = 0;
  for(int i=0; i<NUMBER_OF_CARD; i++){
    // 山札の要素の値が0の個数をカウント
    if(pGameManage->deck[i] == 0)
      zeroCount++;
  }
  return zeroCount;
}

// 山札を昇順に並べ替える
void deckSort(GameManage* pGameManage){
  for(int i=0; i<NUMBER_OF_CARD-1; i++){
    for(int j=i+1; j<NUMBER_OF_CARD; j++){
      // 入れ替えを行う
      if(pGameManage->deck[i] > pGameManage->deck[j]){
        int temp = pGameManage->deck[i];
        pGameManage->deck[i] = pGameManage->deck[j];
        pGameManage->deck[j] = temp;
      }
    }
  }
}

// ************************ add card to hand ***********************************
// カードを手札に加える処理
void addCard_to_hand(Player* pPlayer, int cardNum){
  // 手札の空いている（要素の値が0）インデックスを取得
  int index;
  for(int i=0; i<NUMBER_OF_HAND; i++){
    if(pPlayer->hand[i] == 0){
      index = i;
      break;
    }
  }
  // カードを手札に加える
  pPlayer->hand[index] = cardNum;
}

// ************************ handCard to discardArea ****************************
// 手札から捨て札へ捨てる処理
void handCard_to_discardArea(Player* pPlayer, int num, bool cardNum){
  int index_hand = num;
  if(cardNum){
    // カードナンバーからインデックスを取得
    for(int i=0; i<NUMBER_OF_HAND; i++){
      if(pPlayer->hand[i] == num) index_hand = i;
    }
  }
  // 捨て札にカードを加える位置（インデックス）を取得
  int index_discardArea = addDiscardArea_Index(pPlayer);
  // カードを捨て札にコピー
  pPlayer->discardArea[index_discardArea] = pPlayer->hand[index_hand];

  // 手札からカードをなくし、0に変換
  pPlayer->hand[index_hand] = 0;
}

// 捨て札に加える位置（インデックス）を取得
int addDiscardArea_Index(Player* pPlayer){
  int index;
  for(int i=0; i<NUMBER_OF_DISCARDAREA; i++){
    if(pPlayer->discardArea[i] == 0){
      index = i;
      break;
    }
  }
  return index;
}

// ************************ select card ****************************************
// カード番号をコマンド入力で選択
int selectCardNum(GameManage* pGameManage, Player* pPlayer, int selectType, bool selectOpenCard, bool wiseManEffect){
  printf("[入力：カードナンバー]\n");
  // コマンド入力
  int cardNum = commandInput(pGameManage, pPlayer, selectType, selectOpenCard, wiseManEffect);
  return cardNum;
}

// 添字でカードを選択
int selectCardIndex(GameManage* pGameManage, Player* pPlayer){
  printf("[入力：左側→0 ／ 右側→1]\n");
  // コマンド入力
  int index = commandInput(pGameManage, pPlayer, 0, false, false);
  return index;
}

// ************************ command ********************************************
// コマンド入力処理　（selectType -> 0：0〜1　1：1〜9　2：1〜10)
int commandInput(GameManage* pGameManage, Player* pPlayer, int selectType, bool selectOpenCard, bool wiseManEffect){
  char command[128], *endptr;
  long commandNum;
  int commandInt;
  bool commandOK = false;

  while(true) {
    errno = 0;    // エラー初期化
    printf("コマンドを入力 -> ");
    scanf("%s", command);

    commandNum = strtol(command, &endptr, 10);
    // [条件1]数学関数の値域
    if(errno != ERANGE){
      // [条件2]整数値（変換不可能部分を含まない）
      if(*endptr == '\0'){
        switch(selectType){
        // カードを2択で選択する場合
        case 0:
          if((0 <= commandNum) && (commandNum <= 1)){
            commandInt = (int)commandNum;
            commandOK = true;
          }
          break;
        // 自分の手札を選択する場合（10は選択できない）
        case 1:
          if((1 <= commandNum) && (commandNum <= 9)){
            commandInt = (int)commandNum;
            if(commandCheck_handCard(pPlayer, commandInt))
              commandOK = true;
          }
          break;
        // 相手の手札を選択する場合
        case 2:
          if((1 <= commandNum) && (commandNum <= 10)){
            commandInt = (int)commandNum;
            // カードを公開の状態で入力
            if(selectOpenCard){
              // 賢者の効果『選択』による入力
              if(wiseManEffect){
                if(commandCheck_wiseManSelectCard(pGameManage, commandInt))
                  commandOK = true;
              // 賢者の効果『選択』による入力でない
              } else {
                if(commandCheck_handCard(pPlayer, commandInt))
                  commandOK = true;
              }
              // カードを非公開の状態での入力
            } else {
              commandOK = true;
            }
          }
          break;
        }
      }
    }
    if(commandOK)
      break;
    printf("コマンド入力エラー\n");
  }
  return commandInt;
}

// コマンド入力と手札の確認
bool commandCheck_handCard(Player* pPlayer, int commandNum){
  // 入力した数値が手札に含まれているか確認
  bool check = false;
  for(int i=0; i<NUMBER_OF_HAND; i++){
    if(commandNum == pPlayer->hand[i]){
      check = true;
      break;
    }
  }
  return check;
}

// コマンド入力と賢者の効果『選択』のカードとの比較
bool commandCheck_wiseManSelectCard(GameManage* pGameManage, int commandNum){
  // 入力した数値が選択のカードに含まれているか確認
  bool check = false;
  // 配列の要素数の取得
  int arrayCount = sizeof(pGameManage->wiseManSelectCard) / sizeof(int);
  for(int i=0; i<arrayCount; i++){
    if(commandNum == pGameManage->wiseManSelectCard[i]){
      check = true;
      break;
    }
  }
  return check;
}

// ************************ card effect ****************************************
// カードの振り分け
void cardEffect(GameManage* pGameManage, Player* pAttackPlayer, Player* pDefensePlayer, int cardNum){
  // 守備側が女神の効果『守備』により攻撃を防げる場合
  if((pDefensePlayer->maidenEffect == true) && !(cardNum == 4 || cardNum == 7)){
    printf("  %s -> [%d:%s]の効果『%s』\n", pAttackPlayer->name, cardNum, CARD_NAME[cardNum], CARD_EFFECT[cardNum]);
    printf("  %s -> %sの効果『%s』により、カードの効果を無効化された\n", pAttackPlayer->name, CARD_NAME[4], CARD_EFFECT[4]);
    // 攻撃側のカードが1だった場合、カウント
    if(cardNum == 1)
      pGameManage->countJuvenile++;
    // 攻撃側のカードが6だった場合、カウント
    if(cardNum == 6)
      pGameManage->countAristocrat++;
  // 守備側が女神の効果で防げない場合 -> 攻撃を続行
  } else {
    switch(cardNum){
      case 1:
        juvenile(pGameManage, pAttackPlayer, pDefensePlayer, cardNum);
        break;
      case 2:
        soldier(pGameManage, pAttackPlayer, pDefensePlayer, cardNum);
        break;
      case 3:
        fortunTeller(pGameManage, pAttackPlayer, pDefensePlayer, cardNum);
        break;
      case 4:
        maiden(pGameManage, pAttackPlayer, pDefensePlayer, cardNum);
        break;
      case 5:
        reaper(pGameManage, pAttackPlayer, pDefensePlayer, cardNum);
        break;
      case 6:
        aristocrat(pGameManage, pAttackPlayer, pDefensePlayer, cardNum);
        break;
      case 7:
        wiseMan(pGameManage, pAttackPlayer, pDefensePlayer, cardNum);
        break;
      case 8:
        spirit(pGameManage, pAttackPlayer, pDefensePlayer, cardNum);
        break;
      case 9:
        emperor(pGameManage, pAttackPlayer, pDefensePlayer, cardNum);
        break;
    }
  }
}

// 1:少年の効果『革命』
void juvenile(GameManage* pGameManage, Player* pAttackPlayer, Player* pDefensePlayer, int cardNum){
  pGameManage->countJuvenile++;
  // 場に出た少年のカード（1）が2枚目の場合、『革命』の効果発動
  if(pGameManage->countJuvenile == 2){
    printf("  %s -> \x1b[41m[%d:%s]の効果『%s』\x1b[49m\n", pAttackPlayer->name, cardNum, CARD_NAME[cardNum], CARD_EFFECT[cardNum]);
    // 山札が1枚以上であることを確認
    if(NUMBER_OF_CARD - deckZeroCount(pGameManage) > 0){
      // 相手にカードを1枚引かせる
      printf("  %s -> %sに山札からカードを1枚引かせる\n", pAttackPlayer->name, pDefensePlayer->name);
      deckCard_to_hand(pGameManage, pDefensePlayer);
      // 相手の手札をオープン
      printf("  %s -> %sの手札... ", pAttackPlayer->name, pDefensePlayer->name);
      for(int i=0; i<NUMBER_OF_HAND; i++){
        printf(" [%d:%s] ", pDefensePlayer->hand[i], CARD_NAME[pDefensePlayer->hand[i]]);
      }
      printf("\n");
      // 捨てさせるカードナンバー
      int cardNum;
      printf("  %s -> %sの手札から捨てるカードを選択\n", pAttackPlayer->name, pDefensePlayer->name);
      // 攻撃側がcomputerの場合 -> 設定したパラメータを元に捨てさせるカードをランダムに選択
      if(pAttackPlayer->computer){
        cardNum = randomHandCard(pGameManage, pDefensePlayer);
      // 攻撃側がhumanの場合 -> 捨てさせるカードを選択（コマンド入力)
      } else {
        cardNum = selectCardNum(pGameManage, pDefensePlayer, 2, true, false);
      }
      // 選択したカードを表示
      printf("  %s -> [%d:%s]を選択\n", pAttackPlayer->name, cardNum, CARD_NAME[cardNum]);
      // カードを捨てる処理
      handCard_to_discardArea(pDefensePlayer, cardNum, true);
    } else {
      printf("山札が0枚のため、ゲーム終了\n");
    }
  // 場に出た少年のカード（1）が1枚目の場合、効果なし
  } else {
    printf("  %s -> [%d:%s]を出した（発動効果なし）\n", pAttackPlayer->name, cardNum, CARD_NAME[cardNum]);
  }
}

// 2:兵士の効果『捜査』
void soldier(GameManage* pGameManage, Player* pAttackPlayer, Player* pDefensePlayer, int cardNum){
  printf("  %s -> \x1b[43m[%d:%s]の効果『%s』\x1b[49m\n", pAttackPlayer->name, cardNum, CARD_NAME[cardNum], CARD_EFFECT[cardNum]);
  // 相手のカードを推測
  int guessNum;
  printf("  %s -> %sの手札を推測して、カードナンバーを入力\n", pAttackPlayer->name, pDefensePlayer->name);
  // 攻撃側がcomputerの場合 -> 場に出ていないカードからランダムに選択
  if(pAttackPlayer->computer){
    guessNum = randomSoldierSelect(pGameManage, pAttackPlayer, pDefensePlayer);
  // 攻撃側がhumanの場合 -> コマンド入力
  } else {
    guessNum = selectCardNum(pGameManage, pDefensePlayer, 2, false, false);
  }
  // 推測した番号が相手の手札のカードと同じ場合
  if(guessNum == handCardNumber(pDefensePlayer)){
    printf("  %s -> %sの手札を的中！！\n", pAttackPlayer->name, pDefensePlayer->name);
    // 相手のカードが10：相手は転生カードを手札にとる
    if(guessNum == 10){
      hero(pGameManage, pDefensePlayer, guessNum);
    // 相手のカードが10以外：攻撃側の勝利
    } else {
      gameSet(pGameManage, pAttackPlayer);
    }
  // 推測した番号が相手の手札のカードと異なる場合
  } else {
    printf("  %s -> 『%s』失敗...\n", pAttackPlayer->name, CARD_EFFECT[cardNum]);
  }
}

// 3:占い師の効果『透視』
void fortunTeller(GameManage* pGameManage, Player* pAttackPlayer, Player* pDefensePlayer, int cardNum){
  printf("  %s -> \x1b[45m[%d:%s]の効果『%s』\x1b[49m\n", pAttackPlayer->name, cardNum, CARD_NAME[cardNum], CARD_EFFECT[cardNum]);
  // 相手のカードナンバーを取得
  int defenseCardNum = handCardNumber(pDefensePlayer);
  printf("  %s -> %sの手札は...　[%d:%s]\n", pAttackPlayer->name, pDefensePlayer->name, defenseCardNum, CARD_NAME[defenseCardNum]);
}

// 4:女神の効果『守備』
void maiden(GameManage* pGameManage, Player* pAttackPlayer, Player* pDefensePlayer, int cardNum){
  printf("  %s -> \x1b[46m[%d:%s]の効果『%s』\x1b[49m\n", pAttackPlayer->name, cardNum, CARD_NAME[cardNum], CARD_EFFECT[cardNum]);
  printf("  %s -> 次のターン %sへの効果（攻撃）を無効化", pAttackPlayer->name, pAttackPlayer->name);
  pAttackPlayer->maidenEffect = true;
}

// 5:死神の効果『疫病』
void reaper(GameManage* pGameManage, Player* pAttackPlayer, Player* pDefensePlayer, int cardNum){
  printf("  %s -> \x1b[47m[%d:%s]の効果『%s』\x1b[49m\n", pAttackPlayer->name, cardNum, CARD_NAME[cardNum], CARD_EFFECT[cardNum]);
  // 山札が1枚以上であることを確認
  if(NUMBER_OF_CARD - deckZeroCount(pGameManage) > 0){
    // 相手にカードを1枚引かせる
    printf("  %s -> %sに山札からカードを1枚引かせる\n", pAttackPlayer->name, pDefensePlayer->name);
    deckCard_to_hand(pGameManage, pDefensePlayer);
    // 相手のカードを伏せた状態で捨てさせるカードを選択
    printf("  %s -> どちらかのカードを選択\n", pAttackPlayer->name);
    printf("  %s -> %sの手札...   [カード：0]     [カード：1]\n", pAttackPlayer->name, pDefensePlayer->name);
    int index;
    // 攻撃側がcomputerの場合 -> ランダムに選択
    if(pAttackPlayer->computer){
      index = rand() % 2;
    // 攻撃側がhumanの場合 -> コマンド入力
    } else {
      index = selectCardIndex(pGameManage, pDefensePlayer);
    }
    // 選択したカードの表示
    printf("  %s -> [カード：%d]を選んだ\n", pAttackPlayer->name, index);
    printf("  %s -> %sの[%d:%s]を捨て札へ\n", pAttackPlayer->name, pDefensePlayer->name, pDefensePlayer->hand[index], CARD_NAME[pDefensePlayer->hand[index]]);
    // カードを捨てる処理
    handCard_to_discardArea(pDefensePlayer, index, false);
  } else {
    printf("山札が0枚のため、ゲーム終了\n");
  }
}

// 6：貴族の効果『対面／対決』
void aristocrat(GameManage* pGameManage, Player* pAttackPlayer, Player* pDefensePlayer, int cardNum){
  pGameManage->countAristocrat++;
  // 場に出た貴族のカード（6）が1枚目 → 『対面』
  if(pGameManage->countAristocrat == 1){
    printf("  %s -> \x1b[44m[%d:%s]の効果『対面』\x1b[49m\n", pAttackPlayer->name, cardNum, CARD_NAME[cardNum]);
    // 攻撃側のカードを表示
    int attackCardNum = handCardNumber(pAttackPlayer);
    printf("      %sのカードは...　[%d:%s]\n", pAttackPlayer->name, attackCardNum, CARD_NAME[attackCardNum]);
    // 守備側のカードを表示
    int defenseCardNum = handCardNumber(pDefensePlayer);
    printf("      %sのカードは...　[%d:%s]\n", pDefensePlayer->name, defenseCardNum, CARD_NAME[defenseCardNum]);
  // 場に出た貴族のカード（6）が2枚目 → 『対決』
  } else if(pGameManage->countAristocrat == 2){
    printf("  %s -> \x1b[44m[%d:%s]の効果『対決』\x1b[49m\n", pAttackPlayer->name, cardNum, CARD_NAME[cardNum]);
    // カードの大きさで勝負
    compareCardNum(pGameManage, pAttackPlayer, pDefensePlayer);
  }
}

// 7:賢者の効果『選択』
void wiseMan(GameManage* pGameManage, Player* pAttackPlayer, Player* pDefensePlayer, int cardNum){
  printf("  %s -> \x1b[42m[%d:%s]の効果『%s』\x1b[49m\n", pAttackPlayer->name, cardNum, CARD_NAME[cardNum], CARD_EFFECT[cardNum]);
  // 攻撃側プレイヤーの賢者の効果がtrueの場合
  if(pAttackPlayer->wiseManEffect == true){
    // 山札が1枚以上であることを確認
    if(NUMBER_OF_CARD - deckZeroCount(pGameManage) > 0){
      // 山札から引く枚数を取得
      int drawCount = 3;
      // 山札が二枚以下の場合
      int deckCardCount = NUMBER_OF_CARD - deckZeroCount(pGameManage);
      if(deckCardCount < 3)
        drawCount = deckCardCount;
      // 山札から枚数分のカードを引く
      printf("  %s -> 山札から%d枚カードを引く\n", pAttackPlayer->name, drawCount);
      for(int i=0; i<drawCount; i++){
        pGameManage->wiseManSelectCard[i] = randomDraw(pGameManage, true);
      }
      // カードの選択
      int selectCard;
      // 攻撃側がcomputerの場合 -> 設定したパラメータを元に選択するカードをランダムに選択
      if(pAttackPlayer->computer){
        printf("  %s -> 手札に加えるカードを選択\n", pAttackPlayer->name);
        selectCard = randomWiseManSelect(pGameManage, pAttackPlayer, drawCount);
      // 攻撃側がhumanの場合 -> 選択するカードを選択（コマンド入力)
      } else {
        // 選択できるカードの表示
        printf("  %s -> ", pAttackPlayer->name);
        for(int i=0; i<drawCount; i++){
          int drawCardNum = pGameManage->wiseManSelectCard[i];
          printf("[%d:%s] ", drawCardNum, CARD_NAME[drawCardNum]);
        }
        printf("\n");
        // カードを選択
        printf("  %s -> 手札に加えるカードを選択\n", pAttackPlayer->name);
        selectCard = selectCardNum(pGameManage, pAttackPlayer, 2, true, true);
      }
      // 選択したカードを手札に加える
      addCard_to_hand(pAttackPlayer, selectCard);
      // 選択したカードの山札を0に変換する処理
      for(int i=0; i<NUMBER_OF_CARD; i++){
        if(pGameManage->deck[i] == selectCard){
          pGameManage->deck[i] = 0;
          break;
        }
      }
      // 攻撃側プレイヤーの賢者の効果『選択』をfalseにする
      pAttackPlayer->wiseManEffect = false;
    } else {
      printf("山札が0枚のため、ゲーム終了\n");
    }
  // 攻撃側プレイヤーの賢者の効果がfalseの場合
  } else {
    printf("  %s -> 次の%sのターンで効果発動\n", pAttackPlayer->name, pAttackPlayer->name);
    pAttackPlayer->wiseManEffect = true;
  }
}

// 8:精霊の効果『交換』
void spirit(GameManage* pGameManage, Player* pAttackPlayer, Player* pDefensePlayer, int cardNum){
  printf("  %s -> \x1b[42m[%d:%s]の効果『%s』\x1b[49m\n", pAttackPlayer->name, cardNum, CARD_NAME[cardNum], CARD_EFFECT[cardNum]);
  // 各プレイヤーの手札のカードナンバーを取得
  int attackCardNum = handCardNumber(pAttackPlayer);
  int defenseCardNum = handCardNumber(pDefensePlayer);
  // 各プレイヤーの手札のカードがあるインデックスを取得
  int attackCardIndex;
  int defenseCardIndex;
  for(int i=0; i<NUMBER_OF_HAND; i++){
    if(pAttackPlayer->hand[i] != 0)
      attackCardIndex = i;
    if(pDefensePlayer->hand[i] != 0)
      defenseCardIndex = i;
  }
  // カードを交換する
  printf("  %s -> お互いの手札のカードを交換\n", pAttackPlayer->name);
  pAttackPlayer->hand[attackCardIndex] = defenseCardNum;
  pDefensePlayer->hand[defenseCardIndex] = attackCardNum;
}

// 9:皇帝の効果『公開処刑』
void emperor(GameManage* pGameManage, Player* pAttackPlayer, Player* pDefensePlayer, int cardNum){
  printf("  %s -> \x1b[41m[%d:%s]の効果『%s』\x1b[49m\n", pAttackPlayer->name, cardNum, CARD_NAME[cardNum], CARD_EFFECT[cardNum]);
  // 山札が1枚以上であることを確認
  if(NUMBER_OF_CARD - deckZeroCount(pGameManage) > 0){
    // 相手にカードを1枚引かせる
    printf("  %s -> %sに山札からカードを1枚引かせる\n", pAttackPlayer->name, pDefensePlayer->name);
    deckCard_to_hand(pGameManage, pDefensePlayer);
    // 相手の手札をオープン
    printf("  %s -> %sの手札... ", pAttackPlayer->name, pDefensePlayer->name);
    for(int i=0; i<NUMBER_OF_HAND; i++){
      printf(" [%d:%s] ", pDefensePlayer->hand[i], CARD_NAME[pDefensePlayer->hand[i]]);
    }
    printf("\n");
    // 捨てさせるカードを選択
    int discardCardNum;
    printf("  %s -> %sの手札から捨てるカードを選択\n", pAttackPlayer->name, pDefensePlayer->name);
    // 攻撃側がcomputerの場合 -> 設定したパラメータを元に捨てさせるカードをランダムに選択
    if(pAttackPlayer->computer){
      discardCardNum = randomHandCard(pGameManage, pDefensePlayer);
    // 攻撃側がhumanの場合 -> 捨てさせるカードを選択（コマンド入力)
    } else {
      discardCardNum = selectCardNum(pGameManage, pDefensePlayer, 2, true, false);
    }
    // 選択したカードを表示
    printf("  %s -> [%d:%s]を選択", pAttackPlayer->name, discardCardNum, CARD_NAME[discardCardNum]);
    // 捨てさせるカードが10の場合
    if(discardCardNum == 10){
      printf("  %s -> %sの効果『%s』により、%sの英雄を公開処刑！！\n", pAttackPlayer->name, CARD_NAME[cardNum], CARD_EFFECT[cardNum], pDefensePlayer->name);
      // 勝敗を決める
      gameSet(pGameManage, pAttackPlayer);
    }
    // カードを捨てる処理
    handCard_to_discardArea(pDefensePlayer, discardCardNum, true);
  } else {
    printf("山札が0枚のため、ゲーム終了\n");
  }
}

// 10:英雄の効果『転生』
void hero(GameManage* pGameManage, Player* pDefensePlayer, int cardNum){
  printf("  %s -> \x1b[47m[%d:%s]の効果『%s』\x1b[49m\n", pDefensePlayer->name, cardNum, CARD_NAME[cardNum], CARD_EFFECT[cardNum]);
  // 手札のカード（10）を捨て札に移動
  handCard_to_discardArea(pDefensePlayer, cardNum, true);
  // 転生札を手札に加える
  addCard_to_hand(pDefensePlayer, pGameManage->reincarnationCard);
}

// ************************ game set *******************************************
// 勝敗の決着
void gameSet(GameManage* pGameManage, Player* pAttackPlayer){
  // 攻撃側がプレイヤーの場合
  if(pAttackPlayer->computer == false)
    pGameManage->draw_win_lose = 1;
  // 攻撃側がコンピューターの場合
  else if(pAttackPlayer->computer == true)
    pGameManage->draw_win_lose = 2;
}

// カードナンバーの比較→勝敗の決着
void compareCardNum(GameManage* pGameManage, Player* pPlayer_1, Player* pPlayer_2){
  // 0：引き分け　1：プレイヤーの勝ち　2：プレイヤーの負け
  int gameSet = 0;
  // 手札のカードナンバーを取得
  int num_1 = handCardNumber(pPlayer_1);
  int num_2 = handCardNumber(pPlayer_2);
  // pPlayer_1がプレイヤーの場合
  if(pPlayer_1->computer == false){
    if(num_1 > num_2)
      gameSet = 1;
    else if(num_1 < num_2)
      gameSet = 2;
  // pPlayer_2がプレイヤーの場合
  } else if(pPlayer_2->computer == false){
    if(num_2 > num_1)
      gameSet = 1;
    else if(num_2 < num_1)
      gameSet = 2;
  }
  pGameManage->draw_win_lose = gameSet;
}

// 手札のカードナンバーを取得
int handCardNumber(Player* pPlayer){
  int cardNum;
  for(int i=0; i<NUMBER_OF_HAND; i++){
    if(pPlayer->hand[i] != 0){
      cardNum = pPlayer->hand[i];
      break;
    }
  }
  return cardNum;
}


// ************************ computer random operation **************************
// 手札から捨て札へ移すカードをランダムに選択
int randomHandCard(GameManage* pGameManage, Player* pPlayer){
  // 1~100の聖数値を取得
  int randNum = 1 + rand() % 100;
  // 山札のカード枚数
  int deckCardCount = NUMBER_OF_CARD - deckZeroCount(pGameManage);
  // computerとhumanで処理を分けて、パラメーターを設定
  int parameter[CARD_TYPE];
  // computerが対象の場合
  if(pPlayer->computer){
    // 手札のカード枚数で設定するパラメーターを判断
    if(deckCardCount >= (NUMBER_OF_CARD / 2 - 2))
      memcpy(parameter, COMPUTER_DISCARD_FIRST, sizeof(COMPUTER_DISCARD_FIRST));
    else
      memcpy(parameter, COMPUTER_DISCARD_LATTER, sizeof(COMPUTER_DISCARD_LATTER));
  // humanが対象の場合
  } else {
    // 手札の枚数で設定するパラメーターを判断
    if(deckCardCount >= (NUMBER_OF_CARD / 2 - 2))
      memcpy(parameter, HUMAN_DISCARD_FIRST, sizeof(HUMAN_DISCARD_FIRST));
    else
      memcpy(parameter, HUMAN_DISCARD_LATTER, sizeof(HUMAN_DISCARD_LATTER));
  }
  // 手札のカードナンバーを取得
  int cardNum_1 = pPlayer->hand[0];
  int cardNum_2 = pPlayer->hand[1];
  // カードナンバーからパラメーターの数値を取得
  int par_1 = parameter[cardNum_1];
  int par_2 = parameter[cardNum_2];
  // 分岐点の計算
  double separatePoint = 100 * (par_1 / (par_1 + par_2));
  // カードナンバーの取得
  int cardNum;
  if(cardNum <= separatePoint)
    cardNum = cardNum_1;
  else
    cardNum = cardNum_2;

  if(cardNum_1 == 10){
    cardNum = cardNum_2;
  } else if(cardNum_2 == 10){
    cardNum = cardNum_1;
  }
  return cardNum;
}

// 賢者の効果『選択』により、カードをランダムに選択
int randomWiseManSelect(GameManage* pGameManage, Player* pPlayer, int drawCount){
  // 1~100の聖数値を取得
  int randNum = 1 + rand() % 100;
  // 山札のカード枚数
  int deckCardCount = NUMBER_OF_CARD - deckZeroCount(pGameManage);
  // 山札のカード枚数に分けて、パラメーターの取得
  int parameter[CARD_TYPE];
  if(deckCardCount >= (NUMBER_OF_CARD / 2 - 2))
    memcpy(parameter, WISEMAN_EFFECT_FIRST, sizeof(WISEMAN_EFFECT_FIRST));
  else
    memcpy(parameter, WISEMAN_EFFECT_LATER, sizeof(WISEMAN_EFFECT_LATER));
  // 引いたカードナンバー
  int cardNum_1 = pGameManage->wiseManSelectCard[0];
  int cardNum_2 = pGameManage->wiseManSelectCard[1];
  // 引いたカードが3枚の場合
  int cardNum_3;
  if(drawCount == 3)
    cardNum_3 = pGameManage->wiseManSelectCard[2];
  // カードナンバーからパラメーターの数値を取得
  int par_1 = parameter[cardNum_1];
  int par_2 = parameter[cardNum_2];
  // 引いたカードが3枚の場合
  int par_3;
  if(drawCount == 3)
    par_3 = parameter[cardNum_3];
  // 取得するカードナンバー
  int cardNum;
  // 引いたカードが3枚の場合
  if(drawCount == 3){
    // 分岐点の計算
    double separatePoint_1 = 100 * (par_1 / (par_1 + par_2 + par_3));
    double separatePoint_2 = 100 * (par_2 / (par_1 + par_2 + par_3)) + separatePoint_1;
    // カードナンバーの取得
    if(cardNum <= separatePoint_1)
      cardNum = cardNum_1;
    else if(cardNum <= separatePoint_2)
      cardNum = cardNum_2;
    else
      cardNum = cardNum_3;
  // 引いたカードが2枚の場合
  } else if(drawCount == 2){
    // 分岐点の計算
    double separatePoint = 100 * (par_1 / (par_1 + par_2 + par_3));
    // カードナンバーの取得
    if(cardNum <= separatePoint)
      cardNum = cardNum_1;
    else
      cardNum = cardNum_2;
  }
  return cardNum;
}

// 兵士の効果『捜査』により、カードをランダムに選択
int randomSoldierSelect(GameManage* pGameManage, Player* pHumanPlayer, Player* pComputerPlayer){
  // 場に出たカード以外のカード枚数の取得　（山札の枚数 ＋ 転生札 ＋ humanの手札）
  int cardCount = (NUMBER_OF_CARD - deckZeroCount(pGameManage)) + 1 + 1;
  // 上記のカードを取得
  int card[cardCount];
  // 転生札のカードを追加
  card[0] = pGameManage->reincarnationCard;
  // humanの手札のカードを追加
  card[1] = handCardNumber(pHumanPlayer);
  // 山札のカードを追加
  for(int i=deckZeroCount(pGameManage)+2; i<NUMBER_OF_CARD; i++){
    if(pGameManage->deck[i] != 0){
      card[i] = pGameManage->deck[i];
    }
  }
  // ランダムにカードを選択
  int cardNum = card[rand() % cardCount];
  return cardNum;
}

// ************************ print game field ************************************
// ゲームのフィールド状況を表示
void printGameField(GameManage* pGameManage, Player* pPlayer_1, Player* pPlayer_2){
  // humanとcomputerの変数を決める
  Player* pHumanPlayer;
  Player* pComputerPlayer;
  if(pPlayer_1->computer){
    pHumanPlayer = pPlayer_2;
    pComputerPlayer = pPlayer_1;
  } else if(pPlayer_2->computer) {
    pHumanPlayer = pPlayer_1;
    pComputerPlayer = pPlayer_2;
  }

  printf("+=============================================================+\n");
  printf("|                                                             |\n");
  // computerの手札を表示
  for(int i=0; i<NUMBER_OF_HAND; i++){
    if(pComputerPlayer->hand[i] != 0){
      printf("              [  *  ]  ");
      // int cardNum = pComputerPlayer->hand[i];
      // printf("              [%d:%s]  ", cardNum, CARD_NAME[cardNum]);
    }
  }
  printf("\n\n");
  // computerの捨て札を表示
  printf("     +---+---+---+---+---+---+---+---+---+---+---+---+---+     \n");
  printf("     |");
  for(int i=NUMBER_OF_DISCARDAREA-1; i>=0; i--){
    if(pComputerPlayer->discardArea[i] == 0)
      printf("   |");
    else
      printf("%2d |", pComputerPlayer->discardArea[i]);
  }
  printf("\n");
  printf("     +---+---+---+---+---+---+---+---+---+---+---+---+---+     \n");
  printf("\n");
  // 山札と転生札を表示
  printf("                   +----+           +-----+\n");
  printf("                   | %2d |           |",  NUMBER_OF_CARD - deckZeroCount(pGameManage));
  if(pGameManage->reincarnationCard > 0)
    printf("  *  |\n");
  else
    printf("     |\n");
  printf("                   +----+           +-----+\n");
  printf("\n");
  // humanの捨て札を表示
  printf("     +---+---+---+---+---+---+---+---+---+---+---+---+---+     \n");
  printf("     |");
  for(int i=0; i<NUMBER_OF_DISCARDAREA; i++){
    if(pHumanPlayer->discardArea[i] == 0)
      printf("   |");
    else
      printf("%2d |", pHumanPlayer->discardArea[i]);
  }
  printf("\n");
  printf("     +---+---+---+---+---+---+---+---+---+---+---+---+---+     \n");
  printf("\n");
  // humanの手札を表示
  for(int i=0; i<NUMBER_OF_HAND; i++){
    if(pHumanPlayer->hand[i] != 0){
      int cardNum = pHumanPlayer->hand[i];
      printf("              [%d:%s]  ", cardNum, CARD_NAME[cardNum]);
    }
  }
  printf("\n");
  printf("|                                                             |\n");
  printf("+=============================================================+\n");














}
