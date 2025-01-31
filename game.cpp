#include "game.h"
#include "UI.h"
#include <iostream>
void Game::initGame(GameState& state) {
    if (history.empty()) {
        // ��ʼ�����̣�����λ��Ϊ�գ�0��
        state.board.clear(); // �������
        state.board.resize(BOARD_SIZE, std::vector<int>(BOARD_SIZE, 0)); // 15x15 �����̣���ʼֵΪ0

        // ���õ�ǰ���Ϊ���ӣ�1��
        state.currentPlayer = 1;

        // ��ʼ����Ϸ������־Ϊ false
        state.isGameOver = false;

        //��Ϸδ����
        state.winner = 0;

        //����λ�û���Ϊ�Ƿ�
        state.lastMoveX = -1;
        state.lastMoveY = -1;
    }
    else {
        state = getCurrentState();
    }
}

bool Game::checkWin(GameState& state, int x, int y) {
    int player = state.board[x][y];
    if (player == 0) return false; // ��λ��Ϊ�գ�������ʤ��

    // ����ĸ�����
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
                state.winner = player; // ����ʤ����
                state.isGameOver = true; // ������Ϸ������־
                return true;
            }
        }
        else {
            if (count >= 5) {
                state.winner = player; // ����ʤ����
                state.isGameOver = true; // ������Ϸ������־
                return true;
            }
        }
    }
    return false;
}

bool Game::isValidMove(const GameState& state, int x, int y) {
    // ��������Ƿ������̷�Χ��
    if (x < 0 || x >= BOARD_SIZE || y < 0 || y >= BOARD_SIZE) {
        return false; // �������̷�Χ
    }

    // ����λ���Ƿ�Ϊ��
    if (state.board[x][y] != 0 && state.board[x][y] != 3) {
        return false; // ��λ����������
    }

    return true; // ���ӺϷ�
}

bool Game::undoMove(GameState& state) {
    // �����ʷ��¼�Ƿ�����������״̬
    if (history.size() < 2) {
        return false; // ��ʷ��¼���㣬�޷�������
    }

    // �ָ�������һ����Ϸ״̬������ҵ���һ���غϣ�
    history.pop();// �Ƴ�AI������״̬
    history.pop(); // �Ƴ���ҵ���һ������״̬

    if (history.empty()) {
        initGame(state);
    }
    else {
        state = history.top(); 
        state.currentPlayer = state.init_choice; // �ָ�����ҵ���һ��״̬
    }
    
    return true;
}

void Game::makeMove(GameState& state, int x, int y) {
    // ��������Ƿ�Ϸ�
    if (!isValidMove(state, x, y)) {
        return; // ���Ӳ��Ϸ���ֱ�ӷ���
    }

    if (isADraw(state)) return;

    // �жϽ���
    bool is_forbidden = isForbiddenMove(state, x, y);

    // ��������״̬
    state.board[x][y] = state.currentPlayer;

    // �������һ�����ӵ�λ��
    state.lastMoveX = x;
    state.lastMoveY = y;

    // �л���ǰ���
    state.currentPlayer = (state.currentPlayer == 1) ? 2 : 1;

    // ���浱ǰ��Ϸ״̬����ʷ��¼��
    history.push(state);

    //std::cout << "Ŀǰ��" << state.currentPlayer << "����" << " " << "history��СΪ" << history.size() << std::endl;
    
    // ����Ƿ�ʤ��
    if (checkWin(state, x, y)) {
        return;
    }

    if (is_forbidden) { // ��λ��Ϊ����
        //std::cout << "Ŀǰ��" << state.currentPlayer << "����,�ǽ���" << std::endl;
        state.isGameOver = true; // ��Ϸ����
        state.winner = 2; // �׷���ʤ
        return;
    }
    /*std::cout << "Ŀǰ��" << state.currentPlayer << "����,���ǽ���" << std::endl;*/

    return;
}

int Game::evaluate(GameState& state, int x, int y) {
    int totalScore = 0;
    // ����ĸ�����
    int dx[] = { 1, 0, 1, 1 };
    int dy[] = { 0, 1, 1, -1 };
    for (int dir = 0; dir < 4; ++dir) {
        // �� (x, y) Ϊ���ģ�������������չ���ҵ����а��� (x, y) ����Ԫ��
        for (int offset = -4; offset <= 0; ++offset) {
            int startX = x + offset * dx[dir];
            int startY = y + offset * dy[dir];
            int blackCount = 0, whiteCount = 0;// ͳ����Ԫ���еĺ���Ͱ�������
            // �����Ԫ���Ƿ���������
            if (isInBoard(startX, startY) && isInBoard(startX + 4 * dx[dir], startY + 4 * dy[dir])) {  
                for (int i = 0; i < 5; ++i) {
                    int currentX = startX + i * dx[dir];
                    int currentY = startY + i * dy[dir];
                    if (state.board[currentX][currentY] == 1) blackCount++;
                    else if (state.board[currentX][currentY] == 2) whiteCount++;
                }
            }
            if (state.init_choice == 1) {// ��������
                blackCount += whiteCount;
                whiteCount = blackCount - whiteCount;
                blackCount -= whiteCount;
            }
            // ���ݵ�����������
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
    // �����Ϸ�Ƿ��Ѿ�����
    if (state.isGameOver) {
        return; // ��Ϸ�ѽ�����AI ������
    }
    // �������кϷ�������λ��
    std::vector<std::pair<int, int>> validMoves;
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            if (state.board[i][j] == 0) { // ��λ
                validMoves.push_back({ i, j });
            }
        }
    }

    // ���û�кϷ�������λ�ã�ֱ�ӷ���
    if (validMoves.empty()) {
        return;
    }

    int x = validMoves[0].first, y = validMoves[0].second, present_score = 0;

    if (/*validMoves.size() == BOARD_SIZE * BOARD_SIZE - 1*/false) {
         //���ѡ��һ���Ϸ�������λ��
        int index = rand() % validMoves.size(); // �������
        x = validMoves[index].first;
        y = validMoves[index].second;
    }
    else {
        for (auto& point : validMoves) {
            if (evaluate(state, point.first, point.second) >= present_score) {
                if (state.init_choice == 1) {
                    // ��ʱ�л���ǰ���
                    state.currentPlayer = (state.currentPlayer == 1) ? 2 : 1;
                    if (isForbiddenMove(state, point.first, point.second)) {
                        state.currentPlayer = (state.currentPlayer == 1) ? 2 : 1; // �л�����
                        continue;
                    }
                    state.currentPlayer = (state.currentPlayer == 1) ? 2 : 1; // �л�����
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

    ////���ѡ��һ���Ϸ�������λ��
    //int index = rand() % validMoves.size(); // �������
    //x = validMoves[index].first;
    //y = validMoves[index].second;
    // ִ������
    makeMove(state, x, y);
}

void Game::setStone(GameState& state, int x, int y, int cStone) {
    state.board[x][y] = cStone; 
    return;
}

bool Game::isFive(GameState& state, int x, int y) {
    int nColor = state.currentPlayer; // ��ǰ���

    // �����λ���������ӣ���ֱ�ӷ���false
    if (state.board[x][y] != 0)
        return false;

    // ��������
    int dx[] = { 1, 0, 1, 1 };
    int dy[] = { 0, 1, 1, -1 };

    // ��ʱ�������ӽ��м��
    setStone(state, x, y, nColor);

    // ����ĸ������ϵ���������
    for (int dir = 0; dir < 4; dir++) {
        int nLine = 1;

        // ��һ��������չ
        int i = x + dx[dir], j = y + dy[dir];
        while (isInBoard(i, j) && state.board[i][j] == nColor) {
            nLine++;
            i += dx[dir];
            j += dy[dir];
        }

        // ���෴������չ
        i = x - dx[dir];
        j = y - dy[dir];
        while (isInBoard(i, j) && state.board[i][j] == nColor) {
            nLine++;
            i -= dx[dir];
            j -= dy[dir];
        }

        // ������Ҫ�����������飬������Ҫ������������
        if ((nColor == 1 && nLine == 5) || (nColor == 2 && nLine >= 5)) {
            state.board[x][y] = 0; // �ָ�Ϊ��λ
            return true;
        }
    }

    setStone(state, x, y, 0); // �ָ�Ϊ��λ
    return false;
}

bool Game::isFive(GameState& state, int x, int y, int nDir) {
    int nColor = state.currentPlayer; // ��ǰ���

    // �����λ���������ӣ���ֱ�ӷ���false
    if (state.board[x][y] != 0)
        return false;

    // ��������
    int dx[] = { 1, 0, 1, 1 };
    int dy[] = { 0, 1, 1, -1 };

    // ��ʱ�������ӽ��м��
    setStone(state, x, y, nColor);

    // ���nDir�����ϵ���������
    int nLine = 1, dir = nDir;

    // ��һ��������չ
    int i = x + dx[dir], j = y + dy[dir];
    while (isInBoard(i, j) && state.board[i][j] == nColor) {
        nLine++;
        i += dx[dir];
        j += dy[dir];
    }

    // ���෴������չ
    i = x - dx[dir];
    j = y - dy[dir];
    while (isInBoard(i, j) && state.board[i][j] == nColor) {
        nLine++;
        i -= dx[dir];
        j -= dy[dir];
    }

    // ������Ҫ�����������飬������Ҫ������������
    if ((nColor == 1 && nLine == 5) || (nColor == 2 && nLine >= 5)) {
        state.board[x][y] = 0; // �ָ�Ϊ��λ
        return true;
    }

    setStone(state, x, y, 0); // �ָ�Ϊ��λ
    return false;
}

bool Game::isOverline(GameState& state, int x, int y) {
    // �����λ���������ӣ���ֱ�ӷ���false
    if (state.board[x][y] != 0)
        return false;

    // ��������
    int dx[] = { 1, 0, 1, 1 };
    int dy[] = { 0, 1, 1, -1 };

    // ��ʱ���ú��ӽ��м��
    setStone(state, x, y, 1); // ����

    bool bOverline = false;

    // ����ĸ������ϵĳ���
    for (int dir = 0; dir < 4; dir++) {
        int nLine = 1;

        // ��һ��������չ
        int i = x + dx[dir], j = y + dy[dir];
        while (isInBoard(i, j) && state.board[i][j] == 1) {
            nLine++;
            i += dx[dir];
            j += dy[dir];
        }

        // ���෴������չ
        i = x - dx[dir];
        j = y - dy[dir];
        while (isInBoard(i, j) && state.board[i][j] == 1) {
            nLine++;
            i -= dx[dir];
            j -= dy[dir];
        }

        // ����γ��������飬�������ò�����false����Ϊ���ǳ�����
        if (nLine == 5) {
            setStone(state, x, y, 0); // �ָ�Ϊ��λ
            return false;
        }

        // ����γɳ�����������bOverlineΪtrue
        if (nLine >= 6) {
            bOverline = true;
        }
    }

    setStone(state, x, y, 0); // �ָ�Ϊ��λ
    return bOverline;
}

bool Game::isFour(GameState& state, int x, int y, int nDir) {
    // �����λ���������ӣ���ֱ�ӷ���false
    if (state.board[x][y] != 0)
        return false;

    // ����������Ӻ���γ��������飬����false
    if (isFive(state, x, y, 1))
        return false;
    // �����ǰ����Ǻڷ��ҷ������Ӻ���γɺڷ��ĳ���������false
    else if ((state.currentPlayer == 1) && (isOverline(state, x, y)))
        return false;
    else {
        int c = state.currentPlayer; // ��ȡ��ǰ���

        // ��ʱ��(x, y)λ�÷�������
        setStone(state, x, y, c);

        // ��������
        int dx[] = { 1, 0, 1, 1 };
        int dy[] = { 0, 1, 1, -1 };

        // ����nDir��ֵ�жϼ��ķ���
        int dir = nDir ; 
        int nLine = 1;

        // ��һ��������չ
        int i = x + dx[dir], j = y + dy[dir];
        while (isInBoard(i, j) && state.board[i][j] == c) {
            nLine++;
            i += dx[dir];
            j += dy[dir];
        }

        // ���෴������չ
        i = x - dx[dir];
        j = y - dy[dir];
        while (isInBoard(i, j) && state.board[i][j] == c) {
            nLine++;
            i -= dx[dir];
            j -= dy[dir];
        }

        // ����γ��������飬�������ò�����true
        if (nLine == 4) {
            setStone(state, x, y, 0); // �ָ�Ϊ��λ
            return true;
        }

        setStone(state, x, y, 0); // �ָ�Ϊ��λ
        return false;
    }
}

int Game::isOpenFour(GameState& state, int x, int y, int nDir) {
    int nColor = state.currentPlayer; // ��ȡ��ǰ���

    // �����λ���������ӣ���ֱ�ӷ���0
    if (state.board[x][y] != 0)
        return 0;

    // ����������Ӻ���γ��������飬����0
    if (isFive(state, x, y, nColor))
        return 0;

    // �����ǰ����Ǻڷ��ҷ������Ӻ���γɺڷ��ĳ���������0
    else if ((nColor == 1) && (isOverline(state, x, y)))
        return 0;
    else {
        // ��ʱ��(x, y)λ�÷�������
        setStone(state, x, y, nColor);

        // ��������
        int dx[] = { 1, 0, 1, 1 };
        int dy[] = { 0, 1, 1, -1 };

        // ����nDir��ֵ�жϼ��ķ���
        int dir = nDir;
        int nLine = 1;

        // ��һ��������չ
        int i = x + dx[dir], j = y + dy[dir];
        while (isInBoard(i, j) && state.board[i][j] == nColor) {
            nLine++;
            i += dx[dir];
            j += dy[dir];
        }
        if (isInBoard(i, j) && state.board[i][j] == 0) {
            // ������ܳ��壬���ǻ���
            if (!isFive(state, i , j , dir)) {
                setStone(state, x, y, 0);
                return 0;
            }
        }
        else {
            setStone(state, x, y, 0);
            return 0;
        }

        // ���෴������չ
        i = x - dx[dir];
        j = y - dy[dir];
        while (isInBoard(i, j) && state.board[i][j] == nColor) {
            nLine++;
            i -= dx[dir];
            j -= dy[dir];
        }
        if (isInBoard(i, j) && state.board[i][j] == 0) {
            // ������Գ����ҵ�ǰ����Ϊ4����Ϊ���ģ�����1�����򷵻�2
            if (isFive(state, i , j , dir)) {
                setStone(state, x, y, 0);
                return (nLine == 4 ? 1 : 2);
            }
        }

        setStone(state, x, y, 0); // �ָ�Ϊ��λ
        return 0;
    }
}

bool Game::isOpenThree(GameState& state, int x, int y, int nDir) {
    int nColor = state.currentPlayer; // ��ȡ��ǰ���

    // �����λ���������ӣ���ֱ�ӷ���false
    if (state.board[x][y] != 0)
        return false;

    // ����������Ӻ���γ��������飬����false
    if (isFive(state, x, y, nColor))
        return false;

    // �����ǰ����Ǻڷ��ҷ������Ӻ���γɺڷ��ĳ���������false
    else if ((nColor == 1) && (isOverline(state, x, y)))
        return false;
    else {
        // ��ʱ��(x, y)λ�÷�������
        setStone(state, x, y, nColor);

        // ��������
        int dx[] = {1, 0, 1, 1};
        int dy[] = {0, 1, 1, -1};

        // ����nDir��ֵ�жϼ��ķ���
        int dir = nDir; 

        // ��һ��������չ
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

        // ���෴������չ
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

        setStone(state, x, y, 0); // �ָ�Ϊ��λ
        return false;
    }
}

bool Game::isDoubleThree(GameState& state, int x, int y) {
    int nColor = state.currentPlayer; // ��ȡ��ǰ���

    // �����λ���������ӣ���ֱ�ӷ���false
    if (state.board[x][y] != 0)
        return false;

    // ����������Ӻ����壬����false
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
    int nColor = state.currentPlayer; // ��ȡ��ǰ���

    // �����λ���������ӣ���ֱ�ӷ���false
    if (state.board[x][y] != 0)
        return false;

    // ����������Ӻ���γ��������飬����false
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
    // ���Ժڷ����н��ּ��
    if (state.currentPlayer != 1) {
        return false; 
    }

    // ����Ƿ��γ�������
    if (isFive(state, x, y)) {
        return false; // �γ����������ǽ���
    }

    // ����Ƿ��γ��˳�������
    if (isOverline(state, x, y)) {
        return true; // �γɳ�������
    }

    // ����Ƿ��γ���˫������
    if (isDoubleThree(state, x, y)) {
        return true; // �γ�˫������
    }

    // ����Ƿ��γ���˫�Ľ���
    if (isDoubleFour(state, x, y)) {
        return true; // �γ�˫�Ľ���
    }

    // ���û���γ��κν��֣��򷵻� false
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