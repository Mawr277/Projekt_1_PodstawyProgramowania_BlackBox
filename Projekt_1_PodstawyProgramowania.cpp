/* 
|############################################|
|Maciej Nowicki 197643 ACiR --> Gra Black Box|
|############################################|

* Kody ASCII:
* 32 - whitespace
* 65 - A
* 72 - H
* 79 - O
* 82 - R
* 111 - o
*/

#include <iostream>
#include <cstdio> //getchar
#include <cstdlib> //srand rand
#include <ctime> //time(NULL)

using namespace std;

struct position {
	int x;
	int y;
};

// mechaniki gry
void clear_console();
void print_logo();
void print_board(int board_size, int particles_number, int board[][23], position player);
void plant_particles(int particles_number, int board_size, int iteration, position particles[8]);
void shoot(int board[][23], int board_size, position player, position particles[8], int particles_number, int &shoot_number, bool toggle_help);
void save_moves(position player, position record[5]);
void undo(int board[][23], position record[5], int& target_number, int& shoot_number, int board_size, position player, position particles[8], int particles_number, bool toggle_help);

// menu
void menu();
void move(int board_size, position particles[8], int particles_number);


int main()
{
	srand(time(NULL));
	menu();

	return 0;
}

// mechaniki gry

void clear_console() {
#ifdef _WIN32
	system("CLS");
#else
	system("clear");
#endif
}

void print_logo() {
	clear_console();

	cout << " ________   ___        ________   ________   ___  __            ________   ________      ___    ___ " << endl;
	cout << "|\\   __  \\ |\\  \\      |\\   __  \\ |\\   ____\\ |\\  \\|\\  \\         |\\   __  \\ |\\   __  \\    |\\  \\  /  /|" << endl;
	cout << "\\ \\  \\|\\ /_\\ \\  \\     \\ \\  \\|\\  \\\\ \\  \\___| \\ \\  \\/  /|_       \\ \\  \\|\\ /_\\ \\  \\|\\  \\   \\ \\  \\/  / /" << endl;
	cout << " \\ \\   __  \\\\ \\  \\     \\ \\   __  \\\\ \\  \\     \\ \\   ___  \\       \\ \\   __  \\\\ \\  \\\\\\  \\   \\ \\    / / " << endl;
	cout << "  \\ \\  \\|\\  \\\\ \\  \\____ \\ \\  \\ \\  \\\\ \\  \\____ \\ \\  \\\\ \\  \\       \\ \\  \\|\\  \\\\ \\  \\\\\\  \\   /     \\/  " << endl;
	cout << "   \\ \\_______\\\\ \\_______\\\\ \\__\\ \\__\\\\ \\_______\\\\ \\__\\\\ \\__\\       \\ \\_______\\\\ \\_______\\ /  /\\   \\  " << endl;
	cout << "    \\|_______| \\|_______| \\|__|\\|__| \\|_______| \\|__| \\|__|        \\|_______| \\|_______|/__/ /\\ __\\ " << endl;
	cout << "                                                                                        |__|/ \\|__| " << "\n\n";
	cout << "Autor projektu: Maciej Nowicki 197643 ACiR" << endl;
	cout << "Autor gry : Eric Solomon" << endl;
	cout << "Zasady: https://en.wikipedia.org/wiki/Black_Box_(game)" << endl;
	cout << "Zaleca sie gre w pelnym ekranie!" << "\n\n";
}

void print_board(int board_size, int particles_number, int board[][23], position player) {
	print_logo();
	cout << "Na planszy znajduja sie: " << particles_number << " atomy/ow." << "\n\n";
	for (int row = 0; row < 23; row++) {
		if (row == 2 * board_size + 1) {
			row = 21;
		}
		for (int col = 0; col < 23; col++) {
			if (col == 2 * board_size + 1) {
				col = 21;
			}
			if (row == player.y && col == player.x) {
				cout << char(80);
			}
			else if (board[row][col] <= 20) {
				cout << board[row][col];
			}
			else {
				cout << char(board[row][col]);
			}
		}
		cout << endl;
	}
}

void plant_particles(int particles_number, int board_size, int iteration, position particles[8]) {
	for (int i = iteration; i < particles_number; i++) {
		particles[i].y = (1 + (rand() % board_size)) * 2;
		particles[i].x = (1 + (rand() % board_size)) * 2;
		for (int r = i - 1; r >= 0; r--) {
			if ((particles[i].y == particles[r].y) && (particles[i].x == particles[r].x)) {
				plant_particles(particles_number, board_size, i, particles);
			}
		}
	}
}

void shoot(int board[][23], int board_size, position player, position particles[8], int particles_number, int &shoot_number, bool toggle_help) {
	position laser = {
		player.x,
		player.y
	};
	
	// wspianie atomów do planszy
	for (int i = 0; i < particles_number; i++) {
		if (board[particles[i].y][particles[i].x] == 111) {
			board[particles[i].y][particles[i].x] = 79;
		}
		else {
			board[particles[i].y][particles[i].x] = 65;
		}
	}

	int direction_x = 0; // modyfikatory kierunku ruchu lasera
	int direction_y = 0;

	if (laser.y == 0) {
		laser.y += 2;
		direction_y = 1;
	}
	else if (laser.x == 0) {
		laser.x += 2;
		direction_x = 1;
	}
	else if (laser.y == 22) {
		laser.y = 2 * board_size;
		direction_y = -1;
	}
	else if (laser.x == 22) {
		laser.x = 2 * board_size;
		direction_x = -1;
	}


	if (board[laser.y][laser.x] == 65 || board[laser.y][laser.x] == 79) {
		board[player.y][player.x] = 72;
		direction_x = 0;
		direction_y = 0;
	}

	// sprawdza czy nie ma atomu przy wyjściu wystrzelonej wiązki lasra
	if (direction_x != 0 && (board[laser.y + 2][laser.x] == 65 || board[laser.y + 2][laser.x] == 79 || board[laser.y - 2][laser.x] == 65 || board[laser.y - 2][laser.x] == 79)) {
		board[player.y][player.x] = 82;
		direction_x = 0;
		direction_y = 0;
	}

	if (direction_y != 0 && (board[laser.y][laser.x - 2] == 65 || board[laser.y][laser.x - 2] == 79 || board[laser.y][laser.x + 2] == 65 || board[laser.y][laser.x + 2] == 79)) {
		board[player.y][player.x] = 82;
		direction_x = 0;
		direction_y = 0;
	}


	// ruch lasera
	while (laser.x != 22 && laser.x != 0 && laser.y != 0 && laser.y != 22 && (direction_y != 0 || direction_x != 0)) {

		if (board[laser.y + 2 * direction_y][laser.x + 2 * direction_x] == 65 || board[laser.y + 2 * direction_y][laser.x + 2 * direction_x] == 79) {
			board[player.y][player.x] = 72;
			break;
		}

		if (board[laser.y - 2][laser.x + 2] == 65 || board[laser.y - 2][laser.x + 2] == 79) {
			if (direction_x == 1) {
				direction_y = 1;
				direction_x = 0;
			}
			else { // direction_y == -1 inna sytuacja nie zajdzie
				direction_y = 0;
				direction_x = -1;
			}
		}
		if (board[laser.y - 2][laser.x - 2] == 65 || board[laser.y - 2][laser.x - 2] == 79) {
			if (direction_x == -1) {
				direction_y = 1;
				direction_x = 0;
			}
			else { // direction_y == -1 inna sytuacja nie zajdzie
				direction_y = 0;
				direction_x = 1;
			}
		}
		if (board[laser.y + 2][laser.x - 2] == 65 || board[laser.y + 2][laser.x - 2] == 79) {
			if (direction_x == -1) {
				direction_y = -1;
				direction_x = 0;
			}
			else { // direction_y == 1 inna sytuacja nie zajdzie
				direction_y = 0;
				direction_x = 1;
			}
		}
		if (board[laser.y + 2][laser.x + 2] == 65 || board[laser.y + 2][laser.x + 2] == 79) {
			if (direction_x == 1) {
				direction_y = -1;
				direction_x = 0;
			}
			else { // direction_y == 1 inna sytuacja nie zajdzie
				direction_y = 0;
				direction_x = -1;
			}
		}

		if (direction_x == 1 && laser.x == board_size * 2) {
			laser.x = 22;
		}
		else if (direction_y == 1 && laser.y == board_size * 2) {
			laser.y = 22;
		}
		else { // poruszenie promienia
			if (direction_x != 0) {
				laser.x += 2 * direction_x;
			}
			else if (direction_y != 0) {
				laser.y += 2 * direction_y;
			}
		}
	}

	if ((laser.x == 22) || (laser.x == 0) || (laser.y == 0) || (laser.y == 22)) {
		if (laser.y == player.y && laser.x == player.x) {
			board[player.y][player.x] = 82;
		}
		else {
			board[player.y][player.x] = shoot_number;	
			board[laser.y][laser.x] = shoot_number;
			shoot_number += 1;
		}
	}

	// usunięcie atomów z planszy
	if (toggle_help == false) {
		for (int i = 0; i < particles_number; i++) {
			if (board[particles[i].y][particles[i].x] == 65) {
				board[particles[i].y][particles[i].x] = 32;
			}
			else {
				board[particles[i].y][particles[i].x] = 111;
			}
		}
	}
}

void save_moves(position player, position record[5]) {
	for (int i = 4; i > 0; i--) {
		record[i].x = record[i - 1].x;
		record[i].y = record[i - 1].y;
	}
	record[0].x = player.x;
	record[0].y = player.y;
}

void undo(int board[][23], position record[5], int &target_number, int &shoot_number, int board_size, position player, position particles[8], int particles_number, bool toggle_help){
	if (board[record[0].y][record[0].x] == 111) {
		board[record[0].y][record[0].x] = 32;

		target_number -= 1;
	}
	else if (board[record[0].y][record[0].x] == 32) {
		if ((record[0].x == 22) || (record[0].x == 0) || (record[0].y == 0) || (record[0].y == 22)) {
			shoot(board, board_size, record[0], particles, particles_number, shoot_number, toggle_help);
		}
		else {
			board[record[0].y][record[0].x] = 111;

			target_number += 1;
		}
	}
	else if (board[record[0].y][record[0].x] == 72 || board[record[0].y][record[0].x] == 82) {// cofnięcie strzału, którego efektem jest H lub R
		board[record[0].y][record[0].x] = 32;
	}
	else if (board[record[0].y][record[0].x] < 20) { // cofnięcie strzału, którego efektem jest liczba
		for (int i = 0; i < 22; i += 2) {
			if (board[0][i] == shoot_number - 1) {
				board[0][i] = 32;
			}
			if (board[i][0] == shoot_number - 1) {
				board[i][0] = 32;
			}
			if (board[i][22] == shoot_number - 1){
				board[i][22] = 32;
			}
			if (board[22][i] == shoot_number - 1) {
				board[22][i] = 32;
			}
		}
		shoot_number -= 1;
	}
}

//menu

void menu() {
	char input; 
	position particles[8];

	print_logo();

	cout << "1. Rozpocznij gre" << endl;
	cout << "2. Sterowanie / Zasady" << endl;
	cout << "3. Wyjdz z gry" << endl;

	cin >> input;
	if (input == '1') {
		print_logo();

		cout << "Wybierz poziom trudnosci rozgrywki: " << endl;
		cout << "1.Poczatkujacy - 5x5 " << endl;
		cout << "2.Sredni - 8x8" << endl;
		cout << "3.Specjalista - 10x10 " << endl;
		cout << "Q - Powrot do menu" << endl;

		cin >> input;
		if (input == '1') { // 5x5
			plant_particles(3, 5, 0, particles);
			move(5, particles, 3);
		}
		else if (input == '2') { // 8x8
			plant_particles(5, 8, 0, particles);
			move(8, particles, 5);
		}
		else if (input == '3') { // 10x10
			plant_particles(8, 10, 0, particles);
			move(10, particles, 8);
		}
		else {
			menu();
		}
	}
	else if (input == '2') { // opis sterowania
		print_logo();

		cout << "Sterowanie: (wg instrukcji projektu)" << endl;
		cout << "1. Poruszanie: " << endl;
		cout << "W - gora, S - dol, A - lewo, P - prawo" << "\n\n";

		cout << "2. Rozgrywka: " << endl;
		cout << "q, Q - wyjscie do menu glownego" << endl;
		cout << "u, U - undo - cofnij ruch" << endl;
		cout << "r, R - redo - powtorz cofniety ruch" << endl;
		cout << "spacja - oddanie strzalu z dowolnej sciany" << endl;
		cout << "o - zaznaczenie przypuszczalnego polozenia atomu" << endl;
		cout << "k - koniec gry - wyswietla rozwiazanie i liczbe trafien gdy obstawiono wszystkie proponowane miejsca atomow" << endl;
		cout << "p - wyswietla rozwiozania i konczy rozgrywke - bez punktacji" << endl;
		cout << "H - Help/Pomoc - wlacz/wylacz pokazanie polozenie atomow\ngdy jest uruchomione nie wolno zaznaczac przypuszczalnego polozenia atomu" << "\n\n";

		cout << "3. Oznaczenia na planszy: " << endl;
		cout << "A - pozycja atomu" << endl;
		cout << "O - trafienie pozycji atomu" << endl;
		cout << "X - bledne zalozenie pozycji atomu" << endl;
		cout << "H - promien lasera trafil atom" << endl;
		cout << "R - promien lasera odbil sie i wrocil do miejsca strzalu" << endl;
		cout << "Liczba - wlot i wylot promienia lasera" << "\n\n";

		cout << "Nacisnj dowolny przycisk, aby wrocic do menu";
		cin >> input;
		menu();
	}
	else if (input == '3') { // exit
		exit(0);
	}
	else {
		menu();
	}
}

void move(int board_size, position particles[8], int particles_number) {
	char input;
	int shoot_number = 1;
	int target_number = 0;
	bool toggle_help = false;
	position record[5]; // undo
	record[0].x = 0; 
	record[0].y = 0;
	position undid_record[5]; // redo
	undid_record[0].x = 0;
	undid_record[0].y = 0;
	bool can_redo = false;
	int board[23][23] =
	{
		{32, 186, 32, 179, 32, 179, 32, 179, 32, 179, 32, 179, 32, 179, 32, 179, 32, 179, 32, 179, 32, 186, 32},//0
		{205, 201, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 187, 205},//1
		{32, 186, 32, 179, 32, 179, 32, 179, 32, 179, 32, 179, 32, 179, 32, 179, 32, 179, 32, 179, 32, 186, 32},//2
		{196, 186, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 186, 196},//3
		{32, 186, 32, 179, 32, 179, 32, 179, 32, 179, 32, 179, 32, 179, 32, 179, 32, 179, 32, 179, 32, 186, 32},//4
		{196, 186, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 186, 196},//5
		{32, 186, 32, 179, 32, 179, 32, 179, 32, 179, 32, 179, 32, 179, 32, 179, 32, 179, 32, 179, 32, 186, 32},//6
		{196, 186, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 186, 196},//7
		{32, 186, 32, 179, 32, 179, 32, 179, 32, 179, 32, 179, 32, 179, 32, 179, 32, 179, 32, 179, 32, 186, 32},//8
		{196, 186, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 186, 196},//9
		{32, 186, 32, 179, 32, 179, 32, 179, 32, 179, 32, 179, 32, 179, 32, 179, 32, 179, 32, 179, 32, 186, 32},//10
		{196, 186, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 186, 196},//11
		{32, 186, 32, 179, 32, 179, 32, 179, 32, 179, 32, 179, 32, 179, 32, 179, 32, 179, 32, 179, 32, 186, 32},//12
		{196, 186, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 186, 196},//13
		{32, 186, 32, 179, 32, 179, 32, 179, 32, 179, 32, 179, 32, 179, 32, 179, 32, 179, 32, 179, 32, 186, 32},//14
		{196, 186, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 186, 196},//15
		{32, 186, 32, 179, 32, 179, 32, 179, 32, 179, 32, 179, 32, 179, 32, 179, 32, 179, 32, 179, 32, 186, 32},//16
		{196, 186, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 186, 196},//17
		{32, 186, 32, 179, 32, 179, 32, 179, 32, 179, 32, 179, 32, 179, 32, 179, 32, 179, 32, 179, 32, 186, 32},//18
		{196, 186, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 186, 196},//19
		{32, 186, 32, 179, 32, 179, 32, 179, 32, 179, 32, 179, 32, 179, 32, 179, 32, 179, 32, 179, 32, 186, 32},//20
		{205, 200, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 188, 205},//21
		{32, 186, 32, 179, 32, 179, 32, 179, 32, 179, 32, 179, 32, 179, 32, 179, 32, 179, 32, 179, 32, 186, 32}//22
	};

	position player;
	player.x = 2;
	player.y = 0;

	while (1) {
		input = getchar();
		if (input == 'w' || input == 'W') {
			if (player.y == 0) {
				continue;
			}
			else if (player.y == 22) {
				player.y = board_size * 2;
			}
			else {
				player.y -= 2;
			}
		}
		else if (input == 's' || input == 'S') {
			if (player.y == 22) {
				continue;
			}
			else if (player.y == board_size * 2) {
				player.y = 22;
			}
			else {
				player.y += 2;
			}
		}
		else if (input == 'a' || input == 'A') {
			if (player.x == 0) {
				continue;
			}
			else if (player.x == 22) {
				player.x = board_size * 2;
			}
			else {
				player.x -= 2;
			}
		}
		else if (input == 'd' || input == 'D') {
			if (player.x == 22) {
				continue;
			}
			else if (player.x == board_size * 2) {
				player.x = 22;
			}
			else {
				player.x += 2;
			}
		}
		else if (input == 'q' || input == 'Q') {
			menu();
		}
		else if ((input == 'h' || input == 'H') && toggle_help == true) {
			for (int i = 0; i < particles_number; i++) {
				if (board[particles[i].y][particles[i].x] == 65) {
					board[particles[i].y][particles[i].x] = 32;
				}
				else {
					board[particles[i].y][particles[i].x] = 111;
				}
			}
			toggle_help = false;
		}
		else if ((input == 'h' || input == 'H') && toggle_help == false) {
			for (int i = 0; i < particles_number; i++) {
				if (board[particles[i].y][particles[i].x] == 111) {
					board[particles[i].y][particles[i].x] = 79;
				}
				else {
					board[particles[i].y][particles[i].x] = 65;
				}
			}
			toggle_help = true;
		}
		else if (input == ' ') {
			if ((((player.x == 0 || player.x == 22) && (player.y != 0 && player.y != 22)) || ((player.y == 0 || player.y == 22) && (player.x != 0 && player.x != 22))) && board[player.y][player.x] == 32) {
				shoot(board, board_size, player, particles, particles_number, shoot_number, toggle_help);
				save_moves(player, record);
				can_redo = false;
			}
			else {
				continue;
			}
		}
		else if (input == 'o' && player.x != 0 && player.x != 22 && player.y != 0 && player.y != 22) {
			if (board[player.y][player.x] == 32 && target_number < particles_number) {
				board[player.y][player.x] = 111;
				target_number += 1;
				save_moves(player, record);
			}
			else if (board[player.y][player.x] == 111) {
				board[player.y][player.x] = 32;
				target_number -= 1;
				save_moves(player, record);
			}

			can_redo = false;
		}
		else if ((input == 'u' || input == 'U') && (record[0].y != 0 || record[0].x != 0)) {
			undo(board, record, target_number, shoot_number , board_size, player, particles, particles_number, toggle_help);
			save_moves(record[0], undid_record);

			for (int i = 0; i < 4; i++) {
				record[i].x = record[i + 1].x;
				record[i].y = record[i + 1].y;
			}
			record[4].x = 0;
			record[4].y = 0;
			can_redo = true;
		}
		else if ((input == 'r' || input == 'R') && can_redo == true && (undid_record[0].y != 0 || undid_record[0].x != 0)) {
			save_moves(undid_record[0], record);
			undo(board, undid_record, target_number, shoot_number, board_size, player, particles, particles_number, toggle_help);

			for (int i = 0; i < 4; i++) {
				undid_record[i].x = undid_record[i + 1].x;
				undid_record[i].y = undid_record[i + 1].y;
			}
			if (undid_record[0].y == 0 && undid_record[0].x == 0) {
				can_redo = false;
			}
			undid_record[4].x = 0;
			undid_record[4].y = 0;
		}
		else if (input == 'p') {
			for (int i = 0; i < particles_number; i++) {
				board[particles[i].y][particles[i].x] = 65;
			}
			print_board(board_size, particles_number, board, player);
			cout << "Podaj dowolny znak, aby kontynuowac." << endl;
			cin >> input;
			menu();
		}
		else if (input == 'k' && target_number == particles_number) {
			int hit = 0;
			player.x = 0;
			player.y = 0;
			for (int row = 2; row <= 2 * board_size; row += 2) {
				for (int col = 2; col <= 2 * board_size; col += 2) {
					if (board[row][col] == 111) {
						for (int i = 0; i < particles_number; i++) {
							if (row == particles[i].y && col == particles[i].x) {
								board[row][col] = 79;
								hit += 1;
								break;
							}
							else {
								board[row][col] = 88;
							}
						}
					}
				}
			}

			for (int i = 0; i < particles_number; i++) {
				if (board[particles[i].y][particles[i].x] != 79) {
					board[particles[i].y][particles[i].x] = 65;
				}
			}
			print_board(board_size, particles_number, board, player);
			cout << endl << "Trafiles/las " << hit << " na " << particles_number << " atomy/ow" << endl;
			cout << "Podaj dowolny znak, aby kontynuowac." << endl;
			cin >> input;
			menu();
		}

		if (can_redo == false) {
			undid_record[0].y = 0;
			undid_record[0].x = 0;
		}

		print_board(board_size, particles_number, board, player);
	}
}
