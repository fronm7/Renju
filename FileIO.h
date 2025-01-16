#ifndef FILEIO_H
#define FILEIO_H

#include <string>
#include <fstream>
#include <stack>
#include "game.h" // ������Ϸ״̬�Ķ���

class FileIO {
public:
    // ������Ϸ״̬���ļ�
    bool saveGame(const Game& game, const GameState& state, const std::string& filename = "save.dat") {
        std::ofstream outFile(filename, std::ios::binary);
        if (!outFile) {
            return false; // �ļ���ʧ��
        }

        //// ��ȡ��ǰ��Ϸ״̬
        //const GameState& state = game.getCurrentState();

        // ��������״̬
        for (const auto& row : state.board) {
            outFile.write(reinterpret_cast<const char*>(row.data()), row.size() * sizeof(int));
        }

        // ���浱ǰ��Һ���Ϸ״̬
        outFile.write(reinterpret_cast<const char*>(&state.currentPlayer), sizeof(int));
        outFile.write(reinterpret_cast<const char*>(&state.isGameOver), sizeof(bool));
        outFile.write(reinterpret_cast<const char*>(&state.init_choice), sizeof(int));
        outFile.write(reinterpret_cast<const char*>(&state.winner), sizeof(int));
        outFile.write(reinterpret_cast<const char*>(&state.lastMoveX), sizeof(int));
        outFile.write(reinterpret_cast<const char*>(&state.lastMoveY), sizeof(int));

        // ������ʷ��¼�Ĵ�С
        size_t historySize = game.getHistorySize();
        outFile.write(reinterpret_cast<const char*>(&historySize), sizeof(size_t));

        // ������ʷ��¼�е�ÿ��״̬
        std::stack<GameState> history = game.getHistory();
        while (!history.empty()) {
            const GameState& historyState = history.top();
            for (const auto& row : historyState.board) {
                outFile.write(reinterpret_cast<const char*>(row.data()), row.size() * sizeof(int));
            }
            outFile.write(reinterpret_cast<const char*>(&historyState.currentPlayer), sizeof(int));
            outFile.write(reinterpret_cast<const char*>(&historyState.isGameOver), sizeof(bool));
            outFile.write(reinterpret_cast<const char*>(&historyState.init_choice), sizeof(int));
            outFile.write(reinterpret_cast<const char*>(&historyState.winner), sizeof(int));
            outFile.write(reinterpret_cast<const char*>(&historyState.lastMoveX), sizeof(int));
            outFile.write(reinterpret_cast<const char*>(&historyState.lastMoveY), sizeof(int));
            history.pop();
        }

        outFile.close();
        return true;
    }

    // ���ļ���ȡ��Ϸ״̬
    bool loadGame(Game& game, GameState& state, const std::string& filename = "save.dat") {
        std::ifstream inFile(filename, std::ios::binary);
        if (!inFile) {
            // ����ļ������ڣ�����һ��Ĭ�ϴ浵
            GameState state = createDefaultGameState();
            game.initGame(state);
            return saveGame(game, state, filename); // ����Ĭ�ϴ浵
        }

        //GameState state;
        // ��ȡ����״̬
        for (auto& row : state.board) {
            inFile.read(reinterpret_cast<char*>(row.data()), row.size() * sizeof(int));
        }

        // ��ȡ��ǰ��Һ���Ϸ״̬
        inFile.read(reinterpret_cast<char*>(&state.currentPlayer), sizeof(int));
        inFile.read(reinterpret_cast<char*>(&state.isGameOver), sizeof(bool));
        inFile.read(reinterpret_cast<char*>(&state.init_choice), sizeof(int));
        inFile.read(reinterpret_cast<char*>(&state.winner), sizeof(int));
        inFile.read(reinterpret_cast<char*>(&state.lastMoveX), sizeof(int));
        inFile.read(reinterpret_cast<char*>(&state.lastMoveY), sizeof(int));

        // ��ȡ��ʷ��¼�Ĵ�С
        size_t historySize;
        inFile.read(reinterpret_cast<char*>(&historySize), sizeof(size_t));

        // ��ȡ��ʷ��¼�е�ÿ��״̬
        std::vector<GameState> historyVector;
        for (size_t i = 0; i < historySize; ++i) {
            GameState historyState;
            for (auto& row : historyState.board) {
                inFile.read(reinterpret_cast<char*>(row.data()), row.size() * sizeof(int));
            }
            inFile.read(reinterpret_cast<char*>(&historyState.currentPlayer), sizeof(int));
            inFile.read(reinterpret_cast<char*>(&historyState.isGameOver), sizeof(bool));
            inFile.read(reinterpret_cast<char*>(&historyState.init_choice), sizeof(int));
            inFile.read(reinterpret_cast<char*>(&historyState.winner), sizeof(int));
            inFile.read(reinterpret_cast<char*>(&historyState.lastMoveX), sizeof(int));
            inFile.read(reinterpret_cast<char*>(&historyState.lastMoveY), sizeof(int));
            historyVector.push_back(historyState);
        }

        // �� vector ��ת���ָ�ջ��˳��
        std::reverse(historyVector.begin(), historyVector.end());

        // ����ʷ��¼ѹ��ջ��
        std::stack<GameState> history;
        for (const auto& historyState : historyVector) {
            history.push(historyState);
        }

        // ����ȡ��״̬����ʷ��¼���õ���Ϸ��
        game.setHistory(history);
        game.initGame(state);

        inFile.close();
        return true;
    }

private:
    // ����Ĭ����Ϸ״̬
    GameState createDefaultGameState() {
        GameState state;
        state.board = std::vector<std::vector<int>>(BOARD_SIZE, std::vector<int>(BOARD_SIZE, 0)); // 15x15 �Ŀ�����
        state.currentPlayer = 1; // ���ִ��
        state.isGameOver = false; // ��Ϸδ����
        state.init_choice = 1; //���ִ��
        state.winner = 0; //û��ʤ����
        state.lastMoveX = -1; // û����������
        state.lastMoveY = -1;
        return state;
    }
};

#endif // FILEIO_H