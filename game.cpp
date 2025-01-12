#include "game.h"
#include "UI.h"

void Game::initGame(GameState& state) {
    // ��ʼ�����̣�����λ��Ϊ�գ�0��
    state.board.clear(); // �������
    state.board.resize(15, std::vector<int>(15, 0)); // 15x15 �����̣���ʼֵΪ0

    // ���õ�ǰ���Ϊ���ӣ�1��
    state.currentPlayer = 1;

    // ��ʼ����Ϸ������־Ϊ false
    state.isGameOver = false;

    //��Ϸδ����
    state.winner = 0;

    // ��ջ�����ʷ��¼
    while (!history.empty()) {
        history.pop();
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
            state.winner = player; // ����ʤ����
            state.isGameOver = true; // ������Ϸ������־
            return true;
        }
    }
    return false;
}

bool Game::isValidMove(const GameState& state, int x, int y) {
    // ��������Ƿ������̷�Χ��
    if (x < 0 || x >= 15 || y < 0 || y >= 15) {
        return false; // �������̷�Χ
    }

    // ����λ���Ƿ�Ϊ��
    if (state.board[x][y] != 0) {
        return false; // ��λ����������
    }

    return true; // ���ӺϷ�
}

void Game::undoMove(GameState& state) {
    // �����ʷ��¼�Ƿ�����������״̬
    if (history.size() < 2) {
        return; // ��ʷ��¼���㣬�޷�������
    }

    // �ָ�������һ����Ϸ״̬������ҵ���һ���غϣ�
    history.pop(); // �Ƴ�AI������״̬
    state = history.top(); // �ָ�����ҵ���һ��״̬

    // �Ƴ���ҵ���һ��״̬
    history.pop();
}

void Game::makeMove(GameState& state, int x, int y) {
    // ��������Ƿ�Ϸ�
    if (!isValidMove(state, x, y)) {
        return; // ���Ӳ��Ϸ���ֱ�ӷ���
    }

    // ���浱ǰ��Ϸ״̬����ʷ��¼��
    history.push(state);

    // ��������״̬
    state.board[x][y] = state.currentPlayer;

    // ����Ƿ�ʤ��
    if (checkWin(state, x, y)) {
        state.isGameOver = true; // ��Ϸ����
        return;
    }

    // �л���ǰ���
    state.currentPlayer = (state.currentPlayer == 1) ? 2 : 1;
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

    // ���ѡ��һ���Ϸ�������λ��
    int index = rand() % validMoves.size(); // �������
    int x = validMoves[index].first;
    int y = validMoves[index].second;

    // ִ������
    makeMove(state, x, y);
}