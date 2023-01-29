#include <iostream>
#include <iomanip>
#include <fstream>
#include <exception>

using namespace std;

#define INVALIDO -1  

// Classe "lines", armazena os dados das 10 primeiras linhas da instancia
class lines{
	
	friend class nodes;	// permite que a classe "nodes" acesse os atributos privados de "lines"
	
	private:
		string NAME;
		string LOCATION;
		string COMMENT;
		string TYPE;
		string st_SIZE;
		string DISTRIBUTION;
		string DEPOT;
		string st_ROUTE_TIME;
		string st_TIME_WINDOW;
		string st_CAPACITY;
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
	
	string texto_antes;
	
	for(int i = 0; i < 10; i++){
		
		getline(arquivo_instancia, texto_antes, ' ');	// leitura ate o 1º espaco
		
		switch(i){	// leitura dos dados. SIZE, ROUTE_TIME, TIME_WINDOW e CAPACITY sao transformados em inteiros
			
			case 0:
				getline(arquivo_instancia, NAME);
				break;
				
			case 1:
				getline(arquivo_instancia, LOCATION);
				break;
				
			case 2:
				getline(arquivo_instancia, COMMENT);	// "COMMENT" é a única string fica com um espaço a esquerda, excecao nao tratada
				break;
				
			case 3:
				getline(arquivo_instancia, TYPE);
				break;
				
			case 4:
				getline(arquivo_instancia, st_SIZE);
				SIZE = stoi(st_SIZE);
				break;
				
			case 5:
				getline(arquivo_instancia, DISTRIBUTION);
				break;
				
			case 6:
				getline(arquivo_instancia, DEPOT);
				break;
				
			case 7:
				getline(arquivo_instancia, st_ROUTE_TIME);
				ROUTE_TIME = stoi(st_ROUTE_TIME);
				break;
				
			case 8:
				getline(arquivo_instancia, st_TIME_WINDOW);
				TIME_WINDOW = stoi(st_TIME_WINDOW);
				break;
				
			case 9:
				getline(arquivo_instancia, st_CAPACITY);
				CAPACITY = stoi(st_CAPACITY);
				break;
		}
	}
	
	getline(arquivo_instancia, texto_antes);	// leitura da linha NODES
	
	return SIZE;
}

// Classe "nodes", armazena os dados de cada vertice do vertice
class nodes{
	
	private:
		int id;
		double lat;
		double longit;
		int dem;
		int etw;
		int ltw;
		int dur;
		int p;
		int d;
		int *dist;
		
	public:
		nodes();
		~nodes();
		void ler_nohs(ifstream& arquivo_instancia, lines *linhas);
		bool precedencia(lines *linhas);
		void distancia_dos_nohs(ifstream& arquivo_instancia, lines *linhas);
};

// Construtor de "nodes"
nodes::nodes(){
	
	id = INVALIDO;
	lat = INVALIDO;
	longit = INVALIDO;
	dem = 0;
	etw = INVALIDO;
	ltw = INVALIDO;
	dur = INVALIDO;
	p = INVALIDO;
	d = INVALIDO;
	dist = NULL;
}

// Destrutor de "nodes", desaloca o ponteiro das distancias do vertices
nodes::~nodes(){
	
	delete[] dist;
}

// Funcao para ler todos os vertices do grafo
void nodes::ler_nohs(ifstream& arquivo_instancia, lines *linhas){
	
	for(int i = 0; i < 9; i++){
		
		switch(i){	// leitura dos dados
			
			case 0:
				arquivo_instancia >> id;
				break;
				
			case 1:
				arquivo_instancia >> lat;
				break;
				
			case 2:
				arquivo_instancia >> longit;
				break;
				
			case 3:
				arquivo_instancia >> dem;
				if(dem > linhas->CAPACITY){
					
					cout << "O vertice de id igual a " << id << " possui capacidade superior ao esperado!" << endl;
					cout << "Capacidade maxima esperada: " << linhas->CAPACITY << endl;
					cout << "Capacidade do vertice encontrada: " << dem << endl;
					throw runtime_error("ERRO CAPACIDADE - capacidade de demanda do vertice ultrapassada!");
				}
				break;
				
			case 4:
				arquivo_instancia >> etw;
				break;
				
			case 5:
				arquivo_instancia >> ltw;
				if(ltw > linhas->ROUTE_TIME){
					
					cout << "O vertice de id igual a " << id << " possui a janela de tempo superior ao esperado!" << endl;
					cout << "Janela de tempo maxima esperada: " << linhas->ROUTE_TIME << endl;
					cout << "Janela de tempo maxima do vertice encontrada: " << ltw << endl;
					throw runtime_error("ERRO JANELA DE TEMPO - janela de tempo do vertice ultrapassada!");
				}
				/*if(id != 0 and ltw - etw > linhas->TIME_WINDOW){
					
					cout << "O vertice de id igual a " << id << " possui a largura da janela de tempo superior ao esperado!" << endl;
					cout << "Largura da janela de tempo esperada: " << linhas->TIME_WINDOW << endl;
					cout << "Largura da janela de tempo do vertice encontrada: " << ltw - etw << endl;
					throw runtime_error("ERRO LARGURA JANELA DE TEMPO - largura da janela de tempo do vertice ultrapassada!");
				}*/
				break;
				
			case 6:
				arquivo_instancia >> dur;
				break;
				
			case 7:
				arquivo_instancia >> p;
				break;
				
			case 8:
				arquivo_instancia >> d;
				break;
		}
	}
}

// Funcao para fins de integridade por 'p' e 'd'
bool nodes::precedencia(lines *linhas){
	
	int aux_d = 0;
	int aux_p = 0;
	
	if(dem > 0){	// o vertice analisado eh de coleta
		
		aux_d = id + ((linhas->SIZE - 1) / 2);	// valor esperado
		aux_p = 0;
	}
	
	if(dem < 0){	// o vertice analisado eh de entrega
		
		aux_d = 0;
		aux_p = id - ((linhas->SIZE - 1) / 2);	// valor esperado
	}
	
	if(p == 0 and d == 0)	// deposito unico
		return true;
		
	else if(p == 0 and d != 0){	// analise do par de entrega 'd'
		
		if(d == aux_d)
			return true;
			
		else{
			
			cout << "O vertice de id igual a " << id << " possui um erro de precedencia!" << endl;
			cout << "Vertice de entrega esperado: " << aux_d << endl;
			cout << "Vertice de entrega encontrado: " << d << endl;
			throw runtime_error("ERRO PRECEDENCIA - erro de integridade!");
		}
	}
	
	else if(p != 0 and d == 0){	// analise do par de entrega 'd'
		
		if(p == aux_p)
			return true;
			
		else{
			
			cout << "O vertice de id igual a " << id << " possui um erro de precedencia!" << endl;
			cout << "Vertice de coleta esperado: " << aux_p << endl;
			cout << "Vertice de coleta encontrado: " << p << endl;
			throw runtime_error("ERRO PRECEDENCIA - erro de integridade!");
		}
	}
	
	else{	// caso em que ambos 'p' e 'd' sao diferentes de 0 (caso raro, nunca deveria acontecer)
		
		cout << "O vertice de id igual a " << id << " possui um erro de precedencia!" << endl;
		cout << "Vertice de entrega esperado: " << aux_d << endl;
		cout << "Vertice de entrega encontrado: " << d << endl;
		cout << "Vertice de coleta esperado: " << aux_p << endl;
		cout << "Vertice de coleta encontrado: " << p << endl;
		throw runtime_error("ERRO PRECEDENCIA - erro de integridade!");
	}
}

// Funcao para leitura das distancias dos vertices
void nodes::distancia_dos_nohs(ifstream& arquivo_instancia, lines *linhas){
	
	dist = new int[linhas->SIZE];
	
	for(int i = 0; i < linhas->SIZE; i++)
		arquivo_instancia >> dist[i];
}
