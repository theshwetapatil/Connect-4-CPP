// main.cpp : Defines the entry point for the console application.
// C++ Game: Connect-4
// Gameplay features: pattern-matching, bitboard, multi-player mode
// Author: Shweta Patil
// Copyright: Shweta Patil © 2018
#include <iostream>
#include <bitset>
#include <limits>
#include <vector>

#define MAXROW 7
#define MAXCOL 6

typedef struct grid
{
	private:
		std::bitset<64> bit_board[3];
		std::vector<int> grid_count = std::vector<int>(MAXCOL, ((MAXROW - 1)*MAXCOL));
		std::vector<bool> grid_flag = std::vector<bool>(MAXCOL, false);
		char player = '.';
		bool game_over = false;
		bool play_again = false;

	public:
		grid()
		{
			default_mode();
		}

		void display_board()
		{
			std::cout << std::endl;
			for (int j = 0; j < MAXCOL; j++)
				std::cout << (j + 1) << " ";

			char ch;

			for (int i = 0; i < MAXCOL*MAXROW; i++)
			{
				if (bit_board[0].test(i))
					ch = 'X';
				else
					if (bit_board[1].test(i))
						ch = 'Y';
					else
						ch = '.';

				if ((i) % MAXCOL == 0)
					std::cout << std::endl << ch;
				else
					std::cout << " " << ch;
			}
			std::cout << std::endl;
		}

		bool check_valid(int num)
		{
			return (num > 0 && num <= MAXCOL);
		}

		bool check_valid(char ch)
		{
			return (tolower(ch) ==  'y' || tolower(ch) == 'n');
		}

		void reset_grid()
		{
			bit_board[0] = 0ULL;
			bit_board[1] = 0ULL;
			bit_board[2] = 0ULL;
		}

		void reset_grid_count()
		{
			size_t size = grid_count.size();
			for (int i = 0; i<size; i++)
				grid_count[i] = ((MAXROW - 1)*MAXCOL);
		}

		void reset_grid_flag()
		{
			for (auto flag : grid_flag)
				flag = false;
		}

		void game_reset()
		{
			game_over = false;
			reset_grid();
			reset_grid_count();
			reset_grid_flag();
			player = 'Y';
			system("cls");
		}

		void update_board(char player)
		{
			std::cout << std::endl;

			int col = 1;
			std::cout << "Player::" << player << "::Enter column::";
			//Check for valid column number
			while (!(std::cin >> col) || !check_valid(col) || grid_flag[col - 1])
			{
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

				if (!check_valid(col))
					std::cout << "Error::Please enter column within the range of 1-6..." << std::endl;
				else
					if (grid_flag[col - 1])
						std::cout << "Error::Column " << col << " is full. Please select some other column..." << std::endl;
					else
						std::cout << "Error::Please enter an integer value..." << std::endl;
				std::cout << std::endl;
				std::cout << "Player::" << player << "::Enter column::";
			}

			std::cout << std::endl;
			int row = grid_count[col - 1];
			int digit = col - 1 + row;

			if (player == 'X')
				bit_board[0].set(digit);
			else
				if (player == 'Y')
					bit_board[1].set(digit);
			grid_count[col - 1] -= MAXCOL;
			if (grid_count[col - 1] < 0)
				grid_flag[col - 1] = true;
			bit_board[2] = bit_board[0] | bit_board[1];
		}

		bool check_win(std::bitset<64> &bit_board)
		{
			std::bitset<64> bs = 0ULL;

			bs = bit_board;
			bs = (bs & (bs >> 1));
			bs = (bs & (bs >> 2 * 1));

			if (bs.any())
				return true;

			//Vertical match
			bs = bit_board;
			bs = (bs & (bs << MAXCOL));
			bs = (bs & (bs << 2 * MAXCOL));
			if (bs.any())
				return true;

			//Diagonal match /
			bs = bit_board;
			bs = (bs & (bs << (MAXCOL - 1)));
			bs = (bs & (bs << 2 * (MAXCOL - 1)));
			if (bs.any())
				return true;

			//Diagonal match \	
			bs = bit_board;
			bs = (bs & (bs << (MAXCOL + 1)));
			bs = (bs & (bs << 2 * (MAXCOL + 1)));
			if (bs.any())
				return true;
		}

		void flip_player()
		{
			if (player == 'X')
				player = 'Y';
			else
				player = 'X';
		}

		char get_winner()
		{
			if (check_win(bit_board[0]))
				return 'X';

			if (check_win(bit_board[1]))
				return 'Y';
		}

		bool is_win()
		{
			return (get_winner() == 'X' || get_winner() == 'Y');
		}

		bool is_full()
		{
			int cells = MAXROW*MAXCOL;
			return !(bit_board[2].count() < cells);
		}

		bool is_game_over()
		{
			if (is_win())
				game_over = true;

			if (is_full())
				game_over = true;

			return game_over;
		}

		void display_game_over()
		{
			if (is_full())
				std::cout << "GAME OVER:: Grid is full..." << std::endl;
			else
				std::cout << "Winner::" << get_winner() << std::endl;

			std::cout << "\nThe final board is::" << std::endl;
			display_board();
			std::cout << std::endl;
		}

		void get_response()
		{
			char ch;
			std::cout << "\nWould you like to play again? : \'Y\': Yes \'N\': No :: ";
			while (!(std::cin >> ch) || !check_valid(ch))
			{
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				std::cout << "Error::Please enter Y or N..." << std::endl;
				std::cout << "Would you like to play again? : \'Y\': Yes \'N\': No :: ";
			}

			if (tolower(ch) == 'y')
			{
				play_again = true;
				game_reset();
			}

			if (tolower(ch) == 'n')
				play_again = false;
		}

		void default_mode()
		{
			std::cout << "Welcome to Connect-4..." << std::endl;

			//Loop till player would like to play
			do
			{
				while (!is_game_over())
				{
					flip_player();
					display_board();
					update_board(player);
				}

				display_game_over();
				get_response();

			} while (play_again);

			std::cout << "\nThank you for playing...\n" << std::endl;
		}

}GRID;


int main()
{
	GRID game;
	system("pause");
	return 0;
}