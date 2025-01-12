#ifndef UI_H
#define UI_H

#include "game.h" // ������Ϸ״̬�Ķ���
#include "FileIO.h" // �����ļ���������
#include <graphics.h> // EASYX ͼ�ο�

class UI {
private:
    const int BOARD_START_X = 50; // �������Ͻ� X ����
    const int BOARD_START_Y = 50; // �������Ͻ� Y ����
    const int CELL_SIZE = 40;     // ÿ�����ӵĴ�С�����أ�
public:
    // ��ʼ��ͼ�λ�����
    void initUI() {
        initgraph(800, 600); // ��ʼ�� 800x600 �Ĵ���
        setbkcolor(WHITE); // ���ñ�����ɫΪ��ɫ
        cleardevice(); // �����Ļ
        settextcolor(BLACK);//����������ɫ
        settextstyle(20, 0, _T("����")); // ��������

        //// ���ر���������ͼƬ
        //loadImages();
    }

    // �ر�ͼ�λ�����
    void closeUI() {
        // �ͷ�ͼƬ��Դ
        //deleteImage(boardImage);
        //deleteImage(blackPieceImage);
        //deleteImage(whitePieceImage);
        closegraph(); // �ر�ͼ�λ�����
    }

    //// �����û����루���������
    //bool handleInput(GameState& state, int& lastX, int& lastY) {
    //    if (MouseHit()) {
    //        MOUSEMSG msg = GetMouseMsg();
    //        if (msg.uMsg == WM_LBUTTONDOWN) {
    //            // �������������ת��Ϊ�����ϵ�����
    //            lastX = (msg.y - BOARD_START_Y) / CELL_SIZE;
    //            lastY = (msg.x - BOARD_START_X) / CELL_SIZE;

    //            // ������Ƿ������̷�Χ��
    //            if (lastX >= 0 && lastX < 15 && lastY >= 0 && lastY < 15) {
    //                return true; // ������Ч
    //            }
    //        }
    //    }
    //    return false; // ������Ч
    //}

    //// ��������
    //void drawBoard(const GameState& state) {
    //    // ���Ʊ���ͼƬ
    //    putimage(0, 0, boardImage);

    //    // ��������
    //    for (int i = 0; i < state.board.size(); i++) {
    //        for (int j = 0; j < state.board[i].size(); j++) {
    //            if (state.board[i][j] == 1) { // ����
    //                drawPiece(i, j, 1);
    //            }
    //            else if (state.board[i][j] == 2) { // ����
    //                drawPiece(i, j, 2);
    //            }
    //        }
    //    }
    //}

    //// ��������
    //void drawPiece(int x, int y, int player) {
    //    // ������������Ļ�ϵ�����
    //    int posX = BOARD_START_X + y * CELL_SIZE; // �кŶ�Ӧ X ����
    //    int posY = BOARD_START_Y + x * CELL_SIZE; // �кŶ�Ӧ Y ����

    //    // ��������
    //    if (player == 1) { // ����
    //        putimage(posX - 15, posY - 15, blackPieceImage); // ����ͼƬ����
    //    }
    //    else if (player == 2) { // ����
    //        putimage(posX - 15, posY - 15, whitePieceImage); // ����ͼƬ����
    //    }
    //}
    // ��ʾ������
    int showMainMenu() {
        cleardevice(); // �����Ļ

        // �������˵�ѡ��
        settextstyle(20, 0, _T("����"));
        outtextxy(300, 100, _T("������"));
        outtextxy(300, 200, _T("1. ��ʼ����Ϸ"));
        outtextxy(300, 250, _T("2. ���ش浵"));
        outtextxy(300, 300, _T("3. �˳���Ϸ"));

        // ���������
        while (true) {
            if (MouseHit()) {
                MOUSEMSG msg = GetMouseMsg();
                if (msg.uMsg == WM_LBUTTONDOWN) {
                    // �����λ��
                    if (msg.x >= 300 && msg.x <= 500) {
                        if (msg.y >= 200 && msg.y <= 220) return 1; // ��ʼ����Ϸ
                        if (msg.y >= 250 && msg.y <= 270) return 2; // ���ش浵
                        if (msg.y >= 300 && msg.y <= 320) return 3; // �˳���Ϸ
                    }
                }
            }
        }
    }

    // ��ʾ��Ϸ�����еĲ˵�
    int showInGameMenu() {
        // ���Ʋ˵�
        cleardevice(); // �����Ļ
        settextstyle(20, 0, _T("����"));
        outtextxy(200, 100, _T("�˵�"));
        outtextxy(200, 200, _T("1. ������Ϸ"));
        outtextxy(200, 250, _T("2. ������Ϸ"));
        outtextxy(200, 300, _T("3. �˳���Ϸ"));

        // ���������
        while (true) {
            if (MouseHit()) {
                MOUSEMSG msg = GetMouseMsg();
                if (msg.uMsg == WM_LBUTTONDOWN) {
                    if (msg.x >= 200 && msg.x <= 400) {
                        if (msg.y >= 200 && msg.y <= 220) return 1; // ������Ϸ
                        if (msg.y >= 250 && msg.y <= 270) return 2; // ������Ϸ
                        if (msg.y >= 300 && msg.y <= 320) return 3; // �˳���Ϸ
                    }
                }
            }
        }
    }


    // ��ʾ��Ϸ�������ѡ��
    int showGameOverMenu() {
        // ������Ϸ�������ѡ��
        cleardevice(); // �����Ļ
        setbkcolor(WHITE);
        settextstyle(20, 0, _T("����"));
        outtextxy(200, 100, _T("��Ϸ����"));
        outtextxy(200, 200, _T("1. �ص�������"));
        outtextxy(200, 250, _T("2. �˳���Ϸ"));

        // ���������
        while (true) {
            if (MouseHit()) {
                MOUSEMSG msg = GetMouseMsg();
                if (msg.uMsg == WM_LBUTTONDOWN) {
                    if (msg.x >= 200 && msg.x <= 400) {
                        if (msg.y >= 200 && msg.y <= 220) return 1; // �ص�������
                        if (msg.y >= 250 && msg.y <= 270) return 2; // �˳���Ϸ
                    }
                }
            }
        }
    }

    // ��ʾ��Ϸ���
    void showResult(const GameState& state) {
        cleardevice();
        setbkcolor(WHITE);
        settextstyle(20, 0, _T("����"));
        if (state.winner == 1) {
            outtextxy(300, 200, _T("�ڷ�ʤ����"));
        }
        else if (state.winner == 2) {
            outtextxy(300, 200, _T("�׷�ʤ����"));
        }
        else {
            outtextxy(300, 200, _T("ƽ�֣�"));
        }
        outtextxy(300, 300, _T("�������λ�ü���"));
        while (!MouseHit()); // �ȴ��û����
    }

    void drawBoard(const GameState& state) {
        // ����˫����
        BeginBatchDraw();

        // ���ñ�����ɫ
        setbkcolor(YELLOW);
        cleardevice();

        // ����������ɫ
        setlinecolor(BLACK);

        // �������Ͻ���ʼλ�ú͸��Ӵ�С
        const int BOARD_START_X = 30;
        const int BOARD_START_Y = 30;
        const int CELL_SIZE = 40;
        const int BOARD_SIZE = 15; // 15x15 ������

        // ������������
        for (int i = 0; i < BOARD_SIZE; i++) {
            // ˮƽ��
            line(BOARD_START_X, BOARD_START_Y + i * CELL_SIZE,
                BOARD_START_X + (BOARD_SIZE - 1) * CELL_SIZE, BOARD_START_Y + i * CELL_SIZE);
            // ��ֱ��
            line(BOARD_START_X + i * CELL_SIZE, BOARD_START_Y,
                BOARD_START_X + i * CELL_SIZE, BOARD_START_Y + (BOARD_SIZE - 1) * CELL_SIZE);
        }

        // ��������
        for (int i = 0; i < state.board.size(); i++) {
            for (int j = 0; j < state.board[i].size(); j++) {
                if (state.board[i][j] == 1) { // ����
                    drawPiece(i, j, 1);
                }
                else if (state.board[i][j] == 2) { // ����
                    drawPiece(i, j, 2);
                }
            }
        }

        // ����˫���壬ˢ����Ļ
        EndBatchDraw();
    }

    void drawPiece(int x, int y, int player) {
        // �������Ͻ���ʼλ�ú͸��Ӵ�С
        const int BOARD_START_X = 30;
        const int BOARD_START_Y = 30;
        const int CELL_SIZE = 40;

        // ����������������
        int centerX = BOARD_START_X + y * CELL_SIZE;
        int centerY = BOARD_START_Y + x * CELL_SIZE;

        // ����������ɫ
        if (player == 1) { // ����
            setfillcolor(BLACK);
        }
        else if (player == 2) { // ����
            setfillcolor(WHITE);
        }

        // �������ӣ�ʵ��Բ��
        solidcircle(centerX, centerY, CELL_SIZE / 2 - 2); // ���Ӱ뾶Ϊ���Ӵ�С��һ���ȥ 2 ����
    }

    bool handleInput(GameState& state, int& lastX, int& lastY) {
        if (MouseHit()) {
            MOUSEMSG msg = GetMouseMsg();
            if (msg.uMsg == WM_LBUTTONDOWN) {
                // �������Ͻ���ʼλ�ú͸��Ӵ�С
                const int BOARD_START_X = 30;
                const int BOARD_START_Y = 30;
                const int CELL_SIZE = 40;

                // �������������ת��Ϊ�����ϵ�����
                lastX = (msg.y - BOARD_START_Y + CELL_SIZE / 2) / CELL_SIZE;
                lastY = (msg.x - BOARD_START_X + CELL_SIZE / 2) / CELL_SIZE;

                // ������Ƿ������̷�Χ��
                if (lastX >= 0 && lastX < 15 && lastY >= 0 && lastY < 15) {
                    return true; // ������Ч
                }
            }
        }
        return false; // ������Ч
    }

//private:
//    // ͼƬ��Դ
//    IMAGE* boardImage; // ���̱���ͼƬ
//    IMAGE* blackPieceImage; // ����ͼƬ
//    IMAGE* whitePieceImage; // ����ͼƬ
//
//    // ����ͼƬ
//    void loadImages() {
//        boardImage = new IMAGE;
//        blackPieceImage = new IMAGE;
//        whitePieceImage = new IMAGE;
//
//        loadimage(boardImage, _T("board.png")); // �������̱���ͼƬ
//        loadimage(blackPieceImage, _T("black_piece.png")); // ���غ���ͼƬ
//        loadimage(whitePieceImage, _T("white_piece.png")); // ���ذ���ͼƬ
//    }
//
//    // �ͷ�ͼƬ��Դ
//    void deleteImage(IMAGE* img) {
//        if (img) {
//            delete img;
//            img = nullptr;
//        }
//    }
};

#endif // UI_H