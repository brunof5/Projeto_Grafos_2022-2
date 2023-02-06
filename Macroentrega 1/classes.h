#include <iostream>
#include <iomanip>
#include <fstream>
#include <exception>
#include <vector>

using namespace std;

#define INVALIDO -1 

// Classe "lines", armazena os dados das 10 primeiras linhas da instancia
class lines{
	
	// permite que as classes "nodes" e "vehicles" acessem os atributos privados de "lines"
	friend class nodes;
	friend class vehicles;
	
	private:
		string NAME;
		string LOCATION;
		string COMMENT;
		string TYPE;
		string DISTRIBUTION;
		string DEPOT;
		int SIZE;
		int ROUTE_TIME;
		int TIME_WINDOW;
		int CAPACITY;
		
	public:
		lines();
		int ler_10_primeiras_linhas(ifstream& arquivo_instancia);
};

// Construtor de "lines"
lines::lines(){
	
	SIZE = INVALIDO;
	ROUTE_TIME = INVALIDO;
	TIME_WINDOW = INVALIDO;
	CAPACITY = INVALIDO;
}

// Funcao para ler as 10 primeiras linhas da instancia, retorna o tamanho do grafo/numero de vertices
int lines::ler_10_primeiras_linhas(ifstream& arquivo_instancia){
	
	string texto;
	
	for(int i = 0; i < 10; i++){
		
		// leitura ate o primeiro espaco
		getline(arquivo_instancia, texto, ' ');
		
		// leitura dos dados. SIZE, ROUTE_TIME, TIME_WINDOW e CAPACITY sao transformados em inteiros
		switch(i){
			
			case 0:
				getline(arquivo_instancia, NAME);
				break;
				
			case 1:
				getline(arquivo_instancia, LOCATION);
				break;
				
			case 2:
				// "COMMENT" eh a unica string que fica com um espaco a esquerda, excecao nao tratada
				getline(arquivo_instancia, COMMENT);
				break;
				
			case 3:
				getline(arquivo_instancia, TYPE);
				break;
				
			case 4:
				getline(arquivo_instancia, texto);
				SIZE = stoi(texto);
				break;
				
			case 5:
				getline(arquivo_instancia, DISTRIBUTION);
				break;
				
			case 6:
				getline(arquivo_instancia, DEPOT);
				break;
				
			case 7:
				getline(arquivo_instancia, texto);
				ROUTE_TIME = stoi(texto);
				break;
				
			case 8:
				getline(arquivo_instancia, texto);
				TIME_WINDOW = stoi(texto);
				break;
				
			case 9:
				getline(arquivo_instancia, texto);
				CAPACITY = stoi(texto);
				break;
		}
	}
	
	return SIZE;
}

// Classe "nodes", armazena os dados de todos os vertice do vertice
class nodes{
	
	// permite que a classes "vehicles" acesse os atributos privados de "nodes"
	friend class vehicles;
	
	private:
		int *id;
		double *lat;
		double *longit;
		int *dem;
		int *etw;
		int *ltw;
		int *dur;
		int *p;
		int *d;
		int **dist;
		
	public:
		nodes(lines *linhas);
		void destrutor(lines *linhas);
		void ler_nohs(ifstream& arquivo_instancia, lines *linhas);
		void precedencia(lines *linhas);
		void distancia_dos_nohs(ifstream& arquivo_instancia, lines *linhas);
		void verificar_restricoes_solucao_aleatoria(ifstream& arq_solucao, lines *linhas);
};

// Construtor de "nodes"
nodes::nodes(lines *linhas){
	
	id = new int[linhas->SIZE];
	lat = new double[linhas->SIZE];
	longit = new double[linhas->SIZE];
	dem = new int[linhas->SIZE];
	etw = new int[linhas->SIZE];
	ltw = new int[linhas->SIZE];
	dur = new int[linhas->SIZE];
	p = new int[linhas->SIZE];
	d = new int[linhas->SIZE];
	dist = new int*[linhas->SIZE];
	for(int i = 0; i < linhas->SIZE; i++)
		dist[i] = new int[linhas->SIZE];
}

// Destrutor de "nodes"
void nodes::destrutor(lines *linhas){
	
	delete[] id;
	delete[] lat;
	delete[] longit;
	delete[] dem;
	delete[] etw;
	delete[] ltw;
	delete[] dur;
	delete[] p;
	delete[] d;
	for(int i = 0; i < linhas->SIZE; i++)
		delete dist[i];
	delete[] dist;
}

// Funcao para ler todos os vertices do grafo
void nodes::ler_nohs(ifstream& arquivo_instancia, lines *linhas){
	
	for(int i = 0; i < linhas->SIZE; i++){
		
		for(int j = 0; j < 9; j++){
			
			// leitura dos dados
			switch(j){
				
				case 0:
					arquivo_instancia >> id[i];
					break;
					
				case 1:
					arquivo_instancia >> lat[i];
					break;
					
				case 2:
					arquivo_instancia >> longit[i];
					break;
					
				case 3:
					arquivo_instancia >> dem[i];
					if(dem[i] > linhas->CAPACITY){
						
						cout << "O vertice de id igual a " << id[i] << " possui capacidade superior ao esperado!" << endl;
						cout << "Capacidade maxima esperada: " << linhas->CAPACITY << endl;
						cout << "Capacidade do vertice encontrada: " << dem[i] << endl;
						throw runtime_error("ERRO CAPACIDADE - capacidade de demanda do vertice ultrapassada!");
					}
					break;
					
				case 4:
					arquivo_instancia >> etw[i];
					break;
					
				case 5:
					arquivo_instancia >> ltw[i];
					if(ltw[i] > linhas->ROUTE_TIME){
						
						cout << "O vertice de id igual a " << id[i] << " possui a janela de tempo superior ao esperado!" << endl;
						cout << "Janela de tempo maxima esperada: " << linhas->ROUTE_TIME << endl;
						cout << "Janela de tempo maxima do vertice encontrada: " << ltw[i] << endl;
						throw runtime_error("ERRO JANELA DE TEMPO - janela de tempo do vertice ultrapassada!");
					}
					/*if(id != 0 and ltw - etw > linhas->TIME_WINDOW){
						
						cout << "O vertice de id igual a " << id[i] << " possui a largura da janela de tempo superior ao esperado!" << endl;
						cout << "Largura da janela de tempo esperada: " << linhas->TIME_WINDOW << endl;
						cout << "Largura da janela de tempo do vertice encontrada: " << ltw[i] - etw[i] << endl;
						throw runtime_error("ERRO LARGURA JANELA DE TEMPO - largura da janela de tempo do vertice ultrapassada!");
					}*/
					break;
					
				case 6:
					arquivo_instancia >> dur[i];
					break;
					
				case 7:
					arquivo_instancia >> p[i];
					break;
					
				case 8:
					arquivo_instancia >> d[i];
					break;
			}
		}
	}
}

// Funcao para fins de integridade por 'p' e 'd'
// Poderia retornar booleano, mas tratou-se os falsos com <exception>
// Se todos os casos forem verdadeiros, nao acontece nada
void nodes::precedencia(lines *linhas){
	
	int aux_d = 0;
	int aux_p = 0;
	
	for(int i = 0; i < linhas->SIZE; i++){
		
		// deposito unico
		if(dem[i] == 0){
			
			if(p[i] != 0 or d[i] != 0){
				
				cout << "O vertice do deposito unico (id = 0) possui um erro de precedencia!" << endl;
				cout << "Valores de p e d esperados: 0" << endl;
				cout << "Valores de p e d encontrados: " << p[i] << " " << d[i] << endl;
				throw runtime_error("ERRO PRECEDENCIA - erro de integridade!");
			}
		}
		
		// o vertice analisado eh de coleta
		if(dem[i] > 0){
			
			// valor esperado
			aux_d = id[i] + ((linhas->SIZE - 1) / 2);
			aux_p = 0;
			
			// analise do par de entrega 'd'
			if(p[i] == 0 and d[i] != 0){
				
				if(d[i] != aux_d){
					
					cout << "O vertice de id igual a " << id[i] << " possui um erro de precedencia!" << endl;
					cout << "Vertice de entrega esperado: " << aux_d << endl;
					cout << "Vertice de entrega encontrado: " << d[i] << endl;
					throw runtime_error("ERRO PRECEDENCIA - erro de integridade!");
				}
			}
		}
		
		// o vertice analisado eh de entrega
		if(dem[i] < 0){
			
			aux_d = 0;
			// valor esperado
			aux_p = id[i] - ((linhas->SIZE - 1) / 2);
			
			// analise do par de coleta 'p'
			if(p[i] != 0 and d[i] == 0){
				
				if(p[i] != aux_p){
					
					cout << "O vertice de id igual a " << id[i] << " possui um erro de precedencia!" << endl;
					cout << "Vertice de coleta esperado: " << aux_p << endl;
					cout << "Vertice de coleta encontrado: " << p[i] << endl;
					throw runtime_error("ERRO PRECEDENCIA - erro de integridade!");
				}
			}
		}
		
		// caso raro, nunca deveria acontecer
		if(p[i] != 0 and d[i] != 0){
			
			aux_d = id[i] + ((linhas->SIZE - 1) / 2);
			aux_p = id[i] - ((linhas->SIZE - 1) / 2);
			
			cout << "O vertice de id igual a " << id[i] << " possui um erro de precedencia!" << endl;
			cout << "Vertice de entrega esperado: " << aux_d << endl;
			cout << "Vertice de entrega encontrado: " << d[i] << endl;
			cout << "Vertice de coleta esperado: " << aux_p << endl;
			cout << "Vertice de coleta encontrado: " << p[i] << endl;
			throw runtime_error("ERRO PRECEDENCIA - erro de integridade!");
		}
	}
}

// Funcao para leitura das distancias dos vertices
void nodes::distancia_dos_nohs(ifstream& arquivo_instancia, lines *linhas){
	
	for(int i = 0; i < linhas->SIZE; i++)
		for(int j = 0; j < linhas->SIZE; j++)
			arquivo_instancia >> dist[i][j];
}

// Classe "vehicles", armazena a rota dos veiculos
class vehicles{
	
	private:
		int *id;
		vector<int> *rota;
		int qtd_veiculos;
		
	public:
		vehicles(ifstream& arq_solucao);
		~vehicles();
		void leitura_rota(ifstream& arq_solucao);
		void restricao_1(ifstream& arq_solucao, lines *linhas, nodes *nohs);
		void restricao_2(ifstream& arq_solucao, lines *linhas, nodes *nohs);
		void restricao_3(ifstream& arq_solucao, lines *linhas, nodes *nohs);
		void restricao_4(ifstream& arq_solucao, lines *linhas, nodes *nohs);
		void restricao_5(ifstream& arq_solucao, lines *linhas, nodes *nohs);
		void restricao_6(ifstream& arq_solucao, lines *linhas, nodes *nohs);
};

// Construtor de "vehicles"
vehicles::vehicles(ifstream& arq_solucao){
	
	// leitura da linhas VEICULOS
	string texto;
	arq_solucao >> texto;
	
	// leitura da quantidade de veiculos
	arq_solucao >> qtd_veiculos;
	
	// leitura da linhas ROTAS
	arq_solucao >> texto;
	
	id = new int[qtd_veiculos];
	rota = new vector<int>[qtd_veiculos];
}

// Destrutor de "vehicles"
vehicles::~vehicles(){
	
	delete[] id;
	delete[] rota;
}

// Funcao para leitura da rota de um veiculo
void vehicles::leitura_rota(ifstream& arq_solucao){
	
	int num_atual;
	
	// leitura da rota do veiculo atual
	for(int i = 0; i < qtd_veiculos; i++){
		
		int tamanho_rota_atual;
		
		arq_solucao >> tamanho_rota_atual;
		
		for(int j = 0; j < tamanho_rota_atual; j++){
			
			arq_solucao >> num_atual;
			rota[i].push_back(num_atual);
		}
	}
}

// RESTRICAO 1: em um pedido r = (i, j), j nao pode ser visitado antes de i
void vehicles::restricao_1(ifstream& arq_solucao, lines *linhas, nodes *nohs){
	
	cout << "Restricao 1: " << endl << endl;
	
	int num_atual;
	
	for(int i = 0; i < qtd_veiculos; i++){
		
		int pos = 0;
		
		for(auto it = rota[i].begin(); it != rota[i].end(); it++, pos++){
			
			num_atual = *it;
			
			// num_atual eh ponto de entrega
			if(nohs->dem[num_atual] < 0){
				
				// ponto de coleta esperado
				int aux_p = num_atual - ((linhas->SIZE - 1) / 2);
				
				// caso o ponto i seja encontrado antes de j, o while é quebrado
				int k = 0;
				while(k < pos){
					
					if(rota[i][k] == aux_p)
						break;
						
					k++;
				}
				
				// caso k percorra ate pos, quer dizer que o ponto j apareceu depois de i
				if(k == pos){
					
					cout << "A rota do veiculo " << i + 1 << " possui o vertice de entrega " << num_atual << " que foi visitado antes do seu vertice de coleta!" << endl;
					cout << endl;
				}
			}
		}
	}
	
	cout << "##################################" << endl;
	cout << endl;
}

// RESTRICAO 2: Cada veículo deve partir e retornar ao ponto de origem {O}, seguindo o tempo [aO, bO]
void vehicles::restricao_2(ifstream& arq_solucao, lines *linhas, nodes *nohs){
	
	cout << "Restricao 2: " << endl << endl;
	
	for(int i = 0; i < qtd_veiculos; i++){
		
		int num_atual = 0;
		int tempo_total = 0;
		int num_prox;
		bool segundo_zero = false;
		
		for(auto it = rota[i].begin(); it != rota[i].end(); it++){
			
			// tempo_total eh somado ate chegar ao segundo 0 (deposito)
			if(!segundo_zero){
				
				num_atual = *it;
				num_prox = *(it + 1);
				tempo_total += nohs->dist[num_atual][num_prox];
			}
			
			if(tempo_total < nohs->etw[num_prox])
				tempo_total = nohs->etw[num_prox] + nohs->dur[num_prox];
				
			else
				tempo_total += nohs->dur[num_prox];
				
			if(num_prox == 0)
				segundo_zero = true;
		}
		
		cout << "O tempo total gasto pelo veiculo " << i + 1 << " eh: " << tempo_total << "." << endl;
		if(tempo_total > linhas->ROUTE_TIME)
			cout << "Logo, ultrapassa o limite de: " << linhas->ROUTE_TIME << "!" << endl;
			
		cout << endl;
	}
	
	cout << "##################################" << endl;
	cout << endl;
}

/* RESTRICAO 3: O tempo de chegada de certo veículo ao ponto i ou j não pode exceder o ltw de tal ponto. 
 * Caso chegue antes de etw de i ou j, o veículo deve esperar até o respectivo tempo para realizar a ação;
 */
void vehicles::restricao_3(ifstream& arq_solucao, lines *linhas, nodes *nohs){
	
	cout << "Restricao 3: " << endl << endl;
	
	for(int i = 0; i < qtd_veiculos; i++){
		
		int num_atual = 0;
		int num_prox = 0;
		int tempo_total = 0;
		bool segundo_zero = false;
		
		for(auto it = rota[i].begin(); it != rota[i].end(); it++){
			
			// tempo_total eh usado para comparar com os valores de etw e ltw de cada vertice da rota
			if(!segundo_zero){
				
				num_atual = *it;
				num_prox = *(it + 1);
				tempo_total += nohs->dist[num_atual][num_prox];
				
				if(tempo_total < nohs->etw[num_prox]){
					
					cout << "O veiculo " << i + 1 << " chegou cedo no ponto " << num_prox << "!" << endl;
					cout << "Janela de tempo inicial esperada: " << nohs->etw[num_prox] << "." << endl;
					cout << "Tempo de chegada: " << tempo_total << "." << endl;
					cout << endl;
					
					tempo_total = nohs->etw[num_prox] + nohs->dur[num_prox];
				}
				
				else
					tempo_total += nohs->dur[num_prox];
				
				if(tempo_total > nohs->ltw[num_prox]){
					
					if(num_prox == 0){
						
						cout << "O veiculo " << i + 1 << " excedeu o limite da janela de tempo para chegar ao deposito!" << endl;
						cout << "Janela de tempo final esperada: " << nohs->ltw[num_prox] << "." << endl;
						cout << "Tempo de chegada: " << tempo_total << "." << endl;
						cout << endl;
					}
					
					else if(num_prox <= (linhas->SIZE - 1) / 2){
						
						cout << "O veiculo " << i + 1 << " excedeu o limite da janela de tempo para coletar no ponto " << num_prox << "!" << endl;
						cout << "Janela de tempo final esperada: " << nohs->ltw[num_prox] << "." << endl;
						cout << "Tempo de chegada: " << tempo_total << "." << endl;
						cout << endl;
					}
					
					else if(num_prox <= (linhas->SIZE - 1)){
						
						cout << "O veiculo " << i + 1 << " excedeu o limite da janela de tempo para entregar no ponto " << num_prox << "!" << endl;
						cout << "Janela de tempo final esperada: " << nohs->ltw[num_prox] << "." << endl;
						cout << "Tempo de chegada: " << tempo_total << "." << endl;
						cout << endl;
					}
				}
			}
			
			if(num_prox == 0)
				segundo_zero = true;
		}
	}
	
	cout << "##################################" << endl;
	cout << endl;
}

// RESTRICAO 4: Sendo um pedido r = (i, j), i e j devem ser visitados exatamente uma vez
void vehicles::restricao_4(ifstream& arq_solucao, lines *linhas, nodes *nohs){
	
	cout << "Restricao 4: " << endl << endl;
	
	int num_atual;
	
	for(int i = 0; i < qtd_veiculos; i++){
		
		int pos = 0;
		
		for(auto it = rota[i].begin(); it != rota[i].end(); it++, pos++){
			
			num_atual = *it;
			
			if(num_atual == 0)
				continue;
			
			// caso o ponto i ou j seja encontrado outra vez, o while eh quebrado
			int k = 0;
			while(k < pos){
				
				if(rota[i][k] == num_atual)
					break;
					
				k++;
			}
			
			// caso k percorra ate pos, quer dizer que o ponto i ou j nao foi encontrado novamente
			if(k != pos){
				
				cout << "A rota do veiculo " << i + 1 << " possui o ponto " << num_atual << " visitado novamente!" << endl;
				cout << endl;
			}
		}
	}
	
	cout << "##################################" << endl;
	cout << endl;
}

// RESTRICAO 5: Sendo um pedido r = (i, j), o mesmo veículo que passa por i passa por j
void vehicles::restricao_5(ifstream& arq_solucao, lines *linhas, nodes *nohs){
	
	cout << "Restricao 5: " << endl << endl;
	
	for(int x = 0; x < qtd_veiculos; x++){
		
		if(x + 1 == qtd_veiculos)
			continue;
			
		for(int i = x + 1; i < qtd_veiculos; i++){
			
			int tam_rota_atual = rota[x].size();
			int tam_rota_prox = rota[i].size();
			
			for(int j = 0; j < tam_rota_atual; j++){
				
				if(rota[x][j] == 0)
					continue;
					
				for(int k = 0; k < tam_rota_prox; k++){
					
					if(rota[i][k] == 0)
						continue;
						
					if(rota[x][j] == rota[i][k]){
						
						cout << "O veiculo " << i + 1 << " possui um ponto em comum com o veiculo " << i + 2 << "!" << endl;
						cout << "Ponto em comum: " << rota[x][j] << "." << endl;
						cout << endl;
					}
				}
			}
		}
	}
	
	cout << "##################################" << endl;
	cout << endl;
}

// RESTRICAO 6: O somatorio das demandas dos pontos atendidos por certo veículo não pode ultrapassar sua capacida
void vehicles::restricao_6(ifstream& arq_solucao, lines *linhas, nodes *nohs){
	
	cout << "Restricao 6: " << endl << endl;
	
	for(int i = 0; i < qtd_veiculos; i++){
		
		int num_atual = 0;
		int dem_total = 0;
		
		for(auto it = rota[i].begin(); it != rota[i].end(); it++){
			
			num_atual = *it;
			
			dem_total += nohs->dem[num_atual];
			
			if(dem_total > linhas->CAPACITY){
				
				cout << "O veiculo " << i + 1 << " excedeu sua capacidade no ponto " << num_atual << " de sua rota!" << endl;
				cout << "Capacidade esperada: " << linhas->CAPACITY << "." << endl;
				cout << "Capacidade soliticada ao veiculo: " << dem_total << "." << endl;
				cout << endl;
			}
		}
	}
	
	cout << "##################################" << endl;
	cout << endl;
}
