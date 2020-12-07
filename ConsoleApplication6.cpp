#include <iostream>
#include <vector>
#include <map>
#include <utility>
#include <algorithm>

#include <random>
#include <ctime>
#include <cstdlib>
#include <ranges>

void clear_screen() {
#ifdef _WIN32 // windows
    system("cls");
#else // macOS and linux
    system("clear");
#endif //_WIN32
}
void pause() {
#ifdef _WIN32 // windows
    system("pause");
#else // macOS and linux
    system("read")
#endif //_WIN32
}

class Player
{
private:
    std::string m_type;
    unsigned char m_name;


public:
    int m_wins;
    int m_draws;
    std::string GetType()const { return m_type; };
    Player(unsigned char name, std::string&& type = "Player") :m_name(name), m_type(type), m_wins(0), m_draws(0) {}
    virtual unsigned char GetName()const { return m_name; }
    virtual int NextMove(std::map<int, unsigned char>& board) = 0;
    virtual ~Player() = default;
    int GetGameWins() const { return m_wins; }
    int GetDraws() const { return m_draws; }
};

class Human : public Player
{

public:
    Human(unsigned char name) :Player(name, "Human") {}
    virtual int NextMove(std::map<int, unsigned char>& board) override {
        int move;
        std::cout << "Enter a number on the board (e.g. 1): ";
        std::cin >> move;
        return move;
    }
    virtual ~Human() = default;
};

class Robot : public Player
{

public:
    Robot(unsigned char name) :Player(name, "Robot") {}

    bool CheckAvailability(std::map<int, unsigned char>& board, int number, std::vector<int>& keys) {
        for (auto& cell : board) {
            if (cell.first == number) {
                if (cell.second == '-') {
                    return true;
                }
            }
        }
        std::remove_if(keys.begin(), keys.end(), [&](auto& key) {
            return key == number;
            });
        return false;
    }
    virtual int NextMove(std::map<int, unsigned char>& board) override
    {
        std::vector<int>number = { 1,2,3,4,5,6,7,8,9 };
        int randNum = 0;
        std::srand(std::time(0));
        do
        {
            randNum = rand() % 9 + 1;
        } while (CheckAvailability(board, randNum, number) == false);
        return randNum;
    }

    virtual ~Robot() = default;
};

class NoughtsAndCrosses
{
private:

    std::vector<Player*>m_player;
    Player* one;
    Player* two;
    virtual bool ClaimSquare(std::map<int, unsigned char>& board, int move, Player* player)
    {
        auto validSquare = std::find_if(board.begin(), board.end(), [&](auto pair) {
            return pair.first == move;
            });
        if (validSquare != board.end())
        {
            if (validSquare->second == '-')
            {
                validSquare->second = player->GetName();
                return true;
            }
            else
            {
                std::cout << "This square has already been claimed. Choose a different square!" << std::endl;
                return false;
            }
        }
        return false;
    }
    //std::vector<Player*> m_p;
    std::map<int, unsigned char>board;
    void DisplayBoard()
    {

        for (auto const& cell : board)
        {
            if (cell.first % 3 == 1) {
                std::cout << "\n\n";
            }
            if (cell.second != '-') {
                std::cout << cell.second << "        ";
            }
            else
            {
                std::cout << cell.first << "        ";
            }
        }
        std::cout << "\n\n";
    }
    auto CheckForAWinner(std::map<int, unsigned char>& board, Player* player)
    {
        if (board.at(1) == player->GetName() && board.at(2) == player->GetName() && board.at(3) == player->GetName()) {
            return true;
        }
        else if (board.at(4) == player->GetName() && board.at(5) == player->GetName() && board.at(6) == player->GetName()) {
            return true;
        }
        else if (board.at(7) == player->GetName() && board.at(8) == player->GetName() && board.at(9) == player->GetName()) {
            return true;
        }
        else if (board.at(1) == player->GetName() && board.at(4) == player->GetName() && board.at(7) == player->GetName()) {
            return true;
        }
        else if (board.at(2) == player->GetName() && board.at(5) == player->GetName() && board.at(8) == player->GetName()) {
            return true;
        }
        else if (board.at(3) == player->GetName() && board.at(6) == player->GetName() && board.at(9) == player->GetName()) {
            return true;
        }
        else if (board.at(1) == player->GetName() && board.at(5) == player->GetName() && board.at(9) == player->GetName()) {
            return true;
        }
        else if (board.at(7) == player->GetName() && board.at(5) == player->GetName() && board.at(3) == player->GetName()) {
            return true;
        }
        else
        {
            return false;
        }
    }
    bool CheckForDraw(std::map<int, unsigned char>& board) {

        return std::all_of(board.begin(), board.end(), [&](auto& pair) {return pair.second != '-'; });
    }
public:
    NoughtsAndCrosses(Player& one, Player& two)
        :one(&one), two(&two)
    {
        m_player.push_back(&one);
        m_player.push_back(&two);

        board = { std::make_pair(1,'-'),std::make_pair(2,'-'),std::make_pair(3,'-'),
          std::make_pair(4,'-'),std::make_pair(5,'-'),std::make_pair(6,'-'),
          std::make_pair(7,'-'),std::make_pair(8,'-'),std::make_pair(9,'-') };
    }
    void ResetBoard() {
        std::for_each(board.begin(), board.end(), [&](auto& pair) {
            pair.second = '-';
            });
    }


    Player* play()
    {
        int currentPlayer = 1;
        bool isWinner = false;
        bool isDraw = false;
        do
        {
            currentPlayer = (currentPlayer + 1) % 2;
            do
            {
                system("cls");
                std::cout << m_player.at(currentPlayer)->GetType() << ": " << m_player.at(currentPlayer)->GetName() << " turn: " << std::endl;
                DisplayBoard();
                system("pause");
            } while (ClaimSquare(board, m_player.at(currentPlayer)->NextMove(board), m_player.at(currentPlayer)) == false);

            //std::cout << "\nPress enter to make the robot move. . .";
            //std::cin.get();
            //system("cls");
        } while (CheckForDraw(board) == false && (isWinner = CheckForAWinner(board, m_player.at(currentPlayer))) == false);


        if (isWinner == true)
        {
            return m_player.at(currentPlayer);
        }
        m_player.at(0)->m_draws++;
        m_player.at(1)->m_draws++;
        DisplayBoard();
        ResetBoard();

        return nullptr;
    }

};

int main() {

    Robot robot1('O');
    Robot robot2('X');
    std::vector<Robot>player = { robot1, robot2 };
    NoughtsAndCrosses game(robot1, robot2);

    int round = 3;
    int roundCount = 0;
    Player* winner;
    do
    {

        int gameCount = 1;
        int totalGamesinRound = 3;
        std::cout << "START GAME!\n";
        system("pause");
        system("cls");
        std::cout << "\nROUND " << ++roundCount << ". . .\n";
        do
        {
            std::cout << "Game " << gameCount << " of round " << roundCount << "\n";
            winner = game.play();

            if (winner != nullptr)
            {
                std::cout << "Winner of game " << gameCount << " is type: " << winner->GetType() << ": " << winner->GetName() << "\n";
                winner->m_wins++;
            }
            else
            {
                system("cls");
                std::cout << "Game " << gameCount << " is a draw!\n";

            }

            gameCount++;
            totalGamesinRound--;
        } while (totalGamesinRound != 0);

        /* std::cout << "Game 2: Human vs Robot\n";
         game.play(robot1, robot1);*/
        std::cout << "Wins for " << robot1.GetType() << ": Player : " << robot1.GetName() << " - " << robot1.GetGameWins() << "\n";
        std::cout << "Wins for " << robot2.GetType() << ": Player : " << robot2.GetName() << " - " << robot2.GetGameWins() << "\n";
        std::cout << "Drawed: " << robot1.GetDraws() << "\n";

        auto playerWithMostWins = std::max_element(player.begin(), player.end(),
            [](const auto& lhs, const auto& rhs)
            {
                return lhs.GetGameWins() < rhs.GetGameWins();
            });

        std::cout << "Winner of round " << roundCount << " is " << playerWithMostWins->GetName() << "\n";
        round--;
    } while (round != 0);
}