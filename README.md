# k-shortest-paths
Implementação em C++ do algoritmo proposto por Martins e Pascoal (2003)[1] para obtenção dos k-caminhos mais curtos entre um par origem-destino
Adaptado a partir da implementação disponível em https://github.com/yan-qi/k-shortest-paths-cpp-version

As adaptações feitas permitem que seja analisado mais de um par OD de cada vez. 
Por preferência pessoal, também alterei o programa para funcionar a partir de um executável que lê um arquivo de entrada e gera um arquivo de saída para processamento posterior por outros programas.

Um problema conhecido e que está sendo analisado é que o programa apresenta um memory leak, que pode tornar inviável as análises de problemas muito grandes.

[1] MARTINS, E. Q. V., PASCOAL, M. M. B, 2003, A new implementation of Yen’s ranking loopless paths algorithm, Quarterly Journal of the Belgian, French and Italian Operations Research Societies, vol 1(2), p. 121-133.