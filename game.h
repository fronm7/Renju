#ifndef GAME_H
#define GAME_H

#include <vector>
#include <stack>
#include <stdexcept>
// ���̴�С
const int BOARD_SIZE = 15;
// ��Ϸ״̬��
class GameState {
public:
    std::vector<std::vector<int>> board; // ����״̬��0: ��, 1: ����, 2: ����, 3: ���֣�
    int currentPlayer; // ��ǰ��ң�1: �ڷ�, 2: �׷���
    bool isGameOver; // ��Ϸ�Ƿ����
    int winner; // ʤ���ߣ�0: δ����, 1: �ڷ�ʤ��, 2: �׷�ʤ����
    int init_choice; //��ҳ�ʼѡ��

    // Ĭ�Ϲ��캯��
    GameState() : currentPlayer(1), isGameOver(false), winner(0), init_choice(1){
        board.resize(BOARD_SIZE, std::vector<int>(BOARD_SIZE, 0)); // ��ʼ�� 15x15 ������
    }

    // �������캯��
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
    // �ж������Ƿ���������
    bool isInBoard(int x, int y) {
        return x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE;
    }
    std::stack<GameState> history; // ������ʷ��¼
public:
    // ��ȡ��ǰ��Ϸ״̬
    const GameState& getCurrentState() const {
        if (history.empty()) {
            throw std::runtime_error("History is empty, no current state available.");
        }
        return history.top();
    }

    // ��ȡ��ʷ��¼�Ĵ�С
    size_t getHistorySize() const {
        return history.size();
    }

    // ��ȡ��ʷ��¼������һ��ջ�ĸ�����
    std::stack<GameState> getHistory() const {
        return history;
    }

    // ������ʷ��¼
    void setHistory(const std::stack<GameState>& newHistory) {
        history = newHistory;
    }

    //����غ���
    int turnnumber() {
        if (history.empty()) return 0;
        else return static_cast<int>(history.size());
    }

    // ��ʼ����Ϸ
    void initGame(GameState& state);

    // �ж������Ƿ�Ϸ�
    bool isValidMove(const GameState& state, int x, int y);

    // �ж��Ƿ�ʤ��
    bool checkWin(GameState& state, int x, int y);

    // ��ָ��λ�÷�������
    void setStone(GameState& state, int x, int y, int cStone);

    // �ж��Ƿ���֣����Ժڷ���
    bool isForbiddenMove(const GameState& state, int x, int y);

    // �ж��Ƿ��γ�����������
    bool isFive(const GameState& state, int x, int y);

    // �ж��Ƿ��γ��˳������������ӣ�
    bool isOverline(const GameState& state, int x, int y);

    // ��ָ�������ж��Ƿ��γ�����������
    bool isFour(const GameState& state, int x, int y, int nDir);

    // ��ָ�������ж��Ƿ��л��ģ���������������Ϊ��λ��
    int isOpenFour(const GameState& state, int x, int y, int nDir);

    // ��ָ�������ж��Ƿ��л�������������������������һ����λ��
    bool isOpenThree(const GameState& state, int x, int y, int nDir);

    // �ж��Ƿ��γ���˫�ģ������������߹���һ���˵㣩
    bool isDoubleFour(const GameState& state, int x, int y);

    // �ж��Ƿ��γ���˫���������������߹���һ���˵㣩
    bool isDoubleThree(const GameState& state, int x, int y);

    // ִ������
    void makeMove(GameState& state, int x, int y);

    // ����
    bool undoMove(GameState& state); 

    // ��������
    int evaluate(GameState& state, int x, int y);

    // �� AI ����
    void makeAIMove(GameState& state);
};

#endif // GAME_H