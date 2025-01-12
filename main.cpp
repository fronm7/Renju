#include "UI.h"
#include "game.h"
#include "FileIO.h"

int main() {
    GameState state;
    Game game;
    UI ui;
    FileIO fileIO;

    // ��ʼ��ͼ�λ�����
    ui.initUI();

    // ������
    while (true) {
        int choice = ui.showMainMenu();
        if (choice == 1) { // ��ʼ����Ϸ
            game.initGame(state);
            break;
        }
        else if (choice == 2) { // ���ش浵
            if (fileIO.loadGame(state)) {
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
        ui.drawBoard(state);
        if (ui.handleInput(state, lastX, lastY)) { // ����������룬������ lastX �� lastY
            // �������
            game.makeMove(state, lastX, lastY);
            if (state.isGameOver == true) {
                ui.showResult(state); // ��ʾ��Ϸ���
                break;
            }

            // AI ����
            if (state.currentPlayer == 2) {
                game.makeAIMove(state);
                if (state.isGameOver == true) {
                    ui.showResult(state); // ��ʾ��Ϸ���
                    break;
                }

                // �л������
                state.currentPlayer = 1;
            }
        }

        // ����Ƿ�򿪲˵�
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) { // ���� ESC ��
            int menuChoice = ui.showInGameMenu();
            if (menuChoice == 1) continue; // ������Ϸ
            if (menuChoice == 2) { // ������Ϸ
                if (fileIO.saveGame(state)) {
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
        }
    }

    // ��Ϸ������Ľ���
    while (true) {
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