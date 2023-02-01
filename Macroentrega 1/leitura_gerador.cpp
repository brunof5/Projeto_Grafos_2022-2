/* Universidade Federal de Lavras - UFLA
 * Instituto de Ciencias Exatas e Tecnologicas
 * Departamento de Ciencia da Computacao
 * GCC218 - Algoritmos em Grafos
 * Prof. Mayron Moreira
 * Aluno: Bruno Crespo Ferreira - 202120489 - 10A
 * 
 * Projeto Pratico - 2022/2
 */

#include "classes.h"
#include "gerar_solucao_aleatoria.h"

int main(){
	
	string nome_instancia;
	
	cout << "Digite o nome da instancia (sem a extensao): ";
	cin >> nome_instancia;
	
	ifstream arquivo_instancia("instances/" + nome_instancia + ".txt");
	
	if(arquivo_instancia){
		
		// "linhas" ira armazenar os valores das 10 primeiras linhas da instancia
		lines *linhas = new lines;
		int numero_vertices = linhas->ler_10_primeiras_linhas(arquivo_instancia);
		// leitura da linha NODES
		string texto;
		arquivo_instancia >> texto;
		
		// "nohs" armazena os valores de cada vertice do grafo
		nodes *nohs = new nodes(linhas);
		// caso ocorra algum erro de restricao de leitura, "integridade" impede a continuidade do programa
		bool integridade = true;
		
		try{
			
			// leitura de todos os vertices do grafo
			nohs->ler_nohs(arquivo_instancia, linhas);
			// "precedencia" faz a analise de integridade dos pares 'p' e 'd'
			nohs->precedencia(linhas);
		}
		catch(exception& e){
			
			cout << e.what();
			integridade = false;
		}
		
		if(integridade){
			
			// leitura da linha EDGES
			arquivo_instancia >> texto;
			
			// leitura das distancias dos vertices de um para os outros
			nohs->distancia_dos_nohs(arquivo_instancia, linhas);
			
			cout << "Leitura realizado com sucesso !!!" << endl;
			cout << endl;
			
			char op;
			cout << "Deseja gerar uma solucao aleatoria? (S/N): ";
			cin.ignore();
			cin.get(op);
			
			if(op == 'S'){
				
				// gera a solucao aleatoria
				gerar_solucao_aleatoria(numero_vertices);
				
				cout << "Solucao aleatoria gerada com sucesso !!!" << endl;
				cout << endl;
				
				ifstream arq_solucao("solucao_gerada.txt");
				
				if(arq_solucao){
					
					vehicles *veiculos = new vehicles(arq_solucao);
					veiculos->leitura_rota(arq_solucao, linhas);
					
					// verifica as 6 restricoes do problema
					veiculos->restricao_1(arq_solucao, linhas, nohs);
					veiculos->restricao_2(arq_solucao, linhas, nohs);
					veiculos->restricao_3(arq_solucao, nohs);
					//veiculos->restricao_4(arq_solucao, linhas, nohs);
					//veiculos->restricao_5(arq_solucao, linhas, nohs);
					veiculos->restricao_6(arq_solucao, linhas, nohs);
					
					delete veiculos;
					
					arq_solucao.close();
				}
				
				else
					cout << "Erro ao ler o arquivo gerado!" << endl;
			}
		}
		
		delete linhas;
		nohs->destrutor(linhas);
		delete nohs;
		
		arquivo_instancia.close();
	}
	
	else
		cout << "Instancia nao encontrada! Verifique se a pasta \"instances\" esta na mesma pasta que o programa, ou se digitou a instancia corretamente.";
		
	return 0;
}
