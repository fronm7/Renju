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

        //落子位置回退为非法
        state.lastMoveX = -1;
        state.lastMoveY = -1;
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

    if (isADraw(state)) return;

    // 判断禁手
    bool is_forbidden = isForbiddenMove(state, x, y);

    // 更新棋盘状态
    state.board[x][y] = state.currentPlayer;

    // 更新最近一次落子的位置
    state.lastMoveX = x;
    state.lastMoveY = y;

    // 切换当前玩家
    state.currentPlayer = (state.currentPlayer == 1) ? 2 : 1;

    // 保存当前游戏状态到历史记录中
    history.push(state);

    //std::cout << "目前是" << state.currentPlayer << "落子" << " " << "history大小为" << history.size() << std::endl;
    
    // 检查是否胜利
    if (checkWin(state, x, y)) {
        return;
    }

    if (is_forbidden) { // 该位置为禁手
        //std::cout << "目前是" << state.currentPlayer << "落子,是禁手" << std::endl;
        state.isGameOver = true; // 游戏结束
        state.winner = 2; // 白方获胜
        return;
    }
    /*std::cout << "目前是" << state.currentPlayer << "落子,不是禁手" << std::endl;*/

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
            //if (blackCount == 0 && whiteCount == 0)		totalScore += 10;
            //else if (blackCount == 1 && whiteCount == 0)	totalScore += 35;
            //else if (blackCount == 2 && whiteCount == 0)	totalScore += 1500;
            //else if (blackCount == 3 && whiteCount == 0)	totalScore += 18000;
            //else if (blackCount == 4 && whiteCount == 0)	totalScore += 1000000;
            //else if (blackCount && whiteCount == 1)	totalScore += 15;
            //else if (blackCount == 0 && whiteCount == 2)	totalScore += 400;
            //else if (blackCount == 0 && whiteCount == 3)	totalScore += 6000;
            //else if (blackCount == 0 && whiteCount == 4)	totalScore += 150000;
            //else if (blackCount != 0 && whiteCount != 0)	totalScore += 0;
            if (blackCount == 0 && whiteCount == 0) totalScore += 7;
            if (blackCount == 1 && whiteCount == 0) totalScore += 35;
            if (blackCount == 2 && whiteCount == 0) totalScore += 800;
            if (blackCount == 3 && whiteCount == 0) totalScore += 15000;
            if (blackCount == 4 && whiteCount == 0) totalScore += 800000;
            if (blackCount == 0 && whiteCount == 1) totalScore += 15;
            if (blackCount == 0 && whiteCount == 2) totalScore += 400;
            if (blackCount == 0 && whiteCount == 3) totalScore += 1800;
            if (blackCount == 0 && whiteCount == 4) totalScore += 100000;
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

    if (/*validMoves.size() == BOARD_SIZE * BOARD_SIZE - 1*/false) {
         //随机选择一个合法的落子位置
        int index = rand() % validMoves.size(); // 随机索引
        x = validMoves[index].first;
        y = validMoves[index].second;
    }
    else {
        for (auto& point : validMoves) {
            if (evaluate(state, point.first, point.second) >= present_score) {
                if (state.init_choice == 1) {
                    // 临时切换当前玩家
                    state.currentPlayer = (state.currentPlayer == 1) ? 2 : 1;
                    if (isForbiddenMove(state, point.first, point.second)) {
                        state.currentPlayer = (state.currentPlayer == 1) ? 2 : 1; // 切换回来
                        continue;
                    }
                    state.currentPlayer = (state.currentPlayer == 1) ? 2 : 1; // 切换回来
                }
                else {
                    if (isForbiddenMove(state, point.first, point.second)) {
                        continue;
                    }
                }
                x = point.first;
                y = point.second;
                present_score = evaluate(state, point.first, point.second);
            }
        }
    }

    ////随机选择一个合法的落子位置
    //int index = rand() % validMoves.size(); // 随机索引
    //x = validMoves[index].first;
    //y = validMoves[index].second;
    // 执行落子
    makeMove(state, x, y);
}

void Game::setStone(GameState& state, int x, int y, int cStone) {
    state.board[x][y] = cStone; 
    return;
}

bool Game::isFive(GameState& state, int x, int y) {
    int nColor = state.currentPlayer; // 当前玩家

    // 如果该位置已有棋子，则直接返回false
    if (state.board[x][y] != 0)
        return false;

    // 方向数组
    int dx[] = { 1, 0, 1, 1 };
    int dy[] = { 0, 1, 1, -1 };

    // 临时放置棋子进行检查
    setStone(state, x, y, nColor);

    // 检查四个方向上的五子连珠
    for (int dir = 0; dir < 4; dir++) {
        int nLine = 1;

        // 向一个方向扩展
        int i = x + dx[dir], j = y + dy[dir];
        while (isInBoard(i, j) && state.board[i][j] == nColor) {
            nLine++;
            i += dx[dir];
            j += dy[dir];
        }

        // 向相反方向扩展
        i = x - dx[dir];
        j = y - dy[dir];
        while (isInBoard(i, j) && state.board[i][j] == nColor) {
            nLine++;
            i -= dx[dir];
            j -= dy[dir];
        }

        // 黑子需要正好五子连珠，白子需要至少五子连珠
        if ((nColor == 1 && nLine == 5) || (nColor == 2 && nLine >= 5)) {
            state.board[x][y] = 0; // 恢复为空位
            return true;
        }
    }

    setStone(state, x, y, 0); // 恢复为空位
    return false;
}

bool Game::isFive(GameState& state, int x, int y, int nDir) {
    int nColor = state.currentPlayer; // 当前玩家

    // 如果该位置已有棋子，则直接返回false
    if (state.board[x][y] != 0)
        return false;

    // 方向数组
    int dx[] = { 1, 0, 1, 1 };
    int dy[] = { 0, 1, 1, -1 };

    // 临时放置棋子进行检查
    setStone(state, x, y, nColor);

    // 检查nDir方向上的五子连珠
    int nLine = 1, dir = nDir;

    // 向一个方向扩展
    int i = x + dx[dir], j = y + dy[dir];
    while (isInBoard(i, j) && state.board[i][j] == nColor) {
        nLine++;
        i += dx[dir];
        j += dy[dir];
    }

    // 向相反方向扩展
    i = x - dx[dir];
    j = y - dy[dir];
    while (isInBoard(i, j) && state.board[i][j] == nColor) {
        nLine++;
        i -= dx[dir];
        j -= dy[dir];
    }

    // 黑子需要正好五子连珠，白子需要至少五子连珠
    if ((nColor == 1 && nLine == 5) || (nColor == 2 && nLine >= 5)) {
        state.board[x][y] = 0; // 恢复为空位
        return true;
    }

    setStone(state, x, y, 0); // 恢复为空位
    return false;
}

bool Game::isOverline(GameState& state, int x, int y) {
    // 如果该位置已有棋子，则直接返回false
    if (state.board[x][y] != 0)
        return false;

    // 方向数组
    int dx[] = { 1, 0, 1, 1 };
    int dy[] = { 0, 1, 1, -1 };

    // 临时放置黑子进行检查
    setStone(state, x, y, 1); // 黑子

    bool bOverline = false;

    // 检查四个方向上的长连
    for (int dir = 0; dir < 4; dir++) {
        int nLine = 1;

        // 向一个方向扩展
        int i = x + dx[dir], j = y + dy[dir];
        while (isInBoard(i, j) && state.board[i][j] == 1) {
            nLine++;
            i += dx[dir];
            j += dy[dir];
        }

        // 向相反方向扩展
        i = x - dx[dir];
        j = y - dy[dir];
        while (isInBoard(i, j) && state.board[i][j] == 1) {
            nLine++;
            i -= dx[dir];
            j -= dy[dir];
        }

        // 如果形成五子连珠，则撤销放置并返回false（因为不是长连）
        if (nLine == 5) {
            setStone(state, x, y, 0); // 恢复为空位
            return false;
        }

        // 如果形成长连，则设置bOverline为true
        if (nLine >= 6) {
            bOverline = true;
        }
    }

    setStone(state, x, y, 0); // 恢复为空位
    return bOverline;
}

bool Game::isFour(GameState& state, int x, int y, int nDir) {
    // 如果该位置已有棋子，则直接返回false
    if (state.board[x][y] != 0)
        return false;

    // 如果放置棋子后会形成五子连珠，返回false
    if (isFive(state, x, y, 1))
        return false;
    // 如果当前玩家是黑方且放置棋子后会形成黑方的长连，返回false
    else if ((state.currentPlayer == 1) && (isOverline(state, x, y)))
        return false;
    else {
        int c = state.currentPlayer; // 获取当前玩家

        // 临时在(x, y)位置放置棋子
        setStone(state, x, y, c);

        // 方向数组
        int dx[] = { 1, 0, 1, 1 };
        int dy[] = { 0, 1, 1, -1 };

        // 根据nDir的值判断检查的方向
        int dir = nDir ; 
        int nLine = 1;

        // 向一个方向扩展
        int i = x + dx[dir], j = y + dy[dir];
        while (isInBoard(i, j) && state.board[i][j] == c) {
            nLine++;
            i += dx[dir];
            j += dy[dir];
        }

        // 向相反方向扩展
        i = x - dx[dir];
        j = y - dy[dir];
        while (isInBoard(i, j) && state.board[i][j] == c) {
            nLine++;
            i -= dx[dir];
            j -= dy[dir];
        }

        // 如果形成四子连珠，则撤销放置并返回true
        if (nLine == 4) {
            setStone(state, x, y, 0); // 恢复为空位
            return true;
        }

        setStone(state, x, y, 0); // 恢复为空位
        return false;
    }
}

int Game::isOpenFour(GameState& state, int x, int y, int nDir) {
    int nColor = state.currentPlayer; // 获取当前玩家

    // 如果该位置已有棋子，则直接返回0
    if (state.board[x][y] != 0)
        return 0;

    // 如果放置棋子后会形成五子连珠，返回0
    if (isFive(state, x, y, nColor))
        return 0;

    // 如果当前玩家是黑方且放置棋子后会形成黑方的长连，返回0
    else if ((nColor == 1) && (isOverline(state, x, y)))
        return 0;
    else {
        // 临时在(x, y)位置放置棋子
        setStone(state, x, y, nColor);

        // 方向数组
        int dx[] = { 1, 0, 1, 1 };
        int dy[] = { 0, 1, 1, -1 };

        // 根据nDir的值判断检查的方向
        int dir = nDir;
        int nLine = 1;

        // 向一个方向扩展
        int i = x + dx[dir], j = y + dy[dir];
        while (isInBoard(i, j) && state.board[i][j] == nColor) {
            nLine++;
            i += dx[dir];
            j += dy[dir];
        }
        if (isInBoard(i, j) && state.board[i][j] == 0) {
            // 如果不能成五，则不是活四
            if (!isFive(state, i , j , dir)) {
                setStone(state, x, y, 0);
                return 0;
            }
        }
        else {
            setStone(state, x, y, 0);
            return 0;
        }

        // 向相反方向扩展
        i = x - dx[dir];
        j = y - dy[dir];
        while (isInBoard(i, j) && state.board[i][j] == nColor) {
            nLine++;
            i -= dx[dir];
            j -= dy[dir];
        }
        if (isInBoard(i, j) && state.board[i][j] == 0) {
            // 如果可以成五且当前链长为4，则为活四，返回1；否则返回2
            if (isFive(state, i , j , dir)) {
                setStone(state, x, y, 0);
                return (nLine == 4 ? 1 : 2);
            }
        }

        setStone(state, x, y, 0); // 恢复为空位
        return 0;
    }
}

bool Game::isOpenThree(GameState& state, int x, int y, int nDir) {
    int nColor = state.currentPlayer; // 获取当前玩家

    // 如果该位置已有棋子，则直接返回false
    if (state.board[x][y] != 0)
        return false;

    // 如果放置棋子后会形成五子连珠，返回false
    if (isFive(state, x, y, nColor))
        return false;

    // 如果当前玩家是黑方且放置棋子后会形成黑方的长连，返回false
    else if ((nColor == 1) && (isOverline(state, x, y)))
        return false;
    else {
        // 临时在(x, y)位置放置棋子
        setStone(state, x, y, nColor);

        // 方向数组
        int dx[] = {1, 0, 1, 1};
        int dy[] = {0, 1, 1, -1};

        // 根据nDir的值判断检查的方向
        int dir = nDir; 

        // 向一个方向扩展
        int i = x + dx[dir], j = y + dy[dir];
        while (isInBoard(i, j) && state.board[i][j] == nColor) {
            i += dx[dir];
            j += dy[dir];
        }
        if (isInBoard(i, j) && state.board[i][j] == 0) {
            if ((isOpenFour(state, i , j, nDir) == 1) && 
                (!isDoubleFour(state, i, j)) &&
                (!isDoubleThree(state, i, j))) {
                setStone(state, x, y, 0);
                return true;
            }
        }

        // 向相反方向扩展
        i = x - dx[dir];
        j = y - dy[dir];
        while (isInBoard(i, j) && state.board[i][j] == nColor) {
            i -= dx[dir];
            j -= dy[dir];
        }
        if (isInBoard(i, j) && state.board[i][j] == 0) {
            if ((isOpenFour(state, i, j, nDir) == 1) &&
                (!isDoubleFour(state, i, j)) &&
                (!isDoubleThree(state, i, j))) {
                setStone(state, x, y, 0);
                return true;
            }
        }

        setStone(state, x, y, 0); // 恢复为空位
        return false;
    }
}

bool Game::isDoubleThree(GameState& state, int x, int y) {
    int nColor = state.currentPlayer; // 获取当前玩家

    // 如果该位置已有棋子，则直接返回false
    if (state.board[x][y] != 0)
        return false;

    // 如果放置棋子后会成五，返回false
    if (isFive(state, x, y))
        return false;

    int nThree = 0;
    for (int i = 0; i < 4; i++) {
        if (isOpenThree(state, x, y, i))
            nThree++;
    }

    if (nThree >= 2)
        return true;
    else
        return false;
}

bool Game::isDoubleFour(GameState& state, int x, int y) {
    int nColor = state.currentPlayer; // 获取当前玩家

    // 如果该位置已有棋子，则直接返回false
    if (state.board[x][y] != 0)
        return false;

    // 如果放置棋子后会形成五子连珠，返回false
    if (isFive(state, x, y, nColor))
        return false;

    int nFour = 0;
    for (int i = 0; i < 4; i++) {
        if (isOpenFour(state, x, y, i) == 2)
            nFour += 2;
        else if (isFour(state, x, y, i))
            nFour++;
    }

    if (nFour >= 2)
        return true;
    else
        return false;
}

bool Game::isForbiddenMove(GameState& state, int x, int y) {
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

bool Game::isADraw(GameState& state) {
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (state.board[i][j] == 0) {
                //if (isForbiddenMove(state, i, j)) continue;
                return false;
            }
        }
    }
    state.winner = 3;
    state.isGameOver = true;
    return true;
}