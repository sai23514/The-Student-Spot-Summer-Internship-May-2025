#include <iostream>
#include <vector>
#include <limits>
#include <cstdlib>
#include <ctime>
using namespace std;
// TicTacToe Game Class
class TicTacToe {
private:
    vector<vector<char>> board;
    char currentPlayer;
    bool gameOver;
    char winner;
    int movesCount;
    bool vsComputer;

    void initializeBoard() {
        board = vector<vector<char>>(3, vector<char>(3, ' '));
        currentPlayer = 'X';
        gameOver = false;
        winner = ' ';
        movesCount = 0;
    }

    bool isValidMove(int row, int col) const {
        return row >= 0 && row < 3 && col >= 0 && col < 3 && board[row][col] == ' ';
    }

    bool checkWin() {
        for (int i = 0; i < 3; i++) {
            if (board[i][0] != ' ' &&
                board[i][0] == board[i][1] &&
                board[i][1] == board[i][2]) {
                winner = board[i][0];
                return true;
            }
        }

        for (int i = 0; i < 3; i++) {
            if (board[0][i] != ' ' &&
                board[0][i] == board[1][i] &&
                board[1][i] == board[2][i]) {
                winner = board[0][i];
                return true;
            }
        }

        if (board[0][0] != ' ' &&
            board[0][0] == board[1][1] &&
            board[1][1] == board[2][2]) {
            winner = board[0][0];
            return true;
        }

        if (board[0][2] != ' ' &&
            board[0][2] == board[1][1] &&
            board[1][1] == board[2][0]) {
            winner = board[0][2];
            return true;
        }

        return false;
    }

    bool checkDraw() {
        return movesCount == 9;
    }

    void switchPlayer() {
        currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
    }

    void computerMove() {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == ' ') {
                    board[i][j] = 'O';
                    if (checkWin()) {
                        movesCount++;
                        return;
                    }
                    board[i][j] = ' ';
                }
            }
        }

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == ' ') {
                    board[i][j] = 'X';
                    if (checkWin()) {
                        board[i][j] = 'O';
                        movesCount++;
                        return;
                    }
                    board[i][j] = ' ';
                }
            }
        }

        if (board[1][1] == ' ') {
            board[1][1] = 'O';
            movesCount++;
            return;
        }

        vector<pair<int, int>> corners = {{0, 0}, {0, 2}, {2, 0}, {2, 2}};
        for (auto corner : corners) {
            if (board[corner.first][corner.second] == ' ') {
                board[corner.first][corner.second] = 'O';
                movesCount++;
                return;
            }
        }

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == ' ') {
                    board[i][j] = 'O';
                    movesCount++;
                    return;
                }
            }
        }
    }

public:
    TicTacToe(bool playAgainstComputer = false) : vsComputer(playAgainstComputer) {
        srand(static_cast<unsigned int>(time(nullptr)));
        initializeBoard();
    }

    void displayBoard() const {
        cout << "\n  TIC-TAC-TOE\n";
        cout << "  1 2 3  (columns)\n";
        for (int i = 0; i < 3; i++) {
            cout << i + 1 << " ";
            for (int j = 0; j < 3; j++) {
                cout << board[i][j];
                if (j < 2) cout << "|";
            }
            cout << endl;
            if (i < 2) cout << "  -+-+-" << endl;
        }
        cout << "  (rows)\n";
    }

    bool makeMove(int row, int col) {
        row--; col--;
        if (!isValidMove(row, col)) {
            return false;
        }

        board[row][col] = currentPlayer;
        movesCount++;

        if (checkWin()) {
            gameOver = true;
            return true;
        }

        if (checkDraw()) {
            gameOver = true;
            winner = ' '; // Explicitly set draw status
            return true;
        }

        switchPlayer();

        if (vsComputer && currentPlayer == 'O' && !gameOver) {
            computerMove();
            if (checkWin()) {
                gameOver = true;
                return true;
            }
            if (checkDraw()) {
                gameOver = true;
                winner = ' '; // Set winner on draw
                return true;
            }
            switchPlayer();
        }

        return true;
    }

    bool isGameOver() const {
        return gameOver;
    }

    char getWinner() const {
        return winner;
    }

    char getCurrentPlayer() const {
        return currentPlayer;
    }

    void resetGame() {
        initializeBoard();
    }

    void setGameMode(bool playAgainstComputer) {
        vsComputer = playAgainstComputer;
        resetGame();
    }

    bool isDraw() const {
        return gameOver && winner == ' ';
    }
};

// Utility functions
int getValidInput(int min, int max) {
    int input;
    while (true) {
        if (cin >> input && input >= min && input <= max) {
            break;
        } else {
            cout << "Invalid input. Please enter a number between " << min << " and " << max << ": ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    return input;
}

void displayMenu() {
    cout << "\n===== TIC-TAC-TOE MENU =====\n";
    cout << "1. Play against another player\n";
    cout << "2. Play against computer\n";
    cout << "3. View game rules\n";
    cout << "4. Exit\n";
    cout << "Enter your choice: ";
}

void displayRules() {
    cout << "\n===== TIC-TAC-TOE RULES =====\n";
    cout << "1. Played on a 3x3 grid.\n";
    cout << "2. Players alternate turns placing X or O.\n";
    cout << "3. First to align 3 marks wins.\n";
    cout << "4. If all cells are filled without a winner, it's a draw.\n";
    cout << "5. X always starts first.\n";
    cout << "6. Enter row and column numbers from 1 to 3 to make your move.\n";
}

int main() {
    TicTacToe game;
    bool running = true;
    int choice;

    cout << "Welcome to Tic-Tac-Toe!" << endl;

    while (running) {
        displayMenu();
        choice = getValidInput(1, 4);
        switch (choice) {
            case 1: {
                game.setGameMode(false);
                cout << "\nStarting 2-player mode...\n";

                while (!game.isGameOver()) {
                    game.displayBoard();
                    cout << "\nPlayer " << game.getCurrentPlayer() << "'s move:\n";
                    cout << "Enter row (1-3): ";
                    int row = getValidInput(1, 3);
                    cout << "Enter column (1-3): ";
                    int col = getValidInput(1, 3);

                    if (!game.makeMove(row, col)) {
                        cout << "Invalid move! Try again.\n";
                    }
                }

                game.displayBoard();
                if (game.isDraw()) {
                    cout << "\nGame ended in a tie!\n";
                } else {
                    cout << "\nPlayer " << game.getWinner() << " wins!\n";
                }
                break;
            }

            case 2: {
                game.setGameMode(true);
                cout << "\nStarting single-player vs Computer...\nYou are X and go first.\n";

                while (!game.isGameOver()) {
                    game.displayBoard();
                    if (game.getCurrentPlayer() == 'X') {
                        cout << "\nYour turn (X):\n";
                        cout << "Enter row (1-3): ";
                        int row = getValidInput(1, 3);
                        cout << "Enter column (1-3): ";
                        int col = getValidInput(1, 3);

                        if (!game.makeMove(row, col)) {
                            cout << "Invalid move! Try again.\n";
                        }
                    }
                }

                game.displayBoard();
                if (game.isDraw()) {
                    cout << "\nIt's a draw! Well played.\n";
                } else if (game.getWinner() == 'X') {
                    cout << "\nCongratulations! You win!\n";
                } else {
                    cout << "\nComputer wins! Better luck next time.\n";
                }
                break;
            }

            case 3:
                displayRules();
                break;

            case 4:
                cout << "\nThanks for playing. Goodbye!\n";
                running = false;
                break;
        }
        if (running) {
            cout << "\nPress Enter to continue...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
        }
    }
    return 0;
}
