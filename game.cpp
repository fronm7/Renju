#include "game.h"
#include "UI.h"
#include <iostream>
void Game::initGame(GameState& state) {
    if (history.empty()) {
        // 初始化棋盘，所有位置为空（0）
        state.board.clear(); // 清空棋盘
        state.board.resize(BOARD_SIZE, std::vector<int>(BOARD_SIZE, 0)); // 15x15 的棋盘，初始值为0

        // 设置当前玩家为黑子（1）
        state.currentPlayer = 1;

        // 初始化游戏结束标志为 false
        state.isGameOver = false;

        //游戏未结束
        state.winner = 0;
    }
    else {
        state = getCurrentState();
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
        while (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE && state.board[nx][ny] == player) {
            count++;
            nx += dx[dir];
            ny += dy[dir];
        }
        nx = x - dx[dir], ny = y - dy[dir];
        while (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE && state.board[nx][ny] == player) {
            count++;
            nx -= dx[dir];
            ny -= dy[dir];
        }
        if (player == 1){
            if (count == 5) {
                state.winner = player; // 设置胜利者
                state.isGameOver = true; // 设置游戏结束标志
                return true;
            }
        }
        else {
            if (count >= 5) {
                state.winner = player; // 设置胜利者
                state.isGameOver = true; // 设置游戏结束标志
                return true;
            }
        }
    }
    return false;
}

bool Game::isValidMove(const GameState& state, int x, int y) {
    // 检查坐标是否在棋盘范围内
    if (x < 0 || x >= BOARD_SIZE || y < 0 || y >= BOARD_SIZE) {
        return false; // 超出棋盘范围
    }

    // 检查该位置是否为空
    if (state.board[x][y] != 0 && state.board[x][y] != 3) {
        return false; // 该位置已有棋子
    }

    return true; // 落子合法
}

bool Game::undoMove(GameState& state) {
    // 检查历史记录是否至少有两个状态
    if (history.size() < 2) {
        return false; // 历史记录不足，无法悔两步
    }

    // 恢复到上上一个游戏状态（即玩家的上一个回合）
    history.pop();// 移除AI的落子状态
    history.pop(); // 移除玩家的上一个落子状态

    if (history.empty()) {
        initGame(state);
    }
    else {
        state = history.top(); 
        state.currentPlayer = state.init_choice; // 恢复到玩家的上一个状态
    }
    
    return true;
}

void Game::makeMove(GameState& state, int x, int y) {
    // 检查落子是否合法
    if (!isValidMove(state, x, y)) {
        return; // 落子不合法，直接返回
    }

    // 更新棋盘状态
    state.board[x][y] = state.currentPlayer;

    // 切换当前玩家
    state.currentPlayer = (state.currentPlayer == 1) ? 2 : 1;

    // 保存当前游戏状态到历史记录中
    history.push(state);

    //std::cout << "目前是" << state.currentPlayer << "落子" << " " << "history大小为" << history.size() << std::endl;
    
    // 检查是否胜利
    if (checkWin(state, x, y)) {
        return;
    }

    // 临时切换当前玩家
    state.currentPlayer = (state.currentPlayer == 1) ? 2 : 1;

    if (state.currentPlayer == 1 && isForbiddenMove(state, x, y)) { // 该位置为禁手
        std::cout << "目前是" << state.currentPlayer << "落子,是禁手" << std::endl;
        state.isGameOver = true; // 游戏结束
        state.winner = 2; // 白方获胜
        return;
    }
    std::cout << "目前是" << state.currentPlayer << "落子,不是禁手" << std::endl;

    // 重新切换当前玩家
    state.currentPlayer = (state.currentPlayer == 1) ? 2 : 1;

    return;
}

int Game::evaluate(GameState& state, int x, int y) {
    int totalScore = 0;
    // 检查四个方向
    int dx[] = { 1, 0, 1, 1 };
    int dy[] = { 0, 1, 1, -1 };
    for (int dir = 0; dir < 4; ++dir) {
        // 以 (x, y) 为中心，向两个方向扩展，找到所有包含 (x, y) 的五元组
        for (int offset = -4; offset <= 0; ++offset) {
            int startX = x + offset * dx[dir];
            int startY = y + offset * dy[dir];
            int blackCount = 0, whiteCount = 0;// 统计五元组中的黑棋和白棋数量
            // 检查五元组是否在棋盘内
            if (isInBoard(startX, startY) && isInBoard(startX + 4 * dx[dir], startY + 4 * dy[dir])) {  
                for (int i = 0; i < 5; ++i) {
                    int currentX = startX + i * dx[dir];
                    int currentY = startY + i * dy[dir];
                    if (state.board[currentX][currentY] == 1) blackCount++;
                    else if (state.board[currentX][currentY] == 2) whiteCount++;
                }
            }
            if (state.init_choice == 1) {// 交换敌我
                blackCount += whiteCount;
                whiteCount = blackCount - whiteCount;
                blackCount -= whiteCount;
            }
            // 依据敌我子数评分
            if (blackCount == 0 && whiteCount == 0)		totalScore += 10;
            else if (blackCount == 1 && whiteCount == 0)	totalScore += 35;
            else if (blackCount == 2 && whiteCount == 0)	totalScore += 1500;
            else if (blackCount == 3 && whiteCount == 0)	totalScore += 18000;
            else if (blackCount == 4 && whiteCount == 0)	totalScore += 1000000;
            else if (blackCount && whiteCount == 1)	totalScore += 15;
            else if (blackCount == 0 && whiteCount == 2)	totalScore += 400;
            else if (blackCount == 0 && whiteCount == 3)	totalScore += 6000;
            else if (blackCount == 0 && whiteCount == 4)	totalScore += 150000;
            else if (blackCount != 0 && whiteCount != 0)	totalScore += 0;
        }
    }
    return totalScore;
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

    int x = validMoves[0].first, y = validMoves[0].second, present_score = 0;

    //if (validMoves.size() == BOARD_SIZE * BOARD_SIZE - 1) {
    //     //随机选择一个合法的落子位置
    //    int index = rand() % validMoves.size(); // 随机索引
    //    x = validMoves[index].first;
    //    y = validMoves[index].second;
    //}
    //else {
    //    for (auto& point : validMoves) {
    //        if (evaluate(state, point.first, point.second) >= present_score) {
    //            x = point.first;
    //            y = point.second;
    //            present_score = evaluate(state, point.first, point.second);
    //        }
    //    }
    //}

    //随机选择一个合法的落子位置
    int index = rand() % validMoves.size(); // 随机索引
    x = validMoves[index].first;
    y = validMoves[index].second;

    // 执行落子
    makeMove(state, x, y);
}

void Game::setStone(GameState& state, int x, int y, int cStone) {
    state.board[x][y] = cStone; 
    return;
}

bool Game::isFive(const GameState& state, int x, int y) {
    int player = state.currentPlayer; // 当前玩家

    // 方向数组：水平、垂直、两个对角线
    int dx[] = { 1, 0, 1, 1 };
    int dy[] = { 0, 1, 1, -1 };

    for (int dir = 0; dir < 4; dir++) {
        int count = 1; // 当前方向的连续棋子数
        // 向正方向检查
        int nx = x + dx[dir], ny = y + dy[dir];
        while (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE && state.board[nx][ny] == player) {
            count++;
            nx += dx[dir];
            ny += dy[dir];
        }
        // 向反方向检查
        nx = x - dx[dir], ny = y - dy[dir];
        while (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE && state.board[nx][ny] == player) {
            count++;
            nx -= dx[dir];
            ny -= dy[dir];
        }
        // 判断是否形成五连
        if (player == 1) { // 黑子必须恰好五子
            if (count == 5) {
                return true;
            }
        }
        else if (player == 2) { // 白子可以五子或更多
            if (count >= 5) {
                return true;
            }
        }
    }
    return false;
}

bool Game::isOverline(const GameState& state, int x, int y) {
    int player = state.currentPlayer; // 当前玩家

    if (player != 1) return false; // 只有黑子需要检查长连

    // 方向数组：水平、垂直、两个对角线
    int dx[] = { 1, 0, 1, 1 };
    int dy[] = { 0, 1, 1, -1 };

    for (int dir = 0; dir < 4; dir++) {
        int count = 1; // 当前方向的连续棋子数
        // 向正方向检查
        int nx = x + dx[dir], ny = y + dy[dir];
        while (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE && state.board[nx][ny] == player) {
            count++;
            nx += dx[dir];
            ny += dy[dir];
        }
        // 向反方向检查
        nx = x - dx[dir], ny = y - dy[dir];
        while (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE && state.board[nx][ny] == player) {
            count++;
            nx -= dx[dir];
            ny -= dy[dir];
        }
        // 判断是否形成长连（超过五子）
        if (count > 5) {
            return true;
        }
    }
    return false;
}

bool Game::isFour(const GameState& state, int x, int y, int nDir) {
    int player = state.currentPlayer; // 当前玩家

    // 方向数组：水平、垂直、两个对角线
    int dx[] = { 1, 0, 1, 1 };
    int dy[] = { 0, 1, 1, -1 };

    // 检查是否形成五连
    if (isFive(state, x, y)) {
        return false;
    }

    // 检查黑方的长连禁手
    if (player == 1 && isOverline(state, x, y)) {
        return false;
    }

    // 模拟放置棋子
    int count = 1; // 当前方向的连续棋子数

    // 向正方向检查
    int nx = x + dx[nDir - 1], ny = y + dy[nDir - 1];
    while (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE && state.board[nx][ny] == player) {
        count++;
        nx += dx[nDir - 1];
        ny += dy[nDir - 1];
    }

    // 向反方向检查
    nx = x - dx[nDir - 1], ny = y - dy[nDir - 1];
    while (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE && state.board[nx][ny] == player) {
        count++;
        nx -= dx[nDir - 1];
        ny -= dy[nDir - 1];
    }

    // 判断是否形成四连
    if (count == 4) {
        return true;
    }

    return false;
}

int Game::isOpenFour(const GameState& state, int x, int y, int nDir) {
    int player = state.currentPlayer; // 当前玩家

    // 检查是否已经形成了五子连线
    if (isFive(state, x, y)) {
        return 0; // 已经形成五子连线，不是活四
    }

    // 检查黑棋是否有长连禁手
    if (player == 1 && isOverline(state, x, y)) {
        return 0; // 黑棋有长连禁手，不是活四
    }

    // 方向数组：水平、垂直、两个对角线
    int dx[] = { 1, 0, 1, 1 };
    int dy[] = { 0, 1, 1, -1 };

    // 检查四个方向
    for (int dir = 0; dir < 4; ++dir) {
        int count = 1; // 当前方向的连续棋子数
        int openEnds = 0; // 开放端的数量

        // 向正方向检查
        int nx = x + dx[dir], ny = y + dy[dir];
        while (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE && state.board[nx][ny] == player) {
            count++;
            nx += dx[dir];
            ny += dy[dir];
        }
        // 检查正方向是否开放
        if (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE && state.board[nx][ny] == 0) {
            openEnds++;
        }

        // 向反方向检查
        nx = x - dx[dir], ny = y - dy[dir];
        while (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE && state.board[nx][ny] == player) {
            count++;
            nx -= dx[dir];
            ny -= dy[dir];
        }
        // 检查反方向是否开放
        if (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE && state.board[nx][ny] == 0) {
            openEnds++;
        }

        // 判断是否形成活四
        if (count == 4 && openEnds >= 1) {
            return 1; // 形成活四
        }
    }

    return 0; // 没有形成活四
}

bool Game::isOpenThree(const GameState& state, int x, int y, int nDir) {
    int player = state.currentPlayer; // 当前玩家

    // 检查是否已经形成了五子连线
    if (isFive(state, x, y)) {
        return false; // 已经形成五子连线，不是活三
    }

    // 检查黑棋是否有长连禁手
    if (player == 1 && isOverline(state, x, y)) {
        return false; // 黑棋有长连禁手，不是活三
    }

    // 方向数组：水平、垂直、两个对角线
    int dx[] = { 1, 0, 1, 1 };
    int dy[] = { 0, 1, 1, -1 };

    // 检查四个方向
    for (int dir = 0; dir < 4; ++dir) {
        int count = 1; // 当前方向的连续棋子数
        int openEnds = 0; // 开放端的数量

        // 向正方向检查
        int nx = x + dx[dir], ny = y + dy[dir];
        while (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE && state.board[nx][ny] == player) {
            count++;
            nx += dx[dir];
            ny += dy[dir];
        }

        // 检查正方向是否开放
        if (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE && state.board[nx][ny] == 0) {
            // 检查空位是否为禁手
            if (!isForbiddenMove(state, nx, ny)) {
                // 检查是否满足开放四连且没有双四和双三阻挡
                if ((isOpenFour(state, nx, ny, dir) == 1) &&
                    (!isDoubleFour(state, nx, ny)) &&
                    (!isDoubleThree(state, nx, ny))) {
                    openEnds++; // 空位有效，开放端计数增加
                }
            }
        }

        // 向反方向检查
        nx = x - dx[dir], ny = y - dy[dir];
        while (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE && state.board[nx][ny] == player) {
            count++;
            nx -= dx[dir];
            ny -= dy[dir];
        }
        // 检查反方向是否开放
        if (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE && state.board[nx][ny] == 0) {
            // 检查空位是否为禁手
            if (!isForbiddenMove(state, nx, ny)) {
                // 检查是否满足开放四连且没有双四和双三阻挡
                if ((isOpenFour(state, nx, ny, dir) == 1) &&
                    (!isDoubleFour(state, nx, ny)) &&
                    (!isDoubleThree(state, nx, ny))) {
                    openEnds++; // 空位有效，开放端计数增加
                }
            }
        }

        // 判断是否形成活三
        if (count == 3 && openEnds >= 1) {
            // 检查是否形成双四或双三
            if (!isDoubleFour(state, x, y) && !isDoubleThree(state, x, y)) {
                return true; // 形成活三
            }
        }
    }

    return false; // 没有形成活三
}

bool Game::isDoubleThree(const GameState& state, int x, int y) {
    int player = state.currentPlayer; // 当前玩家

    // 检查是否已经形成了五子连线
    if (isFive(state, x, y)) {
        return false; // 已经形成五子连线，不是双三
    }

    //// 检查黑棋是否有长连禁手
    //if (player == 1 && isOverline(state, x, y)) {
    //    return false; // 黑棋有长连禁手，不是双三
    //}

    int threeCount = 0; // 记录活三的数量

    // 方向数组：水平、垂直、两个对角线
    int dx[] = { 1, 0, 1, 1 };
    int dy[] = { 0, 1, 1, -1 };

    // 检查四个方向
    for (int dir = 0; dir < 4; ++dir) {
        // 检查当前方向是否形成活三
        if (isOpenThree(state, x, y, dir)) {
            threeCount++;
        }
    }

    // 如果活三的数量大于等于2，则返回true，表示形成双三
    return threeCount >= 2;
}

bool Game::isDoubleFour(const GameState& state, int x, int y) {
    int player = state.currentPlayer; // 当前玩家

    // 检查是否已经形成了五子连线
    if (isFive(state, x, y)) {
        return false; // 已经形成五子连线，不是双四
    }

    //// 检查黑棋是否有长连禁手
    //if (player == 1 && isOverline(state, x, y)) {
    //    return false; // 黑棋有长连禁手，不是双四
    //}

    int fourCount = 0; // 记录四连的数量

    // 方向数组：水平、垂直、两个对角线
    int dx[] = { 1, 0, 1, 1 };
    int dy[] = { 0, 1, 1, -1 };

    // 检查四个方向
    for (int dir = 0; dir < 4; ++dir) {
        // 检查当前方向是否形成活四
        if (isOpenFour(state, x, y, dir) == 1) {
            fourCount += 2; // 活四算作两个四连
        }
        // 检查当前方向是否形成普通四连
        else if (isFour(state, x, y, dir)) {
            fourCount++;
        }
    }

    // 如果四连的数量大于等于2，则返回true，表示形成双四
    return fourCount >= 2;
}

bool Game::isForbiddenMove(const GameState& state, int x, int y) {
    // 仅对黑方进行禁手检查
    if (state.currentPlayer != 1) {
        return false; 
    }

    // 检查是否形成了五连
    if (isFive(state, x, y)) {
        return false; // 形成五连，不是禁手
    }

    // 检查是否形成了长连禁手
    if (isOverline(state, x, y)) {
        return true; // 形成长连禁手
    }

    // 检查是否形成了双三禁手
    if (isDoubleThree(state, x, y)) {
        return true; // 形成双三禁手
    }

    // 检查是否形成了双四禁手
    if (isDoubleFour(state, x, y)) {
        return true; // 形成双四禁手
    }

    // 如果没有形成任何禁手，则返回 false
    return false;
}