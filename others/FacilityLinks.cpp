#include <iostream>
#include <string>
#include <fstream>
#include <set>


using namespace std;

int main() {

	int source, sink, linkID;
	bool found;
	

	ifstream facilities("Facilities.txt");
	ifstream fullList("listaRJFull.txt");

	ofstream facilityLinks;
	facilityLinks.open("ResultLinks.txt", ios_base::trunc);

	int fullMatrix[24414][3];						// valor fixo pois este valor vem da quantidade de links no mapa
	int i;

	for (i = 0; i < 24414; i++) {					// preenche uma matriz com todos os links existentes, e os nós de origem e destino
		fullList >> fullMatrix[i][0];
		fullList >> fullMatrix[i][1];
		fullList >> fullMatrix[i][2];

		fullList.ignore(10000, '\n');
	}

	fullList.close();

	while (facilities >> source) {
		facilities >> sink;
		found = 0;

		for (i = 0; i < 24414 && !found; i++) {
			if (source == fullMatrix[i][1] && sink == fullMatrix[i][2]) {	// se encontrar na lista completa a mesma origem e destino
				if (fullMatrix[i][0] > 1000000) {
					facilityLinks << fullMatrix[i-1][0] << "\n";
					found = 1;
				}
				else {
					facilityLinks << fullMatrix[i][0] << "\n";					// escrever o ID do link no arquivo de saída
					found = 1;
				}
			}
		}
	}

	facilities.close();
	facilityLinks.close();

	set <int>FinalList;
	set <int>::iterator it;

	ifstream FacLinks("ResultLinks.txt");

	while (FacLinks >> linkID) {
		FinalList.insert(linkID);
	}

	FacLinks.close();

	ofstream FinalLinks("ResultLinks.txt", ios_base::trunc);

	for (it = FinalList.begin(); it != FinalList.end(); it++) {
		FinalLinks << *it << "\n";
	}

	FinalLinks.close();

	system("pause");

	return 0;
}