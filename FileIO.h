#ifndef FILEIO_H
#define FILEIO_H

#include <string>
#include <fstream>
#include "game.h" // ������Ϸ״̬�Ķ���

class FileIO {
public:
    // ������Ϸ״̬���ļ�
    bool saveGame(const GameState& state, const std::string& filename = "save.dat") {
        std::ofstream outFile(filename, std::ios::binary);
        if (!outFile) {
            return false; // �ļ���ʧ��
        }

        // ��������״̬
        for (const auto& row : state.board) {
            outFile.write(reinterpret_cast<const char*>(row.data()), row.size() * sizeof(int));
        }

        // ���浱ǰ��Һ���Ϸ״̬
        outFile.write(reinterpret_cast<const char*>(&state.currentPlayer), sizeof(int));
        outFile.write(reinterpret_cast<const char*>(&state.isGameOver), sizeof(bool));

        outFile.close();
        return true;
    }

    // ���ļ���ȡ��Ϸ״̬
    bool loadGame(GameState& state, const std::string& filename = "save.dat") {
        std::ifstream inFile(filename, std::ios::binary);
        if (!inFile) {
            // ����ļ������ڣ�����һ��Ĭ�ϴ浵
            state = createDefaultGameState();
            return saveGame(state, filename); // ����Ĭ�ϴ浵
        }

        // ��ȡ����״̬
        for (auto& row : state.board) {
            inFile.read(reinterpret_cast<char*>(row.data()), row.size() * sizeof(int));
        }

        // ��ȡ��ǰ��Һ���Ϸ״̬
        inFile.read(reinterpret_cast<char*>(&state.currentPlayer), sizeof(int));
        inFile.read(reinterpret_cast<char*>(&state.isGameOver), sizeof(bool));

        inFile.close();
        return true;
    }

private:
    // ����Ĭ����Ϸ״̬
    GameState createDefaultGameState() {
        GameState state;
        state.board = std::vector<std::vector<int>>(15, std::vector<int>(15, 0)); // 15x15 �Ŀ�����
        state.currentPlayer = 1; // ���ִ��
        state.isGameOver = false; // ��Ϸδ����
        int winner = 0;
        return state;
    }
};

#endif // FILEIO_H