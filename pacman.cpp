#include <iostream>
#include <stdio.h>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <time.h>
#include <unistd.h>

using namespace std;

const int SIZE = 15;            //Dimensione del labirinto (maze); non comprende i muri perimetrali
const int TURNI_FRUTTA = 7;     // Numero turni per cui i frutti devono rimanere
const int MAX_FRUTTI = 5;       // Numero massimo frutti sul terreno di gioco
const int POINT_DOT = 10;       // Punti guadagnati mangiando un puntino
const int POINT_FRUIT = 300;    // Punti guadagnati mangiando un puntino

const char EMPTY_C = ' ';       // spazio vuoto
const char WALL_C = '#';        // muro
const char DOT_C = '.';         // puntino
const char FRUIT_C = 'o';        // frutto

const string MAZE_CONFIG_NAME = "maze.cfg";//nome del file con i dati iniziali del labirinto

enum PacmanDir { su, giu, sinistra, destra };
enum Command { Go_N, Go_S, Go_W, Go_E, Quit, Unknown };
enum Labirinth_Elems { wall, empty, dot, fruit};

struct PacMan{
    PacmanDir direction;
    int x;
    int y;
};

struct Fruits {
	int x;
	int y;
	bool visible;
	int rounds_left;
};

Labirinth_Elems char2lab_elem(char);
char lab_elem2char(Labirinth_Elems);
void set_pacman(PacMan &, int, int, PacmanDir);
void init(string, Labirinth_Elems[SIZE][SIZE], PacMan &, int &, Fruits *);
void init_maze(string, Labirinth_Elems [SIZE][SIZE]);
void display(Labirinth_Elems[SIZE][SIZE], PacMan, int &, Fruits *);
char PacmanDir2char(PacmanDir);
Command get_command();
Command char2command(const char);
PacmanDir cmd2dir(Command);
bool make_move(PacMan &, Command, Labirinth_Elems [SIZE][SIZE], int &, Fruits *);
bool acceptable(int, int, Labirinth_Elems [SIZE][SIZE]);
void rand_coord(int &, int &, Labirinth_Elems [SIZE][SIZE]);
//void game_end();

int main(){

	Labirinth_Elems maze[SIZE][SIZE];
	PacMan p;
	cout << "PAC MAN BASIC" << endl;
	int points;
	Fruits a_frutti[MAX_FRUTTI];
	try {
		init(MAZE_CONFIG_NAME, maze, p, points, a_frutti);
	}
	catch (string& e) {
		cout << e;
		return -1;
	}
	char my_char;
	Command my_cmd;

	for (;;){
		display(maze, p, points, a_frutti);
		my_cmd = get_command();
		switch (my_cmd) {
		    case Quit:
			cout << "\n Grazie di aver giocato con me, alla prossima" << endl;
			return (0);
		    case Unknown:
			cout << "Unexpected Command" << endl;
			break;
		    case Go_S:
		    case Go_E:
		    case Go_W:
		    case Go_N:
		    system("clear");
			if (!make_move(p, my_cmd, maze, points, a_frutti)) cout << "Mossa impossibile: non puoi passare attraverso i muri" << endl;
			break;
		    default:
			cout << "Not a command" << endl;
		}

	}
}

void set_pacman(PacMan & pac_man, int x, int y, PacmanDir d){
	pac_man.x = x;
	pac_man.y = y;
	pac_man.direction = d;
}

Labirinth_Elems char2lab_elem(char c) {
    switch (c) {
    case WALL_C:
        return Labirinth_Elems::wall;
    case EMPTY_C:
        return Labirinth_Elems::empty;
    case DOT_C:
        return Labirinth_Elems::dot;
	case FRUIT_C:
        return Labirinth_Elems::fruit;
    default:return Labirinth_Elems::empty;
    }
}

void init_maze(string config_file_name, Labirinth_Elems mm[SIZE][SIZE]) {
    ifstream cfgfile;
    cfgfile.open(config_file_name);
    if (!cfgfile)
        throw "file not found: missing " + config_file_name;

    for (int y = 0; y<SIZE; ++y) {
        string line;
        if (!getline(cfgfile, line))
            line = "";
        istringstream line_stream(line);
        for (int x = 0; x < SIZE; ++x) {
            char c;
            if (line_stream >> c)
                mm[x][y] = char2lab_elem(c);
            else
                mm[x][y] = Labirinth_Elems::empty;
        }
    }
    cfgfile.close();
}

void init(string config_file_name, Labirinth_Elems M[SIZE][SIZE], PacMan & pac_man, int &points, Fruits *frutti){
	// -- SET RESOLUTION --
	cout << "\e[8;18;29t";
	//---------------------
	init_maze(config_file_name, M);
	srand (time(NULL));
	int x, y;
	rand_coord(x, y, M);
	int X, Y;
	for (int i=0; i<MAX_FRUTTI; i++){
		rand_coord(X, Y, M);
		frutti[i].x = X;
		frutti[i].y = Y;
		frutti[i].visible = rand() % 2 == 0;
		frutti[i].rounds_left = TURNI_FRUTTA;
		if(frutti[i].visible) M[frutti[i].x][frutti[i].y] = Labirinth_Elems::fruit;
	}
	set_pacman(pac_man, x, y, giu);
}

void rand_coord(int &x, int &y, Labirinth_Elems M[SIZE][SIZE]){
	do{
		x = rand() % SIZE;
		y = rand() % SIZE;
	} while (!acceptable(x, y, M));
}

void display(Labirinth_Elems M[SIZE][SIZE], PacMan pac_man, int & points, Fruits * frutti){
	for(int i=0; i<SIZE+2; i++) cout << "\033[1;34m" << WALL_C << "\033[0m";
	cout << endl;
	for(int i=0; i<SIZE; i++){				// NUMERO RIGHE
		cout << "\033[1;34m" << WALL_C << "\033[0m";
		for(int j=0; j<SIZE; j++){			// NUMERO COLONNE
			if(i==pac_man.y && j==pac_man.x)
				cout << "\033[1;33m" << PacmanDir2char(pac_man.direction) << "\033[0m";
			else {
				char d = lab_elem2char(M[j][i]);
				{
					if (d == FRUIT_C) cout << "\033[1;31m" << lab_elem2char(M[j][i]) << "\033[0m";
					else {
						if (d == WALL_C) cout << "\033[1;34m" << lab_elem2char(M[j][i]) << "\033[0m";
						else cout << lab_elem2char(M[j][i]);
					}
				}
			}
		}
		cout << "\033[1;34m" << WALL_C << "\033[0m";
		if (i == 7) cout << "\tSCORE" << endl;
		else{
			if (i == 8){
				cout << "\t" << points << endl;
			} else cout << endl;
		}
	}
	for(int i=0; i<SIZE+2; i++) cout << "\033[1;34m" << WALL_C << "\033[0m";
	cout << endl;
}

char lab_elem2char(Labirinth_Elems e) {
    switch (e) {
    case wall:
        return  WALL_C;
    case Labirinth_Elems::empty:
        return EMPTY_C;
    case dot:
        return DOT_C;
	case fruit:
        return FRUIT_C;
    default: throw "not a valid labirinth element";
    }
}

char PacmanDir2char(PacmanDir d) {
    switch (d) {
    case giu: return '^';
    case destra:return '<';
    case sinistra:return '>';
    case su:return 'v';
    default: throw "not a valid direction";
    }
}

Command get_command(){
	char my_char;
	Command my_cmd;
	do{
		system("stty raw");
		my_char = getchar();
		system("stty cooked");
		my_cmd = char2command(my_char);
	} while(my_cmd == Unknown);
	return my_cmd;
}

Command char2command(const char c) {
	switch (c) {
		case 'w': return Go_N;
		case 'W': return Go_N;
		case 'a': return Go_W;
		case 'A': return Go_W;
		case 'd': return Go_E;
		case 'D': return Go_E;
		case 's': return Go_S;
		case 'S': return Go_S;
		case 'q': return Quit;
		case 'Q': return Quit;
		default: return Unknown;
	}
}

PacmanDir cmd2dir(Command c){
	switch (c) {
	    case Go_N:
		return su;
	    case Go_S:
		return giu;
	    case Go_E:
		return destra;
	    case Go_W:
		return sinistra;
	    default:
		cout << "ERRORE" << endl;
	}
}

bool make_move(PacMan & pac_man, Command c, Labirinth_Elems M[SIZE][SIZE], int &points, Fruits * frutti){
	int x = pac_man.x;
	int y = pac_man.y;
	switch (c) {
	  case Go_N:
		  y--;
		break;

	  case Go_S:
		  y++;
		break;

	  case Go_E:
		  x++;
		break;

	  case Go_W:
		  x--;
		break;

	  case Quit:
		  cout << "ERRORE" << endl;
		break;

	  case Unknown:
		  cout << "ERRORE" << endl;
	}
	if (x < 0 || y < 0 || x >= SIZE || y >= SIZE)
		return false;
	else if (lab_elem2char(M[x][y]) == WALL_C)
		return false;
	cout << endl;
	int count = 0;
	if (lab_elem2char(M[x][y]) == DOT_C)
	points+=POINT_DOT;
	for (int i=0; i<MAX_FRUTTI; i++){
		if (!frutti[i].visible){
			M[x][y] = Labirinth_Elems::empty;
		}
		frutti[i].rounds_left --;
		count ++;

		if (frutti[i].rounds_left == 0) frutti[i].visible = false;
		if (frutti[i].rounds_left < 0 && rand() % 2 == 1 && M[frutti[i].x][frutti[i].y] != Labirinth_Elems::empty){
			M[frutti[i].x][frutti[i].y] = Labirinth_Elems::dot;
			int X, Y;
			rand_coord(X, Y, M);
			frutti[i].x = X;
			frutti[i].y = Y;
			M[X][Y] = Labirinth_Elems::fruit;
			frutti[i].visible = true;
			frutti[i].rounds_left = TURNI_FRUTTA;
		}
		if (!frutti[i].visible && rand()%2 == 1 && count == 4){
			frutti[i].visible = true;
			M[frutti[i].x][frutti[i].y] = Labirinth_Elems::fruit;
		}
		if (x == frutti[i].x && y == frutti[i].y && frutti[i].visible){
			M[x][y] = Labirinth_Elems::empty;
			frutti[i].visible = false;
			points += POINT_FRUIT;
			frutti[i].rounds_left = 1;
		}
	}

	set_pacman(pac_man, x, y, cmd2dir(c));
	return true;
}

/*void game_end(){
	string scelta;
	cout << "HAI PERSO" << endl;
	cout << "Vuoi riprovarci?" << endl;
	cin >> scelta;
	if (scelta == "si") main();
	else cout << "ADDIO" << endl;
}*/

bool acceptable(int x, int y, Labirinth_Elems M[SIZE][SIZE]){
  if (lab_elem2char(M[x][y]) == WALL_C) return false;

  if (x>0 && lab_elem2char(M[x-1][y]) != WALL_C) return true;

  if (y>0 && lab_elem2char(M[x][y-1]) != WALL_C) return true;

  if (x<SIZE-1 && lab_elem2char(M[x+1][y]) != WALL_C) return true;

  if (y<SIZE-1 && lab_elem2char(M[x][y+1]) != WALL_C) return true;

  return false;
}
