#ifndef UI_H
#define UI_H

#include "game.h" // 包含游戏状态的定义
#include "FileIO.h" // 包含文件操作功能
#include <graphics.h> // EASYX 图形库
const int BOARD_START_X = 25; // 棋盘左上角 X 坐标
const int BOARD_START_Y = 25; // 棋盘左上角 Y 坐标
const int CELL_SIZE = 38;     // 每个格子的大小（像素）
const int UNDO_BUTTON_X = 610; // 悔棋按钮左上角 X 坐标
const int UNDO_BUTTON_Y = 150;  // 悔棋按钮左上角 Y 坐标
const int UNDO_BUTTON_WIDTH = 100; // 悔棋按钮宽度
const int UNDO_BUTTON_HEIGHT = 40; // 悔棋按钮高度

class UI {
public:
    // 初始化图形化界面
    void initUI() {
        initgraph(800, 600); // 初始化 800x600 的窗口
        setbkcolor(WHITE); // 设置背景颜色为白色
        cleardevice(); // 清空屏幕
        settextcolor(BLACK);//设置字体颜色
        settextstyle(20, 0, _T("宋体")); // 设置字体

        //// 加载背景和棋子图片
        //loadImages();
    }

    // 关闭图形化界面
    void closeUI() {
        // 释放图片资源
        //deleteImage(boardImage);
        //deleteImage(blackPieceImage);
        //deleteImage(whitePieceImage);
        closegraph(); // 关闭图形化界面
    }

    //// 处理用户输入（如鼠标点击）
    //bool handleInput(GameState& state, int& lastX, int& lastY) {
    //    if (MouseHit()) {
    //        MOUSEMSG msg = GetMouseMsg();
    //        if (msg.uMsg == WM_LBUTTONDOWN) {
    //            // 将鼠标点击的坐标转换为棋盘上的行列
    //            lastX = (msg.y - BOARD_START_Y) / CELL_SIZE;
    //            lastY = (msg.x - BOARD_START_X) / CELL_SIZE;

    //            // 检查点击是否在棋盘范围内
    //            if (lastX >= 0 && lastX < 15 && lastY >= 0 && lastY < 15) {
    //                return true; // 输入有效
    //            }
    //        }
    //    }
    //    return false; // 输入无效
    //}

    //// 绘制棋盘
    //void drawBoard(const GameState& state) {
    //    // 绘制背景图片
    //    putimage(0, 0, boardImage);

    //    // 绘制棋子
    //    for (int i = 0; i < state.board.size(); i++) {
    //        for (int j = 0; j < state.board[i].size(); j++) {
    //            if (state.board[i][j] == 1) { // 黑子
    //                drawPiece(i, j, 1);
    //            }
    //            else if (state.board[i][j] == 2) { // 白子
    //                drawPiece(i, j, 2);
    //            }
    //        }
    //    }
    //}

    //// 绘制棋子
    //void drawPiece(int x, int y, int player) {
    //    // 计算棋子在屏幕上的坐标
    //    int posX = BOARD_START_X + y * CELL_SIZE; // 列号对应 X 坐标
    //    int posY = BOARD_START_Y + x * CELL_SIZE; // 行号对应 Y 坐标

    //    // 绘制棋子
    //    if (player == 1) { // 黑子
    //        putimage(posX - 15, posY - 15, blackPieceImage); // 棋子图片居中
    //    }
    //    else if (player == 2) { // 白子
    //        putimage(posX - 15, posY - 15, whitePieceImage); // 棋子图片居中
    //    }
    //}
    // 显示主界面
    int showMainMenu() {
        cleardevice(); // 清空屏幕

        // 绘制主菜单选项
        settextstyle(20, 0, _T("宋体"));
        outtextxy(300, 100, _T("五子棋"));
        outtextxy(300, 200, _T("1. 开始新游戏"));
        outtextxy(300, 250, _T("2. 加载存档"));
        outtextxy(300, 300, _T("3. 退出游戏"));

        // 处理鼠标点击
        while (true) {
            if (MouseHit()) {
                MOUSEMSG msg = GetMouseMsg();
                if (msg.uMsg == WM_LBUTTONDOWN) {
                    // 检查点击位置
                    if (msg.x >= 300 && msg.x <= 500) {
                        if (msg.y >= 200 && msg.y <= 220) return 1; // 开始新游戏
                        if (msg.y >= 250 && msg.y <= 270) return 2; // 加载存档
                        if (msg.y >= 300 && msg.y <= 320) return 3; // 退出游戏
                    }
                }
            }
        }
    }

    // 显示游戏过程中的菜单
    int showInGameMenu() {
        // 绘制菜单
        cleardevice(); // 清空屏幕
        settextstyle(20, 0, _T("宋体"));
        outtextxy(200, 100, _T("菜单"));
        outtextxy(200, 150, _T("1. 继续游戏"));
        outtextxy(200, 200, _T("2. 保存游戏"));
        outtextxy(200, 250, _T("3. 退出游戏"));
        outtextxy(200, 300, _T("4. 返回主菜单"));

        // 处理鼠标点击
        while (true) {
            if (MouseHit()) {
                MOUSEMSG msg = GetMouseMsg();
                if (msg.uMsg == WM_LBUTTONDOWN) {
                    if (msg.x >= 200 && msg.x <= 400) {
                        if (msg.y >= 150 && msg.y <= 170) return 1; // 继续游戏
                        if (msg.y >= 200 && msg.y <= 220) return 2; // 保存游戏
                        if (msg.y >= 250 && msg.y <= 270) return 3; // 退出游戏
                        if (msg.y >= 300 && msg.y <= 320) return 4; // 返回主菜单
                    }
                }
            }
        }
    }

    int showSelectMenu() {
        // 绘制选子菜单
        cleardevice(); // 清空屏幕
        settextstyle(20, 0, _T("宋体"));
        outtextxy(200, 100, _T("请选择执黑或执白"));
        outtextxy(200, 200, _T("1. 执黑"));
        outtextxy(200, 250, _T("2. 执白"));
        outtextxy(200, 300, _T("3. 返回主菜单"));

        // 处理鼠标点击
        while (true) {
            if (MouseHit()) {
                MOUSEMSG msg = GetMouseMsg();
                if (msg.uMsg == WM_LBUTTONDOWN) {
                    if (msg.x >= 200 && msg.x <= 400) {
                        if (msg.y >= 200 && msg.y <= 220) return 1; // 执黑
                        if (msg.y >= 250 && msg.y <= 270) return 2; // 执白
                        if (msg.y >= 300 && msg.y <= 320) return 3; // 返回主菜单
                    }
                }
            }
        }
    }

    // 显示游戏结束后的选项
    int showGameOverMenu() {
        // 绘制游戏结束后的选项
        cleardevice(); // 清空屏幕
        setbkcolor(WHITE);
        settextstyle(20, 0, _T("宋体"));
        outtextxy(200, 100, _T("游戏结束"));
        outtextxy(200, 200, _T("1. 返回主菜单"));
        outtextxy(200, 250, _T("2. 退出游戏"));

        // 处理鼠标点击
        while (true) {
            if (MouseHit()) {
                MOUSEMSG msg = GetMouseMsg();
                if (msg.uMsg == WM_LBUTTONDOWN) {
                    if (msg.x >= 200 && msg.x <= 400) {
                        if (msg.y >= 200 && msg.y <= 220) return 1; // 回到主界面
                        if (msg.y >= 250 && msg.y <= 270) return 2; // 退出游戏
                    }
                }
            }
        }
    }

    // 显示游戏结果
    void showResult(const GameState& state) {
        //cleardevice();  //清除棋盘和棋子
        setbkcolor(WHITE);
        settextstyle(20, 0, _T("宋体"));
        if (state.winner == 1) {
            outtextxy(600, 300, _T("黑方胜利！"));
        }
        else if (state.winner == 2) {
            outtextxy(600, 300, _T("白方胜利！"));
        }
        else {
            outtextxy(600, 300, _T("平局！"));
        }
        outtextxy(600, 400, _T("点击任意位置继续"));
        //while (!MouseHit()); // 等待用户点击
        //Sleep(1000);  //显示获胜方界面1秒。调试语句，使用此语句可以让获胜文字停留在界面上。
        while (true) {
            //outtextxy(600, 500, rand());
            if (MouseHit()) {
                MOUSEMSG msg = GetMouseMsg();
                if (msg.uMsg == WM_LBUTTONDOWN)
                    //outtextxy(600, 400, rand());
                    break;
            }
            //else{
                //outtextxy(600, 450, rand());
                //break;
            //} 
        }
    }

    void drawBoard(const GameState& state, Game& game) {
        // 启用双缓冲
        BeginBatchDraw();

        // 设置背景颜色
        setbkcolor(WHITE);
        cleardevice();

        // 设置线条颜色
        setlinecolor(BLACK);

        // 绘制棋盘线条
        for (int i = 0; i < BOARD_SIZE; i++) {
            // 水平线
            line(BOARD_START_X, BOARD_START_Y + i * CELL_SIZE,
                BOARD_START_X + (BOARD_SIZE - 1) * CELL_SIZE, BOARD_START_Y + i * CELL_SIZE);
            // 垂直线
            line(BOARD_START_X + i * CELL_SIZE, BOARD_START_Y,
                BOARD_START_X + i * CELL_SIZE, BOARD_START_Y + (BOARD_SIZE - 1) * CELL_SIZE);
        }

        // 绘制棋子
        for (int i = 0; i < state.board.size(); i++) {
            for (int j = 0; j < state.board[i].size(); j++) {
                if (state.board[i][j] == 1) { // 黑子
                    drawPiece(i, j, 1);
                }
                else if (state.board[i][j] == 2) { // 白子
                    drawPiece(i, j, 2);
                }
            }
        }
        drawUndoButton();
        drawUI(game);
        // 结束双缓冲，刷新屏幕
        EndBatchDraw();
    }

    void drawPiece(int x, int y, int player) {

        // 计算棋子中心坐标
        int centerX = BOARD_START_X + y * CELL_SIZE;
        int centerY = BOARD_START_Y + x * CELL_SIZE;

        // 设置棋子颜色
        if (player == 1) { // 黑子
            setfillcolor(BLACK);
            // 绘制棋子（实心圆）
            solidcircle(centerX, centerY, CELL_SIZE / 2 - 2); // 棋子半径为格子大小的一半减去 2 像素
        }
        else if (player == 2) { // 白子
            setlinecolor(BLACK);
            setfillcolor(WHITE);
            solidcircle(centerX, centerY, CELL_SIZE / 2 - 2);
            circle(centerX, centerY, CELL_SIZE / 2 - 2); // 绘制空心圆
        }  
    }

    bool handleInput(GameState& state, int& lastX, int& lastY, const MOUSEMSG& msg) {
        //if (MouseHit()) {
            //MOUSEMSG msg = GetMouseMsg();
            if (msg.uMsg == WM_LBUTTONDOWN) {

                // 将鼠标点击的坐标转换为棋盘上的行列
                lastX = (msg.y - BOARD_START_Y + CELL_SIZE / 2) / CELL_SIZE;
                lastY = (msg.x - BOARD_START_X + CELL_SIZE / 2) / CELL_SIZE;

                // 检查点击是否在棋盘范围内
                if (lastX >= 0 && lastX < 15 && lastY >= 0 && lastY < 15) {
                    return true; // 输入有效
                }
            }
        //}
        return false; // 输入无效
    }

    void drawUndoButton() {
        // 设置按钮背景颜色
        setfillcolor(LIGHTGRAY);
        solidrectangle(UNDO_BUTTON_X, UNDO_BUTTON_Y,
            UNDO_BUTTON_X + UNDO_BUTTON_WIDTH,
            UNDO_BUTTON_Y + UNDO_BUTTON_HEIGHT);

        // 设置按钮边框颜色
        setlinecolor(BLACK);
        rectangle(UNDO_BUTTON_X, UNDO_BUTTON_Y,
            UNDO_BUTTON_X + UNDO_BUTTON_WIDTH,
            UNDO_BUTTON_Y + UNDO_BUTTON_HEIGHT);

        // 设置按钮文字
        settextstyle(20, 0, _T("宋体"));
        settextcolor(BLACK);
        outtextxy(UNDO_BUTTON_X + 30, UNDO_BUTTON_Y + 11, _T("悔棋"));
    }

    bool handleUndoButtonClick(const MOUSEMSG& msg) {
        // 检查鼠标点击是否在悔棋按钮范围内
        if (msg.uMsg == WM_LBUTTONDOWN &&
            msg.x >= UNDO_BUTTON_X && msg.x <= UNDO_BUTTON_X + UNDO_BUTTON_WIDTH &&
            msg.y >= UNDO_BUTTON_Y && msg.y <= UNDO_BUTTON_Y + UNDO_BUTTON_HEIGHT) {
            return true; // 点击了悔棋按钮
        }
        return false; // 未点击悔棋按钮
    }

    void drawUI(Game& game) {
        // 设置文字样式
        settextstyle(20, 0, _T("宋体"));
        settextcolor(BLACK);
        // 显示当前回合数
        TCHAR turnText[50];
        swprintf_s(turnText, _T("回合数: %d"), game.turnnumber());
        outtextxy(UNDO_BUTTON_X, UNDO_BUTTON_Y+50, turnText);
    }

private:
    // 图片资源
    IMAGE* boardImage; // 棋盘背景图片
    IMAGE* blackPieceImage; // 黑子图片
    IMAGE* whitePieceImage; // 白子图片
    // 加载图片
    void loadImages() {
        boardImage = new IMAGE;
        blackPieceImage = new IMAGE;
        whitePieceImage = new IMAGE;

        loadimage(boardImage, _T("board.png")); // 加载棋盘背景图片
        loadimage(blackPieceImage, _T("black_piece.png")); // 加载黑子图片
        loadimage(whitePieceImage, _T("white_piece.png")); // 加载白子图片
    }

    // 释放图片资源
    void deleteImage(IMAGE* img) {
        if (img) {
            delete img;
            img = nullptr;
        }
    }
};

#endif // UI_H