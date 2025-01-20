#include "UI.h"
#include "game.h"
#include "FileIO.h"
#pragma comment(linker, "/subsystem:windows /entry:mainCRTStartup")
//int Freq = 200;
//int T0 = 1000 / Freq;
int main() {
    GameState state;
    Game game;
    UI ui;
    FileIO fileIO;
    // 初始化图形化界面
    ui.initUI();

    // 主界面
    while (true) {
        Sleep(5);
        int choice = ui.showMainMenu();
        if (choice == 1) { // 开始新游戏
            game.initGame(state);
            //进入选子界面
            int select_choice =ui.showSelectMenu();
            if (select_choice == 3) {
                main();
                return 0;
            }
            else {
                state.init_choice = select_choice;
            }
            break;
        }
        else if (choice == 2) { // 加载存档
            if (fileIO.loadGame(game,state)) {
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
        Sleep(5);

        ui.drawBoard(state, game); //绘制棋盘、悔棋按钮,显示回合数
        
        // AI先手天元
        if (state.init_choice == 2 && game.turnnumber() == 0) {
            game.makeMove(state, BOARD_SIZE / 2, BOARD_SIZE / 2);
            ui.drawBoard(state, game); //绘制棋盘、悔棋按钮,显示回合数
        }

        if (MouseHit()) {
            MOUSEMSG msg = GetMouseMsg();

            // 处理悔棋按钮点击事件
            if (ui.handleUndoButtonClick(msg)) {
                if (state.currentPlayer != state.init_choice) {
                    outtextxy(UNDO_BUTTON_X, UNDO_BUTTON_Y-25, _T("这不是你的回合")); // 提示无法悔棋
                    Sleep(1000); // 显示提示 1 秒
                }
                else if (!game.undoMove(state)) { // 至少有两步才可以悔棋
                    outtextxy(UNDO_BUTTON_X, UNDO_BUTTON_Y-25, _T("无法悔棋")); // 提示无法悔棋
                    Sleep(1000); // 显示提示 1 秒
                }
            }

            // 玩家落子
            if (state.currentPlayer == state.init_choice && ui.handleInput(state, lastX, lastY, msg)) {
                game.makeMove(state, lastX, lastY);
                if (state.isGameOver == true) {
                    break;
                }
            }

            // AI 落子
            if (state.currentPlayer != state.init_choice) {
                ui.drawBoard(state, game); //绘制棋盘、悔棋按钮,显示回合数
                //Sleep(1000); // 用于调试高亮功能
                game.makeAIMove(state);
                if (state.isGameOver == true) {
                    break;
                }
            }
        }

        // 检查是否打开菜单
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) { // 按下 ESC 键
            int menuChoice = ui.showInGameMenu();
            if (menuChoice == 1) continue; // 继续游戏
            if (menuChoice == 2) { // 保存游戏
                if (fileIO.saveGame(game,state)) {
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
            if (menuChoice == 4) {
                main();
                return 0;
            }
        }
    }
    ui.drawBoard(state, game);//画出最后一粒棋子
    ui.showResult(state);// 显示游戏结果
    // 游戏结束后的交互
    while (true) {
        Sleep(5);
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