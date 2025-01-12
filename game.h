#ifndef GAME_H
#define GAME_H

#include <vector>
#include <stack>

// ��Ϸ״̬��
class GameState {
public:
    std::vector<std::vector<int>> board; // ����״̬��0: ��, 1: ����, 2: ���ӣ�
    int currentPlayer; // ��ǰ��ң�1: �ڷ�, 2: �׷���
    bool isGameOver; // ��Ϸ�Ƿ����
    int winner; // ʤ���ߣ�0: δ����, 1: �ڷ�ʤ��, 2: �׷�ʤ����

    // Ĭ�Ϲ��캯��
    GameState() : currentPlayer(1), isGameOver(false), winner(0) {
        board.resize(15, std::vector<int>(15, 0)); // ��ʼ�� 15x15 ������
    }

    // �������캯��
    GameState(const GameState& other)
        : board(other.board),
        currentPlayer(other.currentPlayer),
        isGameOver(other.isGameOver),
        winner(other.winner) {
        // history ����Ҫ�������Ϊ undoMove �����¹�����ʷ��¼
    }
};

class Game {
private:
    std::stack<GameState> history; // ������ʷ��¼
public:
    // ��ʼ����Ϸ
    void initGame(GameState& state);

    // �ж������Ƿ�Ϸ�
    bool isValidMove(const GameState& state, int x, int y);

    // �ж��Ƿ�ʤ��
    bool checkWin(GameState& state, int x, int y);

    //// �ж��Ƿ���֣����Ժڷ���
    //bool isForbiddenMove(const GameState& state, int x, int y);

    // ִ������
    void makeMove(GameState& state, int x, int y);

    // ����
    void undoMove(GameState& state);

    // �� AI ����
    void makeAIMove(GameState& state);
};

#endif // GAME_H