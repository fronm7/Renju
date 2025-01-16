#ifndef FILEIO_H
#define FILEIO_H

#include <string>
#include <fstream>
#include <stack>
#include "game.h" // 包含游戏状态的定义

class FileIO {
public:
    // 保存游戏状态到文件
    bool saveGame(const Game& game, const GameState& state, const std::string& filename = "save.dat") {
        std::ofstream outFile(filename, std::ios::binary);
        if (!outFile) {
            return false; // 文件打开失败
        }

        //// 获取当前游戏状态
        //const GameState& state = game.getCurrentState();

        // 保存棋盘状态
        for (const auto& row : state.board) {
            outFile.write(reinterpret_cast<const char*>(row.data()), row.size() * sizeof(int));
        }

        // 保存当前玩家和游戏状态
        outFile.write(reinterpret_cast<const char*>(&state.currentPlayer), sizeof(int));
        outFile.write(reinterpret_cast<const char*>(&state.isGameOver), sizeof(bool));
        outFile.write(reinterpret_cast<const char*>(&state.init_choice), sizeof(int));
        outFile.write(reinterpret_cast<const char*>(&state.winner), sizeof(int));
        outFile.write(reinterpret_cast<const char*>(&state.lastMoveX), sizeof(int));
        outFile.write(reinterpret_cast<const char*>(&state.lastMoveY), sizeof(int));

        // 保存历史记录的大小
        size_t historySize = game.getHistorySize();
        outFile.write(reinterpret_cast<const char*>(&historySize), sizeof(size_t));

        // 保存历史记录中的每个状态
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

    // 从文件读取游戏状态
    bool loadGame(Game& game, GameState& state, const std::string& filename = "save.dat") {
        std::ifstream inFile(filename, std::ios::binary);
        if (!inFile) {
            // 如果文件不存在，创建一个默认存档
            GameState state = createDefaultGameState();
            game.initGame(state);
            return saveGame(game, state, filename); // 保存默认存档
        }

        //GameState state;
        // 读取棋盘状态
        for (auto& row : state.board) {
            inFile.read(reinterpret_cast<char*>(row.data()), row.size() * sizeof(int));
        }

        // 读取当前玩家和游戏状态
        inFile.read(reinterpret_cast<char*>(&state.currentPlayer), sizeof(int));
        inFile.read(reinterpret_cast<char*>(&state.isGameOver), sizeof(bool));
        inFile.read(reinterpret_cast<char*>(&state.init_choice), sizeof(int));
        inFile.read(reinterpret_cast<char*>(&state.winner), sizeof(int));
        inFile.read(reinterpret_cast<char*>(&state.lastMoveX), sizeof(int));
        inFile.read(reinterpret_cast<char*>(&state.lastMoveY), sizeof(int));

        // 读取历史记录的大小
        size_t historySize;
        inFile.read(reinterpret_cast<char*>(&historySize), sizeof(size_t));

        // 读取历史记录中的每个状态
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

        // 将 vector 反转，恢复栈的顺序
        std::reverse(historyVector.begin(), historyVector.end());

        // 将历史记录压入栈中
        std::stack<GameState> history;
        for (const auto& historyState : historyVector) {
            history.push(historyState);
        }

        // 将读取的状态和历史记录设置到游戏中
        game.setHistory(history);
        game.initGame(state);

        inFile.close();
        return true;
    }

private:
    // 创建默认游戏状态
    GameState createDefaultGameState() {
        GameState state;
        state.board = std::vector<std::vector<int>>(BOARD_SIZE, std::vector<int>(BOARD_SIZE, 0)); // 15x15 的空棋盘
        state.currentPlayer = 1; // 玩家执黑
        state.isGameOver = false; // 游戏未结束
        state.init_choice = 1; //玩家执黑
        state.winner = 0; //没有胜利者
        state.lastMoveX = -1; // 没有最新落子
        state.lastMoveY = -1;
        return state;
    }
};

#endif // FILEIO_H