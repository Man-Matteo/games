#include <iostream>
#include <time.h>
#include <stdlib.h>
using namespace std;

int main () {

	//scelgo la modalità
	int choice;
	cout << "EVEN NUMBER OPTION: VS PC " << endl;
	cout << "ODD NUMBER OPTION: VS USER" << endl;
	cin >> choice;
    choice = choice % 2;

	int vertical, horizontal;

	cout << " How many vertical squares you want to play with? " << endl;
	cin >> vertical;

	cout << " How many horizontal squares you want to play with? " << endl;
	cin >> horizontal;

	char MYmap [vertical] [horizontal];
	char PCmap [vertical] [horizontal];
	char PCmapCOVERED [vertical] [horizontal];
	char MYmapCOVERED [vertical] [horizontal];

	//genero mappe GIOCATORE 1
	for (int i=0; i<vertical; i++)	{
		for (int j=0; j<horizontal; j++) {
			MYmap [i][j] = '~';
			MYmapCOVERED [i][j] = '~';
		}
	}

	//genero mappe PC/GIOCATORE 2
    for (int i=0; i<vertical; i++) {
			for (int j=0; j<horizontal; j++) {
				PCmap [i][j] = '~';
				PCmapCOVERED [i][j] = '~';
			}
		}

	cout << endl;

	//con quante navi giochiamo
    int numShips = 0;
	cout << " How many ships do you want to play with? " << endl;

	do
		cin >> numShips;
	while (numShips > horizontal*vertical || numShips <= 0);

	cout << " Ok, lets play with " << numShips << " ships! " << endl;

	//posiziono navi del giocatore 1
	cout << " Place your ships " << endl;

	for (int i=0; i<numShips; i++)
	{
		int x,y;
		cout << " Ship n." << i+1 << endl;
		cout << " Coordinated x: " << endl;
		cin >> x;

		if (x > horizontal || x < 1)
		{
			cout << " Error, you must enter an acceptable number (less or equal to " << horizontal << ") " << endl;
			i--;
		}
		else
		{
			cout << " Coordinated y: " << endl;
			cin >> y;
			if (y > vertical || y < 1)
			{
				cout << " Error, you must enter an acceptable number (less or equal to " << vertical << ") " << endl;
				i--;
			}
			else
			{
				if (MYmap [x-1][y-1] == '#')
				{
					cout << " You have already placed a ship here! " << endl;
					i--;
				}
				else
					MYmap [x-1][y-1] = '#';
			}
		}
	}

	int MYShips, PCships;

    MYShips = numShips;
    PCships = numShips;

// Scelgo con lo switch se gioco contro il pc o contro un utente
/*****************************************************/

	switch (choice) {

	case (0):   //CONTRO PC

    	//posiziono navi pc
    	for (int i=0; i<numShips; i++)
    	{

    		int x,y;

    		x = rand() %horizontal;
    		y = rand() %vertical;

    		if (PCmap [x][y] == '#')
    			i--;
    		else
    			PCmap [x][y] = '#';
    	}

    	//si spara!
    	for (;;) {

    		//stampo mappa giocatore
    		cout << "PLAYER MAP" << endl;
    		for (int i=0; i<vertical; i++)
    		{
    			cout << endl;
    			for (int j=0; j<horizontal; j++)
    			    if (MYmap [i][j] == '~')
    				    cout << "\033[1;34m" << MYmap [i][j] << "\033[0m";
    				else
    				    cout << MYmap [i][j];
    		}
    	    cout << endl << endl;

    		//stampo mappa PC (coperta)
    		cout << "PC MAP" << endl;

        	for (int i=0; i<vertical; i++)
        	{
        		cout << endl;
        		for (int j=0; j<horizontal; j++)
        			cout << "\033[1;34m" << PCmapCOVERED [i][j] << "\033[0m";
        	}

    		cout << endl;

    		int MYx, MYy;
    		//MIA GIOCATA
    		for (int i=0; i<1; i++)
    		{
    			cout << " Where do you want to hit?\n Coordinated x: " << endl;
    			cin >> MYx;
    			if (MYx > horizontal || MYx < 1)
    			{
    				cout << " Error, you must enter an acceptable number (less or equal to " << horizontal << ") " << endl;
    				i--;
    			}
    			else
    			{
    				cout << " Coordinated y: " << endl;
    				cin >> MYy;
    				if (MYy > vertical || MYy < 1)
    				{
    					cout << " Error, you must enter an acceptable number (less or equal to " << vertical << ") " << endl;
    					i--;
    				}
    				else
    				{

    					//SE HO COLPITO
    					if (PCmap [MYx-1][MYy-1] == '#')
    					{
    						cout << " YOU'VE HITTEN A SHIP! " << endl;
    						PCmapCOVERED [MYx-1][MYy-1] = 'X';
    						PCships--;
    					}

    					//SE HO MANCATO
    					else if (PCmap [MYx-1][MYy-1] == '~')
    					{
    						cout << " YOU'VE MISSED! " << endl;
    						PCmapCOVERED [MYx-1][MYy-1] = 'o';
    					}
    					else
    						cout << " You've already shot here!" << endl;
    				}
    			}
    		}

        	//controllo se il pc ha 0 navi -> vinco
    		if (PCships == 0)
    		{
    			cout << " YOU WON! " << endl;
    			return 0;
    		}

    		//PC GIOCATA
    		int PCx,PCy;

    		PCx = rand() % horizontal;
    		PCy = rand() % vertical;

    		//SE PC COLPISCE
    		if (MYmap [PCx][PCy] == '#')
    		{
    			cout << " YOU LOST A SHIP! " << endl;
    			MYmap [PCx][PCy] = 'X';
    			MYShips--;
    		}

    		//SE PC MANCA
    		else if (MYmap [PCx][PCy] == '~')
    		{
    			cout << " MISSED! " << endl;
    			MYmap [PCx][PCy] = 'o';
    		}
    		else
    			cout << " I've already shot here! " << endl;

    	    //controllo se ho 0 navi -> perdo
    		if (MYShips == 0)
    		{
    			cout << " YOU LOST! " << endl;
    			return 0;
    		}
	 }
	break;


	case (1):     //CONTRO GIOCATORE

		//posiziono navi del giocatore 2
	    cout << " Place your ships " << endl;

    	for (int i=0; i<numShips; i++)
    	{
    		int x,y;
    		cout << " Ship n." << i+1 << endl;
    		cout << " Coordinated x: " << endl;
    		cin >> x;

    		if (x > horizontal || x < 1)
    		{
    			cout << " Error, you must enter an acceptable number (less or equal to " << horizontal << ") " << endl;
    			i--;
    		}
    		else
    		{
    			cout << " Coordinated y: " << endl;
    			cin >> y;
    			if (y > vertical || y < 1)
    			{
    				cout << " Error, you must enter an acceptable number (less or equal to " << vertical << ") " << endl;
    				i--;
    			}
    			else
    			{
    				if (PCmap [x-1][y-1] == '#')
    				{
    					cout << " You have already placed a ship here! " << endl;
    					i--;
    				}
    				else
    					PCmap [x-1][y-1] = '#';
    			}
    		}
    	}

    	//si spara!
    	for (;;) {


    		//stampo mappa giocatore 1
    		cout << "PLAYER 1 MAP" << endl;
    		for (int i=0; i<vertical; i++)
    		{
    			cout << endl;
    			for (int j=0; j<horizontal; j++)
    				cout << "\033[1;34m" << MYmapCOVERED [i][j] << "\033[0m" ;
    		}

    	    cout << endl << endl;

    		//stampo mappa giocatore 2
    		cout << "PLAYER 2 MAP" << endl;

        	for (int i=0; i<vertical; i++)
        	{
        		cout << endl;
        		for (int j=0; j<horizontal; j++)
        			cout << "\033[1;34m" << PCmapCOVERED [i][j] << "\033[0m";
        	}

    		cout << endl;

    		int MYx, MYy;
    		//GIOCATA PLAYERR 1

    		for (int i=0; i<1; i++)
    		{
    			cout << " Where do you want to hit? (PLAYER 1)\n Coordinated x: " << endl;
    			cin >> MYx;
    			if (MYx > horizontal || MYx < 1)
    			{
    				cout << " Error, you must enter an acceptable number (less or equal to " << horizontal << ") " << endl;
    				i--;
    			}
    			else
    			{
    				cout << " Coordinated y: " << endl;
    				cin >> MYy;
    				if (MYy > vertical || MYy < 1)
    				{
    					cout << " Error, you must enter an acceptable number (less or equal to " << vertical << ") " << endl;
    					i--;
    				}
    				else
    				{

    					//SE HO COLPITO
    					if (PCmap [MYx-1][MYy-1] == '#')
    					{
    						cout << " YOU'VE HITTEN A SHIP! " << endl;
    						PCmapCOVERED [MYx-1][MYy-1] = 'X';
    						PCships--;
    					}

    					//SE HO MANCATO
    					else if (PCmap [MYx-1][MYy-1] == '~')
    					{
    						cout << " YOU'VE MISSED! " << endl;
    						PCmapCOVERED [MYx-1][MYy-1] = 'o';
    					}
    					else
    						cout << " You've already shot here!" << endl;
    				}
    			}
    		}

        	//controllo se il giocatore 2 ha 0 navi -> vinco
    		if (PCships == 0)
    		{
    			cout << " PLAYER 1 WON! " << endl;
    			return 0;
    		}

    		//GIOCATORE 2 GIOCATA

    	    for (int i=0; i<1; i++)
    		{
    			cout << " Where do you want to hit? (PLAYER 2)\n Coordinated x: " << endl;
    			cin >> MYx;
    			if (MYx > horizontal || MYx < 1)
    			{
    				cout << " Error, you must enter an acceptable number (less or equal to " << horizontal << ") " << endl;
    				i--;
    			}
    			else
    			{
    				cout << " Coordinated y: " << endl;
    				cin >> MYy;
    				if (MYy > vertical || MYy < 1)
    				{
    					cout << " Error, you must enter an acceptable number (less or equal to " << vertical << ") " << endl;
    					i--;
    				}
    				else
    				{

    					//SE HO COLPITO
    					if (MYmap [MYx-1][MYy-1] == '#')
    					{
    						cout << " YOU'VE HITTEN A SHIP! " << endl;
    						MYmapCOVERED [MYx-1][MYy-1] = 'X';
    						PCships--;
    					}

    					//SE HO MANCATO
    					else if (MYmap [MYx-1][MYy-1] == '~')
    					{
    						cout << " YOU'VE MISSED! " << endl;
    						MYmapCOVERED [MYx-1][MYy-1] = 'o';
    					}
    					else
    						cout << " You've already shot here!" << endl;
    				}
    			}
    		}
    	    //controllo se giocatore 2 ha 0 navi -> perdo
    		if (MYShips == 0)
    		{
    			cout << " YOU LOST! " << endl;
    			return 0;
    		}

	    }
	    break;

	    default:
		    return 0;
	}
}
