#include <iostream>
#include <vector>
#include <iterator>
#include <random>
#include <conio.h>
#include "windows.h"

using namespace std;

const char charApple = '#';
const char charFillField = '-';
const char charSnake = 'o';
const int countRow = 4;
const int countColumn = 8;
const int countCell = countRow * countColumn;
const char upDirection = 'w';
const char leftDirection = 'a';
const char downDirection = 's';
const char rigthDirection = 'd';
const char exitChar = 'q';
const char headSnakeChar = '@';
const int speadSnake = 400;

struct Apple
{
    int x;
    int y;
};

struct Snake
{
    int x;
    int y;
    char direction;
};

void PrintField(vector<vector<char>>& field)
{
    for_each(field.begin(), field.end(), [](const vector<char>& v)
        {
            copy(v.begin(), v.end(), ostream_iterator<char>(cout, " "));
            cout << endl;
        });
}

void FillField(vector<vector<char>>& field)
{
    vector<char> rowField(countColumn);
    for_each(rowField.begin(), rowField.end(), [](char& v) mutable { v = charFillField; });
    for_each(field.begin(), field.end(), [rowField = rowField](vector<char>& i) mutable { i = rowField; });
}

int GetRandomValue(int minValue, int maxValue)
{
    random_device dev;
    mt19937 rng(dev());
    uniform_int_distribution<std::mt19937::result_type> dist6(minValue, maxValue);
    return dist6(rng);
}

void CreateApple(Apple& apple, vector<Snake>& snake)
{
    apple.y = GetRandomValue(0, countRow - 1);
    apple.x = GetRandomValue(0, countColumn - 1);

    for (int i = 0; i < snake.size(); i++)
    {
        Snake snakePart = snake[i];
        if (snakePart.x == apple.x && snakePart.y == apple.y)
        {
            apple.y = GetRandomValue(0, countRow - 1);
            apple.x = GetRandomValue(0, countColumn - 1);
            i = 0;
        }
    }
}

void AddAppleToField(vector<vector<char>>& field, Apple& apple)
{
    field[apple.y][apple.x] = charApple;
}

void CreateSnake(vector<vector<char>>& field, vector<Snake>& snake)
{
    Snake headSnake;
    headSnake.x = GetRandomValue(countColumn - 5, countColumn - 2);
    headSnake.y = GetRandomValue(0, countRow - 1);
    headSnake.direction = leftDirection;
    snake.push_back(headSnake);

    Snake tailSnake;
    tailSnake.x = headSnake.x + 1;
    tailSnake.y = headSnake.y;
    tailSnake.direction = leftDirection;
    snake.push_back(tailSnake);
}

void AddSnakeToField(vector<vector<char>>& field, vector<Snake>& snake)
{
    field[snake[0].y][snake[0].x] = headSnakeChar;
    for (int i = 1; i < snake.size(); i++)
    {
        field[snake[i].y][snake[i].x] = charSnake;
    }
}

void ClearSnakeOnField(vector<vector<char>>& field, vector<Snake>& snake)
{
    for (int i = 0; i < snake.size(); i++)
    {
        field[snake[i].y][snake[i].x] = charFillField;
    }
}

void GameOver(vector<vector<char>>& field)
{
    field[1][2] = 'G';
    field[1][3] = 'a';
    field[1][4] = 'm';
    field[1][5] = 'e';

    field[2][2] = 'O';
    field[2][3] = 'v';
    field[2][4] = 'e';
    field[2][5] = 'r';
    PrintField(field);
    exit(0);
}

bool SnakeEatApple(Snake headSnake, Apple apple)
{
    return headSnake.x == apple.x && headSnake.y == apple.y;
}

void IncreaseSnake(vector<Snake>& snake)
{
    Snake headSnake = snake[0];
    Snake tailSnake = snake[snake.size() - 1];
    Snake newTailSnake;
    newTailSnake = tailSnake;
    if (tailSnake.direction == upDirection)
    {
        newTailSnake.y = tailSnake.y + 1;
    }
    if (tailSnake.direction == leftDirection)
    {
        newTailSnake.x = tailSnake.x + 1;
    }
    if (tailSnake.direction == downDirection)
    {
        newTailSnake.y = tailSnake.y - 1;
    }
    if (tailSnake.direction == rigthDirection)
    {
        newTailSnake.x = tailSnake.x - 1;
    }
    snake.push_back(newTailSnake);
}

bool IsFlankWall(int x)
{
    return (x == -1 || x == 8);
}

bool IsСoverageWall(int y)
{
    return (y == -1 || y == 4);
}

bool IsWall(char direction, Snake headSnake)
{
    if (direction == upDirection)
    {
        if (IsСoverageWall(headSnake.y - 1))
        {
            return true;
        }
    }
    if (direction == downDirection)
    {
        if (IsСoverageWall(headSnake.y + 1))
        {
            return true;
        }
    }
    if (direction == leftDirection)
    {
        if (IsFlankWall(headSnake.x - 1))
        {
            return true;
        }
    }
    if (direction == rigthDirection)
    {
        if (IsFlankWall(headSnake.x + 1))
        {
            return true;
        }
    }
    return false;
}

void MoveSnake(vector<Snake>& snake, char newDirection, Apple& apple)
{
    snake.pop_back();

    Snake newHeadSnake = snake[0];
    if (newDirection == upDirection)
    {
        newHeadSnake.y = --newHeadSnake.y;
    }
    if (newDirection == leftDirection)
    {
        newHeadSnake.x = --newHeadSnake.x;
    }
    if (newDirection == downDirection)
    {
        newHeadSnake.y = ++newHeadSnake.y;
    }
    if (newDirection == rigthDirection)
    {
        newHeadSnake.x = ++newHeadSnake.x;
    }
    newHeadSnake.direction = newDirection;
    snake.insert(snake.begin(), newHeadSnake);
}

bool CrashingIntoYouself(vector<Snake>& snake)
{
    Snake headSnake = snake[0];
    for (int i = 1; i < snake.size(); i++)
    {
        Snake snakePart = snake[i];
        if (snakePart.x == headSnake.x && snakePart.y == headSnake.y)
        {
            return true;
        }
    }
    return false;
}

bool IsWin(vector<Snake>& snake)
{
    return snake.size() == countCell;
}

void Win(vector<vector<char>>& field)
{
    field[1][2] = 'W';
    field[1][3] = 'i';
    field[1][4] = 'n';
    field[1][5] = '!';

    field[2][1] = 'W';
    field[2][2] = 'i';
    field[2][3] = 'n';
    field[2][4] = '!';
    field[2][5] = '!';
    field[2][6] = '!';
    PrintField(field);
    exit(0);
}

bool IsOpositeDirection(Snake headSnake, char direction)
{
    if (headSnake.direction == leftDirection && direction == rigthDirection)
    {
        return true;
    }
    if (headSnake.direction == rigthDirection && direction == leftDirection)
    {
        return true;
    }
    if (headSnake.direction == upDirection && direction == downDirection)
    {
        return true;
    }
    if (headSnake.direction == downDirection && direction == upDirection)
    {
        return true;
    }
    return false;
}

char PreviousDirection(char direction)
{
    if (direction == leftDirection)
    {
        return rigthDirection;
    }
    if (direction == rigthDirection)
    {
        return leftDirection;
    }
    if (direction == upDirection)
    {
        return downDirection;
    }
    if (direction == downDirection)
    {
        return upDirection;
    }
}

int main()
{
    vector<vector<char>> field(countRow);
    FillField(field);

    vector<Snake> snake;
    CreateSnake(field, snake);
    AddSnakeToField(field, snake);

    Apple apple;
    CreateApple(apple, snake);
    AddAppleToField(field, apple);
    
    PrintField(field);

    char direction = leftDirection;
    while (1)
    {
        Sleep(speadSnake);
        if (_kbhit())
        {
            direction = _getch();
        }
        if (IsOpositeDirection(snake[0], direction))
        {
            direction = PreviousDirection(direction);
        }
        //printf("\033[2J\x1B[H"); через такой способ консоль очищается быстрее, но мерцаний больше
        system("cls"); // лучше использовать этот
        ClearSnakeOnField(field, snake);
        if (IsWall(direction, snake[0]))
        {
            GameOver(field);
        }
        MoveSnake(snake, direction, apple);
        if (SnakeEatApple(snake[0], apple))
        {
            IncreaseSnake(snake);
            CreateApple(apple, snake);
            AddAppleToField(field, apple);
        }
        if (IsWin(snake))
        {
            Win(field);
        }
        if (CrashingIntoYouself(snake))
        {
            GameOver(field);
        }
        AddSnakeToField(field, snake);
        PrintField(field);
    }

    return 0;
}
