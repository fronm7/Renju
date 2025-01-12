#ifndef FILEIO_H
#define FILEIO_H

#include <string>
#include <fstream>
#include "game.h" // 包含游戏状态的定义

class FileIO {
public:
    // 保存游戏状态到文件
    bool saveGame(const GameState& state, const std::string& filename = "save.dat") {
        std::ofstream outFile(filename, std::ios::binary);
        if (!outFile) {
            return false; // 文件打开失败
        }

        // 保存棋盘状态
        for (const auto& row : state.board) {
            outFile.write(reinterpret_cast<const char*>(row.data()), row.size() * sizeof(int));
        }

        // 保存当前玩家和游戏状态
        outFile.write(reinterpret_cast<const char*>(&state.currentPlayer), sizeof(int));
        outFile.write(reinterpret_cast<const char*>(&state.isGameOver), sizeof(bool));

        outFile.close();
        return true;
    }

    // 从文件读取游戏状态
    bool loadGame(GameState& state, const std::string& filename = "save.dat") {
        std::ifstream inFile(filename, std::ios::binary);
        if (!inFile) {
            // 如果文件不存在，创建一个默认存档
            state = createDefaultGameState();
            return saveGame(state, filename); // 保存默认存档
        }

        // 读取棋盘状态
        for (auto& row : state.board) {
            inFile.read(reinterpret_cast<char*>(row.data()), row.size() * sizeof(int));
        }

        // 读取当前玩家和游戏状态
        inFile.read(reinterpret_cast<char*>(&state.currentPlayer), sizeof(int));
        inFile.read(reinterpret_cast<char*>(&state.isGameOver), sizeof(bool));

        inFile.close();
        return true;
    }

private:
    // 创建默认游戏状态
    GameState createDefaultGameState() {
        GameState state;
        state.board = std::vector<std::vector<int>>(15, std::vector<int>(15, 0)); // 15x15 的空棋盘
        state.currentPlayer = 1; // 玩家执黑
        state.isGameOver = false; // 游戏未结束
        int winner = 0;
        return state;
    }
};

#endif // FILEIO_H