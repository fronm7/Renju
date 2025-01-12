#include "UI.h"
#include "game.h"
#include "FileIO.h"

int main() {
    GameState state;
    Game game;
    UI ui;
    FileIO fileIO;

    // 初始化图形化界面
    ui.initUI();

    // 主界面
    while (true) {
        int choice = ui.showMainMenu();
        if (choice == 1) { // 开始新游戏
            game.initGame(state);
            break;
        }
        else if (choice == 2) { // 加载存档
            if (fileIO.loadGame(state)) {
                break;
            }
            else {
                outtextxy(200, 350, _T("加载存档失败！"));
                Sleep(1000); // 提示 1 秒
            }
        }
        else if (choice == 3) { // 退出游戏
            ui.closeUI();
            return 0;
        }
    }

    // 游戏主循环
    int lastX = -1, lastY = -1; // 记录最后一次落子的位置
    while (!state.isGameOver) {
        ui.drawBoard(state);
        if (ui.handleInput(state, lastX, lastY)) { // 处理玩家输入，并更新 lastX 和 lastY
            // 玩家落子
            game.makeMove(state, lastX, lastY);
            if (state.isGameOver == true) {
                ui.showResult(state); // 显示游戏结果
                break;
            }

            // AI 落子
            if (state.currentPlayer == 2) {
                game.makeAIMove(state);
                if (state.isGameOver == true) {
                    ui.showResult(state); // 显示游戏结果
                    break;
                }

                // 切换回玩家
                state.currentPlayer = 1;
            }
        }

        // 检查是否打开菜单
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) { // 按下 ESC 键
            int menuChoice = ui.showInGameMenu();
            if (menuChoice == 1) continue; // 继续游戏
            if (menuChoice == 2) { // 保存游戏
                if (fileIO.saveGame(state)) {
                    outtextxy(200, 350, _T("游戏已保存！"));
                    Sleep(1000); // 提示 1 秒
                }
                else {
                    outtextxy(200, 350, _T("保存失败！"));
                    Sleep(1000); // 提示 1 秒
                }
            }
            if (menuChoice == 3) { // 退出游戏
                ui.closeUI();
                return 0;
            }
        }
    }

    // 游戏结束后的交互
    while (true) {
        int choice = ui.showGameOverMenu();
        if (choice == 1) { // 回到主界面
            main(); // 重新启动主程序
            return 0;
        }
        else if (choice == 2) { // 退出游戏
            ui.closeUI();
            return 0;
        }
    }
}