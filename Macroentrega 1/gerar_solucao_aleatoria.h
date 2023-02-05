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
	
	// cria-se um vector dos pedidos de coleta
	for(int i = 0; i < (SIZE - 1) / 2; i++)
		random_numbers.push_back(i + 1);
		
	// randomiza os numeros nas posicoes do vector
	random_shuffle(random_numbers.begin(), random_numbers.end());
	
	int tamanho_rota = 0, num_pedidos = (SIZE - 1) / 2, pedidos_atendidos = 0;
	
	for(int i = 0; i < qtd_veiculos; i++){
		
		cout << "Digite o numero de pedidos da rota (numeros de pedidos disponiveis = " << ((SIZE - 1) / 2) - pedidos_atendidos << "): ";
		cin >> tamanho_rota;
		
		arq_solucao << (tamanho_rota * 2) + 2 << " ";
		
		int j = 0;
		while(pedidos_atendidos < num_pedidos and j < tamanho_rota){
			
			rotas[i].push_back(random_numbers[pedidos_atendidos]);
			pedidos_atendidos++;
			j++;
		}
		// insere-se 0 no inicio e no fim da rota para representar o local de origem e destino (deposito unico)
		rotas[i].insert(rotas[i].begin(), 0);
		rotas[i].push_back(0);
		
		bool primeiro_zero = false;
		
		// insere-se as informacoes no arquivo das rotas
		for(int k = 0; k < tamanho_rota + 2; k++){
			
			if(rotas[i][k] == 0 and !primeiro_zero){
				
				arq_solucao << rotas[i][k] << " ";
				primeiro_zero = true;
			}
			
			else if(rotas[i][k] == 0 and primeiro_zero)
				arq_solucao << rotas[i][k];
				
			else{
				
				arq_solucao << rotas[i][k] << " " << rotas[i][k] + ((SIZE - 1) / 2) << " ";
			}
		}
		
		arq_solucao << '\n';
		cout << endl;
	}
	
	arq_solucao.close();
	delete[] rotas;
}
