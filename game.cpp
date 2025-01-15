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

    // ��������״̬
    state.board[x][y] = state.currentPlayer;

    // �л���ǰ���
    state.currentPlayer = (state.currentPlayer == 1) ? 2 : 1;

    // ���浱ǰ��Ϸ״̬����ʷ��¼��
    history.push(state);

    //std::cout << "Ŀǰ��" << state.currentPlayer << "����" << " " << "history��СΪ" << history.size() << std::endl;
    
    // ����Ƿ�ʤ��
    if (checkWin(state, x, y)) {
        return;
    }

    // ��ʱ�л���ǰ���
    state.currentPlayer = (state.currentPlayer == 1) ? 2 : 1;

    if (state.currentPlayer == 1 && isForbiddenMove(state, x, y)) { // ��λ��Ϊ����
        std::cout << "Ŀǰ��" << state.currentPlayer << "����,�ǽ���" << std::endl;
        state.isGameOver = true; // ��Ϸ����
        state.winner = 2; // �׷���ʤ
        return;
    }
    std::cout << "Ŀǰ��" << state.currentPlayer << "����,���ǽ���" << std::endl;

    // �����л���ǰ���
    state.currentPlayer = (state.currentPlayer == 1) ? 2 : 1;

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

    //if (validMoves.size() == BOARD_SIZE * BOARD_SIZE - 1) {
    //     //���ѡ��һ���Ϸ�������λ��
    //    int index = rand() % validMoves.size(); // �������
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

    //���ѡ��һ���Ϸ�������λ��
    int index = rand() % validMoves.size(); // �������
    x = validMoves[index].first;
    y = validMoves[index].second;

    // ִ������
    makeMove(state, x, y);
}

void Game::setStone(GameState& state, int x, int y, int cStone) {
    state.board[x][y] = cStone; 
    return;
}

bool Game::isFive(const GameState& state, int x, int y) {
    int player = state.currentPlayer; // ��ǰ���

    // �������飺ˮƽ����ֱ�������Խ���
    int dx[] = { 1, 0, 1, 1 };
    int dy[] = { 0, 1, 1, -1 };

    for (int dir = 0; dir < 4; dir++) {
        int count = 1; // ��ǰ���������������
        // ����������
        int nx = x + dx[dir], ny = y + dy[dir];
        while (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE && state.board[nx][ny] == player) {
            count++;
            nx += dx[dir];
            ny += dy[dir];
        }
        // �򷴷�����
        nx = x - dx[dir], ny = y - dy[dir];
        while (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE && state.board[nx][ny] == player) {
            count++;
            nx -= dx[dir];
            ny -= dy[dir];
        }
        // �ж��Ƿ��γ�����
        if (player == 1) { // ���ӱ���ǡ������
            if (count == 5) {
                return true;
            }
        }
        else if (player == 2) { // ���ӿ������ӻ����
            if (count >= 5) {
                return true;
            }
        }
    }
    return false;
}

bool Game::isOverline(const GameState& state, int x, int y) {
    int player = state.currentPlayer; // ��ǰ���

    if (player != 1) return false; // ֻ�к�����Ҫ��鳤��

    // �������飺ˮƽ����ֱ�������Խ���
    int dx[] = { 1, 0, 1, 1 };
    int dy[] = { 0, 1, 1, -1 };

    for (int dir = 0; dir < 4; dir++) {
        int count = 1; // ��ǰ���������������
        // ����������
        int nx = x + dx[dir], ny = y + dy[dir];
        while (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE && state.board[nx][ny] == player) {
            count++;
            nx += dx[dir];
            ny += dy[dir];
        }
        // �򷴷�����
        nx = x - dx[dir], ny = y - dy[dir];
        while (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE && state.board[nx][ny] == player) {
            count++;
            nx -= dx[dir];
            ny -= dy[dir];
        }
        // �ж��Ƿ��γɳ������������ӣ�
        if (count > 5) {
            return true;
        }
    }
    return false;
}

bool Game::isFour(const GameState& state, int x, int y, int nDir) {
    int player = state.currentPlayer; // ��ǰ���

    // �������飺ˮƽ����ֱ�������Խ���
    int dx[] = { 1, 0, 1, 1 };
    int dy[] = { 0, 1, 1, -1 };

    // ����Ƿ��γ�����
    if (isFive(state, x, y)) {
        return false;
    }

    // ���ڷ��ĳ�������
    if (player == 1 && isOverline(state, x, y)) {
        return false;
    }

    // ģ���������
    int count = 1; // ��ǰ���������������

    // ����������
    int nx = x + dx[nDir - 1], ny = y + dy[nDir - 1];
    while (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE && state.board[nx][ny] == player) {
        count++;
        nx += dx[nDir - 1];
        ny += dy[nDir - 1];
    }

    // �򷴷�����
    nx = x - dx[nDir - 1], ny = y - dy[nDir - 1];
    while (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE && state.board[nx][ny] == player) {
        count++;
        nx -= dx[nDir - 1];
        ny -= dy[nDir - 1];
    }

    // �ж��Ƿ��γ�����
    if (count == 4) {
        return true;
    }

    return false;
}

int Game::isOpenFour(const GameState& state, int x, int y, int nDir) {
    int player = state.currentPlayer; // ��ǰ���

    // ����Ƿ��Ѿ��γ�����������
    if (isFive(state, x, y)) {
        return 0; // �Ѿ��γ��������ߣ����ǻ���
    }

    // �������Ƿ��г�������
    if (player == 1 && isOverline(state, x, y)) {
        return 0; // �����г������֣����ǻ���
    }

    // �������飺ˮƽ����ֱ�������Խ���
    int dx[] = { 1, 0, 1, 1 };
    int dy[] = { 0, 1, 1, -1 };

    // ����ĸ�����
    for (int dir = 0; dir < 4; ++dir) {
        int count = 1; // ��ǰ���������������
        int openEnds = 0; // ���Ŷ˵�����

        // ����������
        int nx = x + dx[dir], ny = y + dy[dir];
        while (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE && state.board[nx][ny] == player) {
            count++;
            nx += dx[dir];
            ny += dy[dir];
        }
        // ����������Ƿ񿪷�
        if (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE && state.board[nx][ny] == 0) {
            openEnds++;
        }

        // �򷴷�����
        nx = x - dx[dir], ny = y - dy[dir];
        while (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE && state.board[nx][ny] == player) {
            count++;
            nx -= dx[dir];
            ny -= dy[dir];
        }
        // ��鷴�����Ƿ񿪷�
        if (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE && state.board[nx][ny] == 0) {
            openEnds++;
        }

        // �ж��Ƿ��γɻ���
        if (count == 4 && openEnds >= 1) {
            return 1; // �γɻ���
        }
    }

    return 0; // û���γɻ���
}

bool Game::isOpenThree(const GameState& state, int x, int y, int nDir) {
    int player = state.currentPlayer; // ��ǰ���

    // ����Ƿ��Ѿ��γ�����������
    if (isFive(state, x, y)) {
        return false; // �Ѿ��γ��������ߣ����ǻ���
    }

    // �������Ƿ��г�������
    if (player == 1 && isOverline(state, x, y)) {
        return false; // �����г������֣����ǻ���
    }

    // �������飺ˮƽ����ֱ�������Խ���
    int dx[] = { 1, 0, 1, 1 };
    int dy[] = { 0, 1, 1, -1 };

    // ����ĸ�����
    for (int dir = 0; dir < 4; ++dir) {
        int count = 1; // ��ǰ���������������
        int openEnds = 0; // ���Ŷ˵�����

        // ����������
        int nx = x + dx[dir], ny = y + dy[dir];
        while (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE && state.board[nx][ny] == player) {
            count++;
            nx += dx[dir];
            ny += dy[dir];
        }

        // ����������Ƿ񿪷�
        if (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE && state.board[nx][ny] == 0) {
            // ����λ�Ƿ�Ϊ����
            if (!isForbiddenMove(state, nx, ny)) {
                // ����Ƿ����㿪��������û��˫�ĺ�˫���赲
                if ((isOpenFour(state, nx, ny, dir) == 1) &&
                    (!isDoubleFour(state, nx, ny)) &&
                    (!isDoubleThree(state, nx, ny))) {
                    openEnds++; // ��λ��Ч�����Ŷ˼�������
                }
            }
        }

        // �򷴷�����
        nx = x - dx[dir], ny = y - dy[dir];
        while (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE && state.board[nx][ny] == player) {
            count++;
            nx -= dx[dir];
            ny -= dy[dir];
        }
        // ��鷴�����Ƿ񿪷�
        if (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE && state.board[nx][ny] == 0) {
            // ����λ�Ƿ�Ϊ����
            if (!isForbiddenMove(state, nx, ny)) {
                // ����Ƿ����㿪��������û��˫�ĺ�˫���赲
                if ((isOpenFour(state, nx, ny, dir) == 1) &&
                    (!isDoubleFour(state, nx, ny)) &&
                    (!isDoubleThree(state, nx, ny))) {
                    openEnds++; // ��λ��Ч�����Ŷ˼�������
                }
            }
        }

        // �ж��Ƿ��γɻ���
        if (count == 3 && openEnds >= 1) {
            // ����Ƿ��γ�˫�Ļ�˫��
            if (!isDoubleFour(state, x, y) && !isDoubleThree(state, x, y)) {
                return true; // �γɻ���
            }
        }
    }

    return false; // û���γɻ���
}

bool Game::isDoubleThree(const GameState& state, int x, int y) {
    int player = state.currentPlayer; // ��ǰ���

    // ����Ƿ��Ѿ��γ�����������
    if (isFive(state, x, y)) {
        return false; // �Ѿ��γ��������ߣ�����˫��
    }

    //// �������Ƿ��г�������
    //if (player == 1 && isOverline(state, x, y)) {
    //    return false; // �����г������֣�����˫��
    //}

    int threeCount = 0; // ��¼����������

    // �������飺ˮƽ����ֱ�������Խ���
    int dx[] = { 1, 0, 1, 1 };
    int dy[] = { 0, 1, 1, -1 };

    // ����ĸ�����
    for (int dir = 0; dir < 4; ++dir) {
        // ��鵱ǰ�����Ƿ��γɻ���
        if (isOpenThree(state, x, y, dir)) {
            threeCount++;
        }
    }

    // ����������������ڵ���2���򷵻�true����ʾ�γ�˫��
    return threeCount >= 2;
}

bool Game::isDoubleFour(const GameState& state, int x, int y) {
    int player = state.currentPlayer; // ��ǰ���

    // ����Ƿ��Ѿ��γ�����������
    if (isFive(state, x, y)) {
        return false; // �Ѿ��γ��������ߣ�����˫��
    }

    //// �������Ƿ��г�������
    //if (player == 1 && isOverline(state, x, y)) {
    //    return false; // �����г������֣�����˫��
    //}

    int fourCount = 0; // ��¼����������

    // �������飺ˮƽ����ֱ�������Խ���
    int dx[] = { 1, 0, 1, 1 };
    int dy[] = { 0, 1, 1, -1 };

    // ����ĸ�����
    for (int dir = 0; dir < 4; ++dir) {
        // ��鵱ǰ�����Ƿ��γɻ���
        if (isOpenFour(state, x, y, dir) == 1) {
            fourCount += 2; // ����������������
        }
        // ��鵱ǰ�����Ƿ��γ���ͨ����
        else if (isFour(state, x, y, dir)) {
            fourCount++;
        }
    }

    // ����������������ڵ���2���򷵻�true����ʾ�γ�˫��
    return fourCount >= 2;
}

bool Game::isForbiddenMove(const GameState& state, int x, int y) {
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