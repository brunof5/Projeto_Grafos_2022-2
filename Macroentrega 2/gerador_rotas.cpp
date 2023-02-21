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
		
		Lines *linhas = new Lines;
		
		int numero_vertices = linhas->ler_10_primeiras_linhas(arquivo_instancia);
		
		Nodes *nohs = new Nodes(linhas);
		bool integridade = true;
		
		try{
			
			nohs->ler_nohs(arquivo_instancia);
			nohs->precedencia();
		}
		catch(exception& e){
			
			cout << e.what();
			integridade = false;
		}
		
		if(integridade){
			
			nohs->distancia_dos_nohs(arquivo_instancia);
			
			cout << endl << "Leitura realizada com sucesso !!!" << endl;
			
			Auxiliary_and_Restrictions *auxiliar_restricoes = new Auxiliary_and_Restrictions(nohs);
			
			int qtd_veiculos = 1;
			Vehicles *veiculos = new Vehicles(auxiliar_restricoes, qtd_veiculos);
			
			vector<pair<int,int>> pedidos;
			pair<int,int> pacote_atual;
			
			int qtd_pedidos = (numero_vertices - 1) / 2;
			for(int i = 0; i < qtd_pedidos; i++)
				pedidos.push_back(make_pair(i + 1, ((i + 1) + qtd_pedidos)));
				
			int id_rota_atual = 0;
			
			while(true){
				
				vector<int> nova_rota;
				
				while(!pedidos.empty()){
					
					if(veiculos->rota_vazia(id_rota_atual)){
						
						veiculos->inserir_pedido_inicial(pedidos, id_rota_atual);
						nova_rota = veiculos->get_rota(id_rota_atual);
					}
					
					vector<int> temporaria_rota = nova_rota;
					
					nova_rota = veiculos->cheapest_insertion(nova_rota, id_rota_atual);
					
					// se a nota rota gerada eh viavel
					if(!nova_rota.empty()){
						
						nova_rota = auxiliar_restricoes->swap_optimization(nova_rota);
						auxiliar_restricoes->_2_opt(nova_rota);
						
						pacote_atual = auxiliar_restricoes->retornar_pacote(nova_rota);
						veiculos->atualizar_rota(nova_rota, id_rota_atual);
						auxiliar_restricoes->marcar_nohs_visitados_remover_pacote(pedidos, pacote_atual);
					}
					
					// se nao eh possivel melhorar a nova rota
					else if(!pedidos.empty()){
						
						veiculos->redimensionar_rotas(qtd_veiculos);
						veiculos->atualizar_rota(temporaria_rota, id_rota_atual);
						id_rota_atual++;
					}
				}
				
				if(pedidos.empty())
					break;
			}
			
			veiculos->gerar_arquivo_rota(qtd_veiculos);
			
			cout << endl << "Rotas geradas com sucesso !!!" << endl;
			
			delete veiculos;
		}
		
		if(!integridade)
			delete nohs;
			
		arquivo_instancia.close();
	}
	
	else
		cout << "Instancia nao encontrada! Verifique se a pasta \"instances\" esta na mesma pasta que o programa, ou se digitou a instancia corretamente.";
		
	return 0;
}
