#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

const int SIZE = 8;

// Цвет
class Color
{
public:
	enum class Value
	{
		WHITE,
		BLACK,
		NONE
	};

	Color(Value v = Value::NONE) : value(v) {}

	string toString() const
	{
		switch (value)
		{
		case Value::WHITE: return "White";
		case Value::BLACK: return "Black";
		default: return "None";
		}
	}

	Value getValue() const { return value; }

private:
	Value value;
};

// Фигура
class Figure
{
public:
	Figure(Color c = Color::Value::NONE, bool l = false) : color(c), isLady(l) {}

	Color getColor() const { return color; }
	bool getIslady() const { return isLady; }

private:
	Color color;
	bool isLady;
};

// Ход
struct Move
{
public:
	Move(int fx = 0, int fy = 0, int tx = 0, int ty = 0) : fromX(fx), fromY(fy), toX(tx), toY(ty) {}

	int getFromX() const { return fromX; }
	int getFromY() const { return fromY; }
	int getToX() const { return toX; }
	int getToY() const { return toY; }

private:
	int fromX, fromY;
	int toX, toY;
};

// Доска
class Board
{
public:	
	Board()
	{
		board.resize(SIZE, vector<Figure>(SIZE));
		initialize();
	}

	// Функция отображение доски
	void display()
	{
		cout << "  ";
		for (int i = 0; i < SIZE; ++i)
		{
			cout << i << " ";
		}
		cout << endl;

		for (int i = 0; i < SIZE; ++i)
		{
			cout << i << " ";
			for (int j = 0; j < SIZE; ++j)
			{
				if (board[i][j].getColor().getValue() == Color::Value::BLACK)
				{
					cout << "B ";
				}
				else if (board[i][j].getColor().getValue() == Color::Value::WHITE)
				{
					cout << "W ";
				}
				else
				{
					cout << ". ";
				}
			}
			cout << endl;
		}
	}

	// Функция для проверки введёных координат
	bool movePiece(const Move& move)
	{
		int fromX = move.getFromX(), fromY = move.getFromY();
		int toX = move.getToX(), toY = move.getToY();

		if (fromX < 0 || fromX >= SIZE || fromY < 0 || fromY >= SIZE || toX < 0 || toX >= SIZE || toY < 0 || toY >= SIZE)
		{
			cout << "Координаты за полем!" << endl;
			return false;
		}

		if (board[fromX][fromY].getColor().getValue() == Color::Value::NONE)
		{
			cout << "Фигура не найдена!" << endl;
			return false;
		}

		if (board[toX][toY].getColor().getValue() != Color::Value::NONE)
		{
			cout << "Назначеная клетка не свободна!" << endl;
			return false;
		}

		int dx = toX - fromX;
		int dy = toY - fromY;

		if (abs(dx) == 1 && abs(dy) == 1)
		{
			board[toX][toY] = board[fromX][fromY];
			board[fromX][fromY] = Figure();
			return true;
		}

		if (abs(dx) == 2 && abs(dy) == 2)
		{
			int midX = fromX + dx / 2;
			int midY = fromY + dy / 2;
			if (board[midX][midY].getColor().getValue() != Color::Value::NONE && board[midX][midY].getColor().getValue() != board[fromX][fromY].getColor().getValue())
			{
				board[toX][toY] = board[fromX][fromY];
				board[fromX][fromY] = Figure();
				board[midX][midY] = Figure();
				return true;
			}
		}
		cout << "Недопустимый ход!" << endl;
		return false;
	}

private:
	vector<vector<Figure>> board;

	// Инициализация фигур
	void initialize()
	{
		// Инициализация чёрных фигур
		for (int i = 0; i < SIZE / 2 - 1; ++i)
		{
			for (int j = (i % 2 == 0 ? 1 : 0); j < SIZE; j += 2)
			{
				board[i][j] = Figure(Color::Value::BLACK);
			}
		}
		// Инициализация белых фигур
		for (int i = SIZE - 1; i >= SIZE / 2 + 1; --i)
		{
			for (int j = (i % 2 == 0 ? 1 : 0); j < SIZE; j += 2)
			{
				board[i][j] = Figure(Color::Value::WHITE);
			}
		}
	}
};

// Игрок
class Player
{
public:
	Player(Color c, string n) : color(c), name(n) {}

	Color getColor() const { return color; }
	string getName() const { return name; }

private:
	Color color;
	string name;
};

// Очки
class Score
{
public:
	Score(int w = 0, int b = 0) : whiteScore(w), blackScore(b) {}

private:
	int whiteScore;
	int blackScore;
};

// История матчей
class MatchHistory
{
public:
	void addMove(const Move& move)
	{
		moves.push_back(move);
	}

private:
	vector<Move> moves;
};

// Правила
struct Rules
{
	// Позже будет либо функция для вывода правил или просто txt документ
	// FILE* fp = fopen(RULES, "r");
};

// Функция для передвижения
void playerTurn(Board& board, Player& player, MatchHistory& history)
{
	int fromX, fromY;
	int toX, toY;

	while (true)
	{
		cout << "Очередь игрока: (" << player.getName() << ") " << player.getColor().toString() << endl;
		cout << "Введите координаты (fromX fromY toX toY): ";
		cin >> fromX >> fromY >> toX >> toY;

		Move move(fromX, fromY, toX, toY);
		if (board.movePiece(move))
		{
			history.addMove(move);
			break;
		}
		else
		{
			cout << "Недопустимый ход! Попробуйте снова." << endl;
		}
	}
}

Color chooseColor(const string& playerName)
{
	char colorChoice;
	while (true)
	{
		cout << playerName << ", Выберете цвет (B для Чёрных, W для Белых): ";
		cin >> colorChoice;
		colorChoice = toupper(colorChoice);

		if (colorChoice == 'B')
		{
			return Color(Color::Value::BLACK);
		}
		else if (colorChoice == 'W')
		{
			return Color(Color::Value::WHITE);
		}
		else
		{
			cout << "Неверный цвет! Попробуйте снова." << endl;
		}
	}
}

int main()
{
	setlocale(LC_ALL, "Rus");
	Board board;
	string blackPlayerName, whitePlayerName;

	cout << "Имя первого игрока(на Англ.яз.): ";
	cin >> blackPlayerName;
	Color blackColor = chooseColor(blackPlayerName);
	Player blackPlayer(blackColor, blackPlayerName);

	cout << "Имя второго игрока(на Англ.чз.): ";
	cin >> whitePlayerName;
	Color whiteColor = (blackColor.getValue() == Color::Value::BLACK) ? Color(Color::Value::WHITE) : Color(Color::Value::BLACK);
	Player whitePlayer(whiteColor, whitePlayerName);

	MatchHistory history;
	Score score;
	Rules rules;

	Player* currentPlayer = (blackPlayer.getColor().getValue() == Color::Value::WHITE) ? &blackPlayer : &whitePlayer;

	// Пример работы с одномерным динамическим массивом
	Player* dynamicPlayer = new Player[2]{ blackPlayer, whitePlayer };

	// Пример работы с массивом динамических объектов класса
	Player** dynamicPlayerPointers = new Player * [2];
	dynamicPlayerPointers[0] = new Player(blackPlayer);
	dynamicPlayerPointers[1] = new Player(whitePlayer);

	while (true)
	{
		board.display();
		playerTurn(board, *currentPlayer, history);
		currentPlayer = (currentPlayer == &blackPlayer) ? &whitePlayer : &blackPlayer;
	}

	// Удаление динамических оюъектов
	delete[] dynamicPlayer;
	delete[] dynamicPlayerPointers;
	delete dynamicPlayerPointers[0];
	delete dynamicPlayerPointers[1];

	return 0;
}