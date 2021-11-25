#include <iostream>
#include <string>
#include <fstream>
#include <set>
#include <vector>
#include <array>

using namespace std;

int main() {

	ifstream TCad("Source.txt");				// Exported from Excel spreadsheet with data from the selection
	TCad.ignore(10000, '\n');

	ifstream FullList("listaRJFull.txt");		// All available links and nodes
	ifstream FullList2("listaRJFull.txt");
	
	ofstream Output;
	Output.open("NewList", ios_base::trunc);

	set <int> AllLinks;
	set <int> AllNodes;
	set <int>::iterator it;

	int LinkID1, LinkID2, dir;
	string length;
	bool end = 0;
	
	while (!end) {
		TCad >> LinkID1;
		AllLinks.insert(LinkID1);				// Inserts links from the selection into the set
		//cout << "\n" << LinkID1;
		TCad >> length;
		
		TCad >> dir;
		

		if (dir == 0) {
			LinkID2 = LinkID1 + 1000000;		// Inserts the reverse order of the link if there is one
			AllLinks.insert(LinkID2);
		}

		TCad.ignore(10000, '\n');
		end = TCad.eof();
		
	}

	TCad.close();

	int i = 0, j;
	vector<int> SelectLinks(AllLinks.size());
	for (it = AllLinks.begin(); it != AllLinks.end(); it++) {
		SelectLinks[i] = *it;									// Transfering data from set to vector for easier handling
		i++;
	}

	int cont = 0;
	while (FullList >> LinkID1) {
		cont++;
		FullList.ignore(10000, '\n');
	}

	FullList.close();
	
	vector <vector<int>> LinksAndNodes(cont);
	vector<double> Weights(cont);
	for (i = 0; i < cont; i++) {
		LinksAndNodes[i] = vector<int>(3);
	}
	
	int Edges;
	Edges = cont;

	double OriNode, DestNode, Weight;
	
	cont = 0;
	while (FullList2 >> LinkID1) {
		FullList2 >> OriNode;
		FullList2 >> DestNode;
		FullList2 >> length;
		FullList2 >> Weight;
		FullList2.ignore(10000, '\n');


		LinksAndNodes[cont][0] = LinkID1;
		LinksAndNodes[cont][1] = OriNode;
		LinksAndNodes[cont][2] = DestNode;
		Weights[cont] = Weight;
		cont++;
	}

	int contador = 0;

	for (i = 0; i < Edges; i++) {
		for (j = i + 1; j < Edges; j++) {
			if (LinksAndNodes[i][1] == LinksAndNodes[j][1] && LinksAndNodes[i][2] == LinksAndNodes[j][2]) {
				cout << "\nAresta " << LinksAndNodes[i][1] << " " << LinksAndNodes[i][2] << " repetida";
				contador++;
			}
		}
	}

	if (contador > 0) {
		cout << "\n\n" << contador << " arestas repetidas\n\n";
	}


	FullList2.close();
	
	vector<vector<int>> OD_Matrix(AllLinks.size());
	for (i = 0; i < AllLinks.size(); i++) {
		OD_Matrix[i] = vector<int>(2);
	}
	vector<double> OD_Weights(AllLinks.size());

	

	for (i = 0; i < AllLinks.size(); i++) {
		for (j = 0; j < cont; j++) {
			if (SelectLinks[i] == LinksAndNodes[j][0]) {
				AllNodes.insert(LinksAndNodes[j][1]);
				AllNodes.insert(LinksAndNodes[j][2]);
			}
		}
	}

	Output << AllNodes.size() << "\n";


	
	for (i = 0; i < AllLinks.size(); i++) {
		for (j = 0; j < cont; j++) {
			if (SelectLinks[i] == LinksAndNodes[j][0]) {
				Output << "\n" << LinksAndNodes[j][1] << " " << LinksAndNodes[j][2] << " " << Weights[j];
			}
		}
	}

	

	system("pause");

	return 0;

}