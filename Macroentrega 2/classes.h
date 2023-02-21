#include <iostream>
#include <iomanip>
#include <fstream>
#include <exception>
#include <vector>
#include <algorithm>

using namespace std;

#define INVALIDO -1

class Lines{
	
	private:
		string NAME;
		string LOCATION;
		string COMMENT;
		string TYPE;
		string DISTRIBUTION;
		string DEPOT;
		
	protected:
		int SIZE;
		int ROUTE_TIME;
		int TIME_WINDOW;
		int CAPACITY;
		
	public:
		Lines();
		int ler_10_primeiras_linhas(ifstream& arquivo_instancia);
};

Lines::Lines(){
	
	SIZE = INVALIDO;
	ROUTE_TIME = INVALIDO;
	TIME_WINDOW = INVALIDO;
	CAPACITY = INVALIDO;
}

int Lines::ler_10_primeiras_linhas(ifstream& arquivo_instancia){
	
	string texto;
	
	for(int i = 0; i < 10; i++){
		
		getline(arquivo_instancia, texto, ' ');
		
		switch(i){
			
			case 0:
				getline(arquivo_instancia, NAME);
				break;
				
			case 1:
				getline(arquivo_instancia, LOCATION);
				break;
				
			case 2:
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

// #############################################################

class Nodes : protected Lines{
	
	protected:
		int *id_noh;
		double *lat;
		double *longit;
		int *dem;
		int *etw;
		int *ltw;
		int *dur;
		int *p;
		int *d;
		int **dist;
		bool *visitado;
		
	public:
		Nodes(Lines *linhas);
		~Nodes();
		void ler_nohs(ifstream& arquivo_instancia);
		void precedencia();
		void distancia_dos_nohs(ifstream& arquivo_instancia);
};

Nodes::Nodes(Lines *linhas) : Lines(*linhas){
	
	id_noh = new int[SIZE];
	lat = new double[SIZE];
	longit = new double[SIZE];
	dem = new int[SIZE];
	etw = new int[SIZE];
	ltw = new int[SIZE];
	dur = new int[SIZE];
	p = new int[SIZE];
	d = new int[SIZE];
	dist = new int*[SIZE];
	for(int i = 0; i < SIZE; i++)
		dist[i] = new int[SIZE];
	visitado = new bool[SIZE];
	
	for(int i = 0; i < SIZE; i++){
		
		id_noh[i] = INVALIDO;
		lat[i] = INVALIDO;
		longit[i] = INVALIDO;
		dem[i] = 0;
		etw[i] = INVALIDO;
		ltw[i] = INVALIDO;
		dur[i] = INVALIDO;
		p[i] = INVALIDO;
		d[i] = INVALIDO;
		visitado[i] = false;
		for(int j = 0; j < SIZE; j++)
			dist[i][j] = INVALIDO;
	}
}

Nodes::~Nodes(){
	
	delete[] id_noh;
	delete[] lat;
	delete[] longit;
	delete[] dem;
	delete[] etw;
	delete[] ltw;
	delete[] dur;
	delete[] p;
	delete[] d;
	for(int i = 0; i < SIZE; i++)
		delete dist[i];
	delete[] dist;
	delete[] visitado;
}

void Nodes::ler_nohs(ifstream& arquivo_instancia){
	
	string texto;
	// leitura da linha NODES
	arquivo_instancia >> texto;
	
	for(int i = 0; i < SIZE; i++){
		
		for(int j = 0; j < 9; j++){
			
			switch(j){
				
				case 0:
					arquivo_instancia >> id_noh[i];
					break;
					
				case 1:
					arquivo_instancia >> lat[i];
					break;
					
				case 2:
					arquivo_instancia >> longit[i];
					break;
					
				case 3:
					arquivo_instancia >> dem[i];
					if(dem[i] > CAPACITY){
						
						cout << "O vertice de id igual a " << id_noh[i] << " possui capacidade superior ao esperado!" << endl;
						cout << "Capacidade maxima esperada: " << CAPACITY << endl;
						cout << "Capacidade do vertice encontrada: " << dem[i] << endl;
						throw runtime_error("ERRO CAPACIDADE - capacidade de demanda do vertice ultrapassada!");
					}
					break;
					
				case 4:
					arquivo_instancia >> etw[i];
					break;
					
				case 5:
					arquivo_instancia >> ltw[i];
					if(ltw[i] > ROUTE_TIME){
						
						cout << "O vertice de id igual a " << id_noh[i] << " possui a janela de tempo superior ao esperado!" << endl;
						cout << "Janela de tempo maxima esperada: " << ROUTE_TIME << endl;
						cout << "Janela de tempo maxima do vertice encontrada: " << ltw[i] << endl;
						throw runtime_error("ERRO JANELA DE TEMPO - janela de tempo do vertice ultrapassada!");
					}
					/*if(id_noh != 0 and ltw - etw > TIME_WINDOW){
						
						cout << "O vertice de id igual a " << id_noh[i] << " possui a largura da janela de tempo superior ao esperado!" << endl;
						cout << "Largura da janela de tempo esperada: " << TIME_WINDOW << endl;
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

void Nodes::precedencia(){
	
	int aux_d = 0;
	int aux_p = 0;
	
	for(int i = 0; i < SIZE; i++){
		
		// deposito unico
		if(dem[i] == 0){
			
			if(p[i] != 0 or d[i] != 0){
				
				cout << "O vertice do deposito unico (id = 0) possui um erro de precedencia!" << endl;
				cout << "Valores de p e d esperados: 0" << endl;
				cout << "Valores de p e d encontrados: " << p[i] << " " << d[i] << endl;
				throw runtime_error("ERRO PRECEDENCIA - erro de integridade!");
			}
		}
		
		// vertice de coleta
		if(dem[i] > 0){
			
			aux_d = id_noh[i] + ((SIZE - 1) / 2);
			aux_p = 0;
			
			if(p[i] == 0 and d[i] != 0){
				
				if(d[i] != aux_d){
					
					cout << "O vertice de id igual a " << id_noh[i] << " possui um erro de precedencia!" << endl;
					cout << "Vertice de entrega esperado: " << aux_d << endl;
					cout << "Vertice de entrega encontrado: " << d[i] << endl;
					throw runtime_error("ERRO PRECEDENCIA - erro de integridade!");
				}
			}
		}
		
		// vertice de entrega
		if(dem[i] < 0){
			
			aux_d = 0;
			aux_p = id_noh[i] - ((SIZE - 1) / 2);
			
			if(p[i] != 0 and d[i] == 0){
				
				if(p[i] != aux_p){
					
					cout << "O vertice de id igual a " << id_noh[i] << " possui um erro de precedencia!" << endl;
					cout << "Vertice de coleta esperado: " << aux_p << endl;
					cout << "Vertice de coleta encontrado: " << p[i] << endl;
					throw runtime_error("ERRO PRECEDENCIA - erro de integridade!");
				}
			}
		}
		
		// caso raro, nunca deveria acontecer
		if(p[i] != 0 and d[i] != 0){
			
			aux_d = id_noh[i] + ((SIZE - 1) / 2);
			aux_p = id_noh[i] - ((SIZE - 1) / 2);
			
			cout << "O vertice de id igual a " << id_noh[i] << " possui um erro de precedencia!" << endl;
			cout << "Vertice de entrega esperado: " << aux_d << endl;
			cout << "Vertice de entrega encontrado: " << d[i] << endl;
			cout << "Vertice de coleta esperado: " << aux_p << endl;
			cout << "Vertice de coleta encontrado: " << p[i] << endl;
			throw runtime_error("ERRO PRECEDENCIA - erro de integridade!");
		}
	}
}

void Nodes::distancia_dos_nohs(ifstream& arquivo_instancia){
	
	string texto;
	// leitura da linha EDGES
	arquivo_instancia >> texto;
	
	for(int i = 0; i < SIZE; i++)
		for(int j = 0; j < SIZE; j++)
			arquivo_instancia >> dist[i][j];
}

// #############################################################

class Auxiliary_and_Restrictions : protected Nodes{
	
	protected:
		bool restricao_1_2_3_6_alguma_rota(vector<int> nova_rota);
		bool restricao_2_3_6_alguma_rota(int &prioridade, vector<int> nova_rota);
		int calcular_tempo_alguma_rota(vector<int> nova_rota);
		
	public:
		Auxiliary_and_Restrictions(Nodes *nohs) : Nodes(*nohs) {};
		void marcar_nohs_visitados_remover_pacote(vector<pair<int,int>> &pedidos, pair<int,int> pacote);
		pair<int,int> retornar_pacote(vector<int> nova_rota);
		vector<int> swap_optimization(vector<int> nova_rota);
		void _2_opt(vector<int> &rota);
};

void Auxiliary_and_Restrictions::marcar_nohs_visitados_remover_pacote(vector<pair<int,int>> &pedidos, pair<int,int> pacote){
	
	visitado[pacote.first] = true;
	visitado[pacote.second] = true;
	
	for(unsigned int i = 0; i < pedidos.size(); i++)
		if(pacote.first == pedidos.at(i).first){
			
			pedidos.erase(pedidos.begin() + i);
			break;
		}
}

pair<int,int> Auxiliary_and_Restrictions::retornar_pacote(vector<int> nova_rota){
	
	int ponto_coleta = INVALIDO;
	int ponto_entrega = INVALIDO;
	
	for(auto it = nova_rota.begin(); it != nova_rota.end(); it++)
		if(!visitado[*it]){
			
			if(ponto_coleta != INVALIDO and ponto_entrega == INVALIDO)
				ponto_entrega = *it;
			
			if(ponto_coleta == INVALIDO)
				ponto_coleta = *it;
		}
		
	pair<int,int> par_coleta_entrega = make_pair(ponto_coleta, ponto_entrega);
	
	return par_coleta_entrega;
}

bool Auxiliary_and_Restrictions::restricao_1_2_3_6_alguma_rota(vector<int> nova_rota){
	
	int vertice_atual = 0;
	int vertice_prox = 0;
	int tempo_total = 0;
	int dem_total = 0;
	int pos = 0;
	bool segundo_zero = false;
	
	for(auto it = nova_rota.begin(); !segundo_zero and it != nova_rota.end(); it++, pos++){
		
		// tempo_total eh somado ate chegar ao segundo 0 (deposito)
		vertice_atual = *it;
		vertice_prox = *(it + 1);
		tempo_total += dist[vertice_atual][vertice_prox];
		dem_total += dem[vertice_atual];
		
		// vertice_atual eh ponto de entrega
		if(dem[vertice_atual] < 0){
			
			int aux_p = vertice_atual - ((SIZE - 1) / 2);
			
			// caso o ponto i seja encontrado antes de j, o while é quebrado
			int k = 0;
			while(k < pos){
				
				if(nova_rota[k] == aux_p)
					break;
					
				k++;
			}
			// caso k percorra ate pos, quer dizer que o ponto j apareceu depois de i
			if(k == pos)
				return false;
		}
		
		// excedeu a capacidade
		if(dem_total > CAPACITY)
			return false;
		
		// veiculo chegou antes de ETW do ponto
		if(tempo_total < etw[vertice_prox])
			tempo_total = etw[vertice_prox] + dur[vertice_prox];
		
		else
			tempo_total += dur[vertice_prox];
		
		// excedeu o LTW do ponto
		if(tempo_total > ltw[vertice_prox])
			return false;
			
		if(vertice_prox == 0)
			segundo_zero = true;
	}
	
	// excedeu o janela de tempo
	if(tempo_total > ROUTE_TIME)
		return false;
		
	return true;
}

bool Auxiliary_and_Restrictions::restricao_2_3_6_alguma_rota(int &prioridade, vector<int> nova_rota){
	
	int vertice_atual = 0;
	int vertice_prox = 0;
	int tempo_total = 0;
	int dem_total = 0;
	bool segundo_zero = false;
	
	for(auto it = nova_rota.begin(); !segundo_zero and it != nova_rota.end(); it++){
		
		// tempo_total eh somado ate chegar ao segundo 0 (deposito)
		vertice_atual = *it;
		vertice_prox = *(it + 1);
		tempo_total += dist[vertice_atual][vertice_prox];
		dem_total += dem[vertice_atual];
		
		// excedeu a capacidade
		if(dem_total > CAPACITY)
			return false;
		
		// veiculo chegou antes de ETW do ponto
		if(tempo_total < etw[vertice_prox]){
			
			tempo_total = etw[vertice_prox] + dur[vertice_prox];
			prioridade++;
		}
		
		else{
			
			tempo_total += dur[vertice_prox];
			prioridade += 2;
		}
		
		// excedeu o LTW do ponto
		if(tempo_total > ltw[vertice_prox])
			return false;
			
		if(vertice_prox == 0)
			segundo_zero = true;
	}
	
	// excedeu o janela de tempo
	if(tempo_total > ROUTE_TIME)
		return false;
	
	return true;
}

int Auxiliary_and_Restrictions::calcular_tempo_alguma_rota(vector<int> nova_rota){
	
	int vertice_atual = 0;
	int vertice_prox = 0;
	int tempo_rota_atual = 0;
	bool segundo_zero = false;
	
	for(auto it = nova_rota.begin(); !segundo_zero and it != nova_rota.end(); it++){
		
		// tempo_total eh somado ate chegar ao segundo 0 (deposito)
		vertice_atual = *it;
		vertice_prox = *(it + 1);
		tempo_rota_atual += dist[vertice_atual][vertice_prox];
		
		// veiculo chegou antes de ETW do ponto
		if(tempo_rota_atual < etw[vertice_prox])
		tempo_rota_atual = etw[vertice_prox] + dur[vertice_prox];
			
		else
			tempo_rota_atual += dur[vertice_prox];
			
		if(vertice_prox == 0)
			segundo_zero = true;
	}
	
	return tempo_rota_atual;
}

vector<int> Auxiliary_and_Restrictions::swap_optimization(vector<int> nova_rota){
	
	int tempo_rota_original = calcular_tempo_alguma_rota(nova_rota);
	int tempo_rota_solucao = 0;
	int tempo_rota_temp = 0;
	vector<int> rota_original = nova_rota;
	vector<int> rota_solucao;
	
	int cont = 0;
	
	for(unsigned int i = 1; i < nova_rota.size() - 2; i++){
		
		for(unsigned int j = i + 1; j < nova_rota.size() - 1; j++){
			
			nova_rota = rota_original;
			
			// troca 2 elementos de posicao do vector
			iter_swap(nova_rota.begin() + i, nova_rota.begin() + j);
			
			if(!restricao_1_2_3_6_alguma_rota(nova_rota)){
				
				cont++;
				
				nova_rota = rota_original;
				continue;
			}
			
			tempo_rota_temp = calcular_tempo_alguma_rota(nova_rota);
			
			if(rota_solucao.empty()){
				
				rota_solucao = nova_rota;
				tempo_rota_solucao = tempo_rota_temp;
			}
			
			else{
				
				if(tempo_rota_temp < tempo_rota_solucao){
					
					rota_solucao = nova_rota;
					tempo_rota_solucao = tempo_rota_temp;
				}
			}
		}
	}
	
	nova_rota = rota_original;
	
	if((tempo_rota_solucao > 0) and (tempo_rota_solucao <= ROUTE_TIME) and (tempo_rota_solucao < tempo_rota_original))
		return rota_solucao;
		
	return rota_original;
}

void Auxiliary_and_Restrictions::_2_opt(vector<int> &rota){
	
	int custo_antes = calcular_tempo_alguma_rota(rota);
	
	for(unsigned int v1 = 1; v1 < rota.size() - 2; v1++){
		
		for(unsigned v2 = v1 + 2; v2 <= rota.size() - 1; v2++){
			
			vector<int> nova_rota = rota;
			
			reverse(begin(nova_rota) + v1 + 1, begin(nova_rota) + v2);
			
			if(restricao_1_2_3_6_alguma_rota(nova_rota)){
				
				int custo_atual = calcular_tempo_alguma_rota(nova_rota);
				
				if(custo_atual < custo_antes)
					rota = nova_rota;
			}
		}
	}
}

// #############################################################

class Vehicles : public Auxiliary_and_Restrictions{
	
	private:
		vector<int> *rota;
		
	public:
		Vehicles(Auxiliary_and_Restrictions *auxiliar_restricoes, int qtd_veiculos);
		~Vehicles();
		bool rota_vazia(int id_veiculo);
		void mostrar_rota(int id_veiculo);
		int calcular_tempo(int id_veiculo);
		void atualizar_rota(vector<int> nova_solucao, int id_veiculo) { rota[id_veiculo] = nova_solucao; };
		vector<int> get_rota(int id_veiculo) { return rota[id_veiculo]; };
		void inserir_pedido_inicial(vector<pair<int,int>> &pedidos, int id_veiculo);
		vector<int> cheapest_insertion(vector<int> rota_modificada, int id_veiculo);
		void redimensionar_rotas(int &qtd_rotas);
		void gerar_arquivo_rota(int qtd_veiculos);
};

Vehicles::Vehicles(Auxiliary_and_Restrictions *auxiliar_restricoes, int qtd_veiculos) : Auxiliary_and_Restrictions(*auxiliar_restricoes){
	
	rota = new vector<int>[qtd_veiculos];
}

Vehicles::~Vehicles(){
	
	delete[] rota;
}

bool Vehicles::rota_vazia(int id_veiculo){
	
	if(rota[id_veiculo].empty())
		return true;
	
	else
		return false;
}

void Vehicles::mostrar_rota(int id_veiculo){
	
	for(auto it = rota[id_veiculo].begin(); it != rota[id_veiculo].end(); it++){
		
		if(it + 1 == rota[id_veiculo].end())
			cout << *it;
			
		else
			cout << *it << " -> ";
	}
	
	cout << endl;
}

int Vehicles::calcular_tempo(int id_veiculo){
	
	int vertice_atual = 0;
	int vertice_prox = 0;
	int tempo_rota_atual = 0;
	bool segundo_zero = false;
	
	for(auto it = rota[id_veiculo].begin(); !segundo_zero and it != rota[id_veiculo].end(); it++){
		
		// tempo_total eh somado ate chegar ao segundo 0 (deposito)
		vertice_atual = *it;
		vertice_prox = *(it + 1);
		tempo_rota_atual += dist[vertice_atual][vertice_prox];
		
		// veiculo chegou antes de ETW do ponto
		if(tempo_rota_atual < etw[vertice_prox])
		tempo_rota_atual = etw[vertice_prox] + dur[vertice_prox];
			
		else
			tempo_rota_atual += dur[vertice_prox];
			
		if(vertice_prox == 0)
			segundo_zero = true;
	}
	
	return tempo_rota_atual;
}

void Vehicles::inserir_pedido_inicial(vector<pair<int,int>> &pedidos, int id_veiculo){
	
	rota[id_veiculo].push_back(pedidos.begin()->first);
	rota[id_veiculo].push_back(pedidos.begin()->second);
	rota[id_veiculo].insert(rota[id_veiculo].begin(), 0);
	rota[id_veiculo].push_back(0);
	
	visitado[pedidos.begin()->first] = true;
	visitado[pedidos.begin()->second] = true;
	visitado[0] = true;
	
	pedidos.erase(pedidos.begin());
}

vector<int> Vehicles::cheapest_insertion(vector<int> rota_modificada, int id_veiculo){
	
	int custo = 0;
	int melhor_custo = 0;
	int prioridade = 0;
	int lixo = 0;
	int melhor_prioridade = 0;
	int numero_pedidos = (SIZE - 1) / 2;
	rota_modificada = rota[id_veiculo];
	vector<int> rota_com_ponto_de_coleta;
	vector<int> rota_nova;
	int ponto_antes = INVALIDO;
	int ponto_inserido = INVALIDO;
	int ponto_depois = INVALIDO;
	
	// analisa se um ponto de coleta pode ser inserido em um ponto especifico da rota
	for(unsigned int i = 1; i < rota_modificada.size(); i++){
		
		// para cada ponto de coleta nao visitado, testa se eh possivel adiciona-lo a rota
		for(int j = 1; j <= numero_pedidos; j++){
			
			rota_modificada = rota[id_veiculo];
			custo = 0;
			prioridade = 0;
			
			if(!visitado[j]){
				
				// "simula" a insercao do ponto de coleta na rota
				rota_modificada.insert(rota_modificada.begin() + i, j);
				
				// se alguma restricao falhar, o ponto de coleta nao pode ser inserido na posicao analisada
				if(!restricao_2_3_6_alguma_rota(lixo, rota_modificada))
					continue;
					
				// calcula o custo, baseado na heuristica de insercao mais barata
				ponto_antes = rota_modificada[i - 1];
				ponto_inserido = rota_modificada[i];
				ponto_depois = rota_modificada[i + 1];
				custo = dist[ponto_antes][ponto_inserido] + 
						dist[ponto_inserido][ponto_depois] - 
						dist[ponto_antes][ponto_depois];
						
				rota_com_ponto_de_coleta = rota_modificada;
				
				// ponto de entrega correspondente ao ponto de coleta analisado
				int k = j + numero_pedidos;
				
				// baseado na rota gerada pela insercao do ponto de coleta...
				// analisa se o ponto de entrega correspondente pode ser inserido em um ponto especifico da rota (depois do ponto de coleta)
				for(unsigned int x = i + 1; x < rota_modificada.size(); x++){
					
					prioridade = 0;
					
					// "simula" a insercao do ponto de entrega na rota
					rota_modificada.insert(rota_modificada.begin() + x, k);
					
					// se alguma restricao falhar, o ponto de entrega nao pode ser inserido na posicao analisada
					if(!restricao_2_3_6_alguma_rota(prioridade, rota_modificada)){
						
						rota_modificada = rota_com_ponto_de_coleta;
						continue;
					}
					
					// calcula o custo, baseado na heuristica de insercao mais barata
					ponto_antes = rota_modificada[x - 1];
					ponto_inserido = rota_modificada[x];
					ponto_depois = rota_modificada[x + 1];
					custo += dist[ponto_antes][ponto_inserido] + 
							 dist[ponto_inserido][ponto_depois] - 
							 dist[ponto_antes][ponto_depois];
							 
					if(rota_nova.empty()){
						
						rota_nova = rota_modificada;
						melhor_custo = custo;
						melhor_prioridade = prioridade;
					}
					
					else if(prioridade >= melhor_prioridade and custo < melhor_custo){
						
						rota_nova = rota_modificada;
						melhor_custo = custo;
						melhor_prioridade = prioridade;
					}
					
					rota_modificada = rota_com_ponto_de_coleta;
				}
			}
		}
		
		rota_modificada = rota[id_veiculo];
	}
	
	return rota_nova;
}

void Vehicles::redimensionar_rotas(int &qtd_rotas){
	
	int novo_tamanho = qtd_rotas + 1;
	
	vector<int> *rotas_novas = new vector<int>[novo_tamanho];
	
	for(int i = 0; i < qtd_rotas; i++)
		rotas_novas[i] = rota[i];
		
	delete[] rota;
	
	qtd_rotas++;
	
	rota = new vector<int>[qtd_rotas];
	
	for(int i = 0; i < qtd_rotas; i++)
		rota[i] = rotas_novas[i];
		
	delete[] rotas_novas;
}

void Vehicles::gerar_arquivo_rota(int qtd_veiculos){
	
	ofstream arq_solucao("solucao_gerada.txt");
	
	arq_solucao << "VEICULOS ";
	arq_solucao << qtd_veiculos << '\n';
	arq_solucao << "ROTAS\n";
	
	int soma_size = 0;
	
	for(int i = 0; i < qtd_veiculos; i++){
		
		soma_size += rota[i].size();
		
		arq_solucao << rota[i].size() << " ";
		
		for(unsigned int j = 0; j < rota[i].size(); j++){
			
			if(j == rota[i].size() - 1)
				arq_solucao << rota[i][j];
				
			else
				arq_solucao << rota[i][j] << " ";
		}
		
		arq_solucao << '\n';
	}
	
	arq_solucao << "EOF";
	
	arq_solucao.close();
	
	int tempo_total = 0;
	
	for(int i = 0; i < qtd_veiculos; i++)
		tempo_total += calcular_tempo(i);
		
	cout << qtd_veiculos << " " << tempo_total << " " << (soma_size - (qtd_veiculos * 2)) / 2 << endl;
}
