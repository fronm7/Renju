#include "game.h"
#include "UI.h"

void Game::initGame(GameState& state) {
    // 初始化棋盘，所有位置为空（0）
    state.board.clear(); // 清空棋盘
    state.board.resize(15, std::vector<int>(15, 0)); // 15x15 的棋盘，初始值为0

    // 设置当前玩家为黑子（1）
    state.currentPlayer = 1;

    // 初始化游戏结束标志为 false
    state.isGameOver = false;

    //游戏未结束
    state.winner = 0;

    // 清空悔棋历史记录
    while (!history.empty()) {
        history.pop();
    }
}

bool Game::checkWin(GameState& state, int x, int y) {
    int player = state.board[x][y];
    if (player == 0) return false; // 该位置为空，不可能胜利

    // 检查四个方向
    int dx[] = { 1, 0, 1, 1 };
    int dy[] = { 0, 1, 1, -1 };

    for (int dir = 0; dir < 4; dir++) {
        int count = 1;
        int nx = x + dx[dir], ny = y + dy[dir];
        while (nx >= 0 && nx < 15 && ny >= 0 && ny < 15 && state.board[nx][ny] == player) {
            count++;
            nx += dx[dir];
            ny += dy[dir];
        }
        nx = x - dx[dir], ny = y - dy[dir];
        while (nx >= 0 && nx < 15 && ny >= 0 && ny < 15 && state.board[nx][ny] == player) {
            count++;
            nx -= dx[dir];
            ny -= dy[dir];
        }
        if (count >= 5) {
            state.winner = player; // 设置胜利者
            state.isGameOver = true; // 设置游戏结束标志
            return true;
        }
    }
    return false;
}

bool Game::isValidMove(const GameState& state, int x, int y) {
    // 检查坐标是否在棋盘范围内
    if (x < 0 || x >= 15 || y < 0 || y >= 15) {
        return false; // 超出棋盘范围
    }

    // 检查该位置是否为空
    if (state.board[x][y] != 0) {
        return false; // 该位置已有棋子
    }

    return true; // 落子合法
}

void Game::undoMove(GameState& state) {
    // 检查历史记录是否至少有两个状态
    if (history.size() < 2) {
        return; // 历史记录不足，无法悔两步
    }

    // 恢复到上上一个游戏状态（即玩家的上一个回合）
    history.pop(); // 移除AI的落子状态
    state = history.top(); // 恢复到玩家的上一个状态

    // 移除玩家的上一个状态
    history.pop();
}

void Game::makeMove(GameState& state, int x, int y) {
    // 检查落子是否合法
    if (!isValidMove(state, x, y)) {
        return; // 落子不合法，直接返回
    }

    // 保存当前游戏状态到历史记录中
    history.push(state);

    // 更新棋盘状态
    state.board[x][y] = state.currentPlayer;

    // 检查是否胜利
    if (checkWin(state, x, y)) {
        state.isGameOver = true; // 游戏结束
        return;
    }

    // 切换当前玩家
    state.currentPlayer = (state.currentPlayer == 1) ? 2 : 1;
}

void Game::makeAIMove(GameState& state) {
    // 检查游戏是否已经结束
    if (state.isGameOver) {
        return; // 游戏已结束，AI 不落子
    }

    // 生成所有合法的落子位置
    std::vector<std::pair<int, int>> validMoves;
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            if (state.board[i][j] == 0) { // 空位
                validMoves.push_back({ i, j });
            }
        }
    }

    // 如果没有合法的落子位置，直接返回
    if (validMoves.empty()) {
        return;
    }

    // 随机选择一个合法的落子位置
    int index = rand() % validMoves.size(); // 随机索引
    int x = validMoves[index].first;
    int y = validMoves[index].second;

    // 执行落子
    makeMove(state, x, y);
}