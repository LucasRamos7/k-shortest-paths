Arquivo de entrada: deve-se chamar InputYen.txt

1ª linha: instância com as arestas e seus pesos
2ª linha: número de caminhos em cada par origem-destino
3ª linha: tipo de análise (1 - todos os pares; 2 - apenas os pares definidos)
4ª linha: Número de pares OD a serem avaliados (se o valor da 3ª linha for 2, pode ficar em branco)
5ª linha em diante:	Tipo 1: nós de interesse, um por linha
					Tipo 2: pares OD, um por linha, origem e destino separados por espaço

Arquivo de saída: Results.txt

1ª linha: instância com as arestas e seus pesos
2ª linha: número de pares OD analisados
3ª linha: número de caminhos analisados por par OD
4ª linha: em branco

5ª linha: nós de origem e de destino do par OD
6ª linha: número de caminhos encontrados para o par
7ª linha: número de nós no caminhos com maior quantidade de nós
8ª a Xª linha: cada caminho, com o número de nós, seu custo, e a lista de nós percorridos

Após uma linha em branco, o padrão a partir da 5ª linha se repete para cada par OD