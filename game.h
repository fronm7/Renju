#ifndef GAME_H
#define GAME_H

#include <vector>
#include <stack>

// 游戏状态类
class GameState {
public:
    std::vector<std::vector<int>> board; // 棋盘状态（0: 空, 1: 黑子, 2: 白子）
    int currentPlayer; // 当前玩家（1: 黑方, 2: 白方）
    bool isGameOver; // 游戏是否结束
    int winner; // 胜利者（0: 未结束, 1: 黑方胜利, 2: 白方胜利）

    // 默认构造函数
    GameState() : currentPlayer(1), isGameOver(false), winner(0) {
        board.resize(15, std::vector<int>(15, 0)); // 初始化 15x15 的棋盘
    }

    // 拷贝构造函数
    GameState(const GameState& other)
        : board(other.board),
        currentPlayer(other.currentPlayer),
        isGameOver(other.isGameOver),
        winner(other.winner) {
        // history 不需要深拷贝，因为 undoMove 会重新构建历史记录
    }
};

class Game {
private:
    std::stack<GameState> history; // 悔棋历史记录
public:
    // 初始化游戏
    void initGame(GameState& state);

    // 判断落子是否合法
    bool isValidMove(const GameState& state, int x, int y);

    // 判断是否胜利
    bool checkWin(GameState& state, int x, int y);

    //// 判断是否禁手（仅对黑方）
    //bool isForbiddenMove(const GameState& state, int x, int y);

    // 执行落子
    void makeMove(GameState& state, int x, int y);

    // 悔棋
    void undoMove(GameState& state);

    // 简单 AI 落子
    void makeAIMove(GameState& state);
};

#endif // GAME_H