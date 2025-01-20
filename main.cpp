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
    // ��ʼ��ͼ�λ�����
    ui.initUI();

    // ������
    while (true) {
        Sleep(5);
        int choice = ui.showMainMenu();
        if (choice == 1) { // ��ʼ����Ϸ
            game.initGame(state);
            //����ѡ�ӽ���
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
        else if (choice == 2) { // ���ش浵
            if (fileIO.loadGame(game,state)) {
                break;
            }
            else {
                outtextxy(200, 350, _T("���ش浵ʧ�ܣ�"));
                Sleep(1000); // ��ʾ 1 ��
            }
        }
        else if (choice == 3) { // �˳���Ϸ
            ui.closeUI();
            return 0;
        }
    }

    // ��Ϸ��ѭ��
    int lastX = -1, lastY = -1; // ��¼���һ�����ӵ�λ��
    while (!state.isGameOver) {
        Sleep(5);

        ui.drawBoard(state, game); //�������̡����尴ť,��ʾ�غ���
        
        // AI������Ԫ
        if (state.init_choice == 2 && game.turnnumber() == 0) {
            game.makeMove(state, BOARD_SIZE / 2, BOARD_SIZE / 2);
            ui.drawBoard(state, game); //�������̡����尴ť,��ʾ�غ���
        }

        if (MouseHit()) {
            MOUSEMSG msg = GetMouseMsg();

            // ������尴ť����¼�
            if (ui.handleUndoButtonClick(msg)) {
                if (state.currentPlayer != state.init_choice) {
                    outtextxy(UNDO_BUTTON_X, UNDO_BUTTON_Y-25, _T("�ⲻ����Ļغ�")); // ��ʾ�޷�����
                    Sleep(1000); // ��ʾ��ʾ 1 ��
                }
                else if (!game.undoMove(state)) { // �����������ſ��Ի���
                    outtextxy(UNDO_BUTTON_X, UNDO_BUTTON_Y-25, _T("�޷�����")); // ��ʾ�޷�����
                    Sleep(1000); // ��ʾ��ʾ 1 ��
                }
            }

            // �������
            if (state.currentPlayer == state.init_choice && ui.handleInput(state, lastX, lastY, msg)) {
                game.makeMove(state, lastX, lastY);
                if (state.isGameOver == true) {
                    break;
                }
            }

            // AI ����
            if (state.currentPlayer != state.init_choice) {
                ui.drawBoard(state, game); //�������̡����尴ť,��ʾ�غ���
                //Sleep(1000); // ���ڵ��Ը�������
                game.makeAIMove(state);
                if (state.isGameOver == true) {
                    break;
                }
            }
        }

        // ����Ƿ�򿪲˵�
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) { // ���� ESC ��
            int menuChoice = ui.showInGameMenu();
            if (menuChoice == 1) continue; // ������Ϸ
            if (menuChoice == 2) { // ������Ϸ
                if (fileIO.saveGame(game,state)) {
                    outtextxy(200, 350, _T("��Ϸ�ѱ��棡"));
                    Sleep(1000); // ��ʾ 1 ��
                }
                else {
                    outtextxy(200, 350, _T("����ʧ�ܣ�"));
                    Sleep(1000); // ��ʾ 1 ��
                }
            }
            if (menuChoice == 3) { // �˳���Ϸ
                ui.closeUI();
                return 0;
            }
            if (menuChoice == 4) {
                main();
                return 0;
            }
        }
    }
    ui.drawBoard(state, game);//�������һ������
    ui.showResult(state);// ��ʾ��Ϸ���
    // ��Ϸ������Ľ���
    while (true) {
        Sleep(5);
        int choice = ui.showGameOverMenu();
        if (choice == 1) { // �ص�������
            main(); // ��������������
            return 0;
        }
        else if (choice == 2) { // �˳���Ϸ
            ui.closeUI();
            return 0;
        }
    }
}