#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>

using namespace std;

// Funcao para gerar uma solucao aleatoria
void gerar_solucao_aleatoria(int SIZE){
	
	// permite que o random_shuffle do vector seja diferente cada vez executado o programa
	srand(time(NULL));
	
	// gera o arquivo texto
	ofstream arq_solucao("solucao_gerada.txt");
	arq_solucao << "VEICULOS ";
	
	int qtd_veiculos;
	cout << "Digite o numero de veiculos: ";
	cin >> qtd_veiculos;
	arq_solucao << qtd_veiculos << '\n';
	
	arq_solucao << "ROTAS\n";
	
	// cria-se uma rota para cada veiculo
	vector<int> *rotas = new vector<int>[qtd_veiculos];
	vector<int> random_numbers;
	
	for(int i = 0; i < qtd_veiculos; i++){
		
		// cria-se um vector de 1 até o numero de vertices - 1 (desconsidera o deposito unico)
		for(int j = 0; j < SIZE - 1; j++)
			random_numbers.push_back(j + 1);
		
		// randomiza os numeros nas posicoes do vector
		random_shuffle(random_numbers.begin(), random_numbers.end());
		
		// insere-se 0 no inicio e no fim da rota para representar o local de origem e destino (deposito unico)
		rotas[i] = random_numbers;
		rotas[i].insert(rotas[i].begin(), 0);
		rotas[i].push_back(0);
		
		random_numbers.clear();
		
		/* insere-se as informacoes no arquivo das rotas
		 * SIZE + 1, pois tem-se inicialmente SIZE - 1 (desconsiderando o deposito unico)
		 * ao realizar a insercao do 0 no inicio e no fim, temos + 2 vertices para percorrer
		 * SIZE - 1 + 2 = SIZE + 1 elementos em cada rota
		 */
		for(int k = 0; k < SIZE + 1; k++){
			
			if(k == SIZE)
				arq_solucao << rotas[i][k];
			
			else
				arq_solucao << rotas[i][k] << " ";
		}
		
		arq_solucao << '\n';
	}
	
	arq_solucao << "EOF";
	
	arq_solucao.close();
	delete[] rotas;
}
