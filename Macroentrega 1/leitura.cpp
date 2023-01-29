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

int main(){
	
	string nome_instancia;
	
	cout << "Digite o nome da instancia (sem a extensao): ";
	cin >> nome_instancia;
	
	ifstream arquivo_instancia("instances/" + nome_instancia + ".txt");
	
	if(arquivo_instancia){
		
		lines *linhas = new lines;	// "linhas" ira armazenar os valores das 10 primeiras linhas da instancia
		
		int numero_vertices = linhas->ler_10_primeiras_linhas(arquivo_instancia);
		
		nodes *nohs = new nodes[numero_vertices];	// cada "noh" armazena os valores de cada vertice do grafo
		bool integridade = true;	// caso ocorra algum erro de restricao de leitura, "integridade" impede a continuidade do programa
		
		try{
			
			for(int i = 0; i < numero_vertices; i++){
				
				nohs[i].ler_nohs(arquivo_instancia, linhas);	// leitura de todos os vertices do grafo
				nohs[i].precedencia(linhas);	// "precedencia" faz a analise de integridade dos pares 'p' e 'd'
			}
		}
		catch(exception& e){
			
			cout << e.what();
			integridade = false;
		}
		
		if(integridade){
			
			string texto;
			arquivo_instancia >> texto;	// leitura da linha EDGES
			
			for(int i = 0; i < numero_vertices; i++)
				nohs[i].distancia_dos_nohs(arquivo_instancia, linhas);	// leitura das distancias dos vertices de um para o outro
		}
		
		delete linhas;
		delete[] nohs;
		
		arquivo_instancia.close();
	}
	
	else
		cout << "Instancia nao encontrada! Verifique se a pasta \"instances\" esta na mesma pasta que o programa, ou se digitou a instancia corretamente.";
	
	return 0;
}
