#ifndef GAME_H
#define GAME_H

#include <vector>
#include <stack>
#include <stdexcept>
// 棋盘大小
const int BOARD_SIZE = 15;
// 游戏状态类
class GameState {
public:
    std::vector<std::vector<int>> board; // 棋盘状态（0: 空, 1: 黑子, 2: 白子, 3: 禁手）
    int currentPlayer; // 当前玩家（1: 黑方, 2: 白方）
    bool isGameOver; // 游戏是否结束
    int winner; // 胜利者（0: 未结束, 1: 黑方胜利, 2: 白方胜利）
    int init_choice; //玩家初始选择

    // 默认构造函数
    GameState() : currentPlayer(1), isGameOver(false), winner(0), init_choice(1){
        board.resize(BOARD_SIZE, std::vector<int>(BOARD_SIZE, 0)); // 初始化 15x15 的棋盘
    }

    // 拷贝构造函数
    GameState(const GameState& other)
        : board(other.board),
        currentPlayer(other.currentPlayer),
        isGameOver(other.isGameOver),
        winner(other.winner),
        init_choice(other.init_choice){
    }
};

class Game {
private:
    // 判断坐标是否在棋盘内
    bool isInBoard(int x, int y) {
        return x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE;
    }
    std::stack<GameState> history; // 悔棋历史记录
public:
    // 获取当前游戏状态
    const GameState& getCurrentState() const {
        if (history.empty()) {
            throw std::runtime_error("History is empty, no current state available.");
        }
        return history.top();
    }

    // 获取历史记录的大小
    size_t getHistorySize() const {
        return history.size();
    }

    // 获取历史记录（返回一个栈的副本）
    std::stack<GameState> getHistory() const {
        return history;
    }

    // 设置历史记录
    void setHistory(const std::stack<GameState>& newHistory) {
        history = newHistory;
    }

    //计算回合数
    int turnnumber() {
        if (history.empty()) return 0;
        else return static_cast<int>(history.size());
    }

    // 初始化游戏
    void initGame(GameState& state);

    // 判断落子是否合法
    bool isValidMove(const GameState& state, int x, int y);

    // 判断是否胜利
    bool checkWin(GameState& state, int x, int y);

    // 在指定位置放置棋子
    void setStone(GameState& state, int x, int y, int cStone);

    // 判断是否禁手（仅对黑方）
    bool isForbiddenMove(const GameState& state, int x, int y);

    // 判断是否形成了五子连线
    bool isFive(const GameState& state, int x, int y);

    // 判断是否形成了长连（超过五子）
    bool isOverline(const GameState& state, int x, int y);

    // 在指定方向判断是否形成了四子连线
    bool isFour(const GameState& state, int x, int y, int nDir);

    // 在指定方向判断是否有活四（四子连线且两端为空位）
    int isOpenFour(const GameState& state, int x, int y, int nDir);

    // 在指定方向判断是否有活三（三子连线且两端至少有一个空位）
    bool isOpenThree(const GameState& state, int x, int y, int nDir);

    // 判断是否形成了双四（两个四子连线共享一个端点）
    bool isDoubleFour(const GameState& state, int x, int y);

    // 判断是否形成了双三（两个三子连线共享一个端点）
    bool isDoubleThree(const GameState& state, int x, int y);

    // 执行落子
    void makeMove(GameState& state, int x, int y);

    // 悔棋
    bool undoMove(GameState& state); 

    // 评估函数
    int evaluate(GameState& state, int x, int y);

    // 简单 AI 落子
    void makeAIMove(GameState& state);
};

#endif // GAME_H