/************************************************************************/
/* $Id: MainP.cpp 65 2010-09-08 06:48:36Z yan.qi.asu $                                                                 */
/************************************************************************/

#include <limits>
#include <set>
#include <map>
#include <queue>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
#include "GraphElements.h"
#include "Graph.h"
#include "YenTopKShortestPathsAlg.h"
#include "WriteOutput.h"
#include <chrono>


using namespace std;

int main()
{
		auto start = chrono::high_resolution_clock::now();

		cout << "Atencao: O Algoritmo considera Grafos Direcionados!" << endl;

		

		ifstream Parameters("InputYen.txt");	// Arquivo de entrada

		string datafile;
		//double Kf;			//Tolerância para o custo máximo
		int Kf;					//Caso com número limitado de caminhos
		int nPares, i, j, AnalysisType;
		int nCaminhos;
		nCaminhos = 0;
		

		
		Parameters >> datafile;
		//getline(Parameters, datafile);
		Parameters.ignore(10000, '\n');

		Parameters >> Kf;
		Parameters.ignore(10000, '\n');
		Parameters >> AnalysisType;				// Analysis type: 1 - todos os pares, 2 - pares selecionados
		Parameters.ignore(10000, '\n');
		

		//double Kf_cento = 1.00 + (Kf / 100);			// Transforma o Kf de inteiro para decimal i.e. 30% -> 1.3		

		Graph my_graph(datafile);

		YenTopKShortestPathsAlg yenAlg(my_graph);
		
		Header(datafile);

		if (AnalysisType == 1) {
			
			Parameters.ignore(10000, '\n');

			set<int> CentroidsSet;
			set<int>::iterator it;

			int cur_node;
			while (Parameters >> cur_node) {
				CentroidsSet.insert(cur_node);
			}

			vector<int> Centroids(CentroidsSet.size());

			int count = CentroidsSet.size() * (CentroidsSet.size() - 1);
			CentroidsCount(count, Kf);
			
			i = 0;
			for (it = CentroidsSet.begin(); it != CentroidsSet.end(); it++) {
				Centroids[i] = *it;
				i++;
			}

			
			CentroidsSet.clear();
			
			for (i = 0; i < Centroids.size(); i++){
				for (j = 0; j < Centroids.size(); j++) {
					if (i != j) {
						
						

						CreateAuxFile(Centroids[i], Centroids[j]);
						cout << "\nPar " << Centroids[i] << " " << Centroids[j] << endl;
						//yenAlg.run(my_graph.get_vertex(Centroids[i]), my_graph.get_vertex(Centroids[j]), Kf_cento, &nCaminhos);
						yenAlg.run(my_graph.get_vertex(Centroids[i]), my_graph.get_vertex(Centroids[j]), Kf, &nCaminhos);
						
						

						

						

						cout << "\n**********************************************\n";
					}
				}
			}

		}
		else if (AnalysisType == 2){
			
			Parameters >> nPares;
			CentroidsCount(nPares, Kf);
			Parameters.ignore(10000, '\n');

			vector<int> MatrizPares(2 * nPares);	// Cria um vetor com os pares origem-destino

			for (i = 0; i < 2 * nPares; i = i + 2) {
				Parameters >> MatrizPares[i];			// Pega os nós de origem e destino
				Parameters >> MatrizPares[i + 1];
				Parameters.ignore(10000, '\n');
			}


			for (i = 0; i < 2 * nPares; i = i + 2) {

				
				
				CreateAuxFile(MatrizPares[i], MatrizPares[i + 1]);		// Imprime os nós de origem e destino no arquivo auxiliar
				cout << "\nPar " << MatrizPares[i] << " " << MatrizPares[i + 1] << endl;
				//yenAlg.run(my_graph.get_vertex(MatrizPares[i]), my_graph.get_vertex(MatrizPares[i + 1]), Kf_cento, &nCaminhos);
				yenAlg.run(my_graph.get_vertex(MatrizPares[i]), my_graph.get_vertex(MatrizPares[i + 1]), Kf, &nCaminhos);
							
				

				cout << "\n**********************************************\n";
			}

		}
		else {
			cout << "\nO tipo de analise so aceita os numeros 1 ou 2!\n\n";
		}


		
		
		

		ofstream Outputfile;
		Outputfile.open("Results.txt", ios_base::app);
		Outputfile << "\n\nTotal: " << nCaminhos << " caminhos";

		auto stop = chrono::high_resolution_clock::now();
		auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);

		cout << "Tempo de execucao: " << duration.count() << " milisegundos\n\n";
	
		system("pause");
	return 0;
}
