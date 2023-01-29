# Macroentrega 1: Criação de funções para leitura dos dados / Proposição de um algoritmo para a resolução do problema

## Objetivos
1. Implementação das estruturas de dados.

2. Implementação de função(ões) para a leitura de dados.

3. Implementação de função de verificação de solução.
  Gere uma solução aleatória. Passando tal solução como parâmetro,
crie uma função que verifica se as restrições descritas acima (janelas
de tempo, precedência e capacidade do veículo) são respeitadas
nessa solução.

4. Crie um documento, com no máximo 2 páginas, que explique em detalhes o algoritmo delineado. Apresente um pseudocódigo.

## Base de Dados

Trinta instâncias foram selecionadas da literatura para a seção de experimentos computacionais deste trabalho.

Todas as instâncias terminam com uma linha contendo a palavra EOF.

Cada instância é dividida em 3 partes principais.

###### Primeiras 10 linhas

Cada arquivo contém informações gerais sobre a instância:

* NAME: identificação de instância única
* LOCATION: cidade onde foi gerado
* COMMENT: uma referência geral
* TYPE: o tipo da instância
* SIZE: número de vértices incluindo o depósito
* DISTRIBUTION: distribuição a qual a instância foi gerada
* DEPOT: tipo de localização do depósito: 'central' ou 'aleatório'
* ROUTE-TIME: horizonte de tempo da roteirização
* TIME-WINDOW: largura da janela de tempo
* CAPACITY: capacidade máxima do veículo

######  Após a linha com a palavra "NODES"

É seguida por uma quantidade SIZE de linhas, contendo as informações completas de cada ponto (vértice) no arquivo de instância.
Para cada linha, existem 9 campos:

* id: o identificador único do ponto (o ponto 0 é o depósito único);
* lat: latitude deste local;
* long: longitude deste local;
* dem: a demanda deste nó (dem > 0 para coleta, dem < 0 para entrega);
* etw: tempo mais cedo possível para iniciar o serviço (janela de tempo);
* ltw: última hora possível para iniciar o serviço (janela de tempo);
* dur: a duração do serviço neste local;
* p: o par de coleta se id for uma entrega; e 0 caso contrário;
* d: o par de entrega se id for uma coleta; e 0 caso contrário

O 'p' e 'd' são apenas para fins de integridade. Em todas as instâncias:
* Para um local de coleta id, sua entrega é dada por (id+((SIZE-1)/2)). 
* Para um local de entrega id, sua coleta é dada por (id-((SIZE-1)/2)).

###### Após todos os NODES

Existe uma linha contendo a palavra EDGES seguida de SIZE linhas, cada uma com SIZE valores inteiros separados por um único espaço
em branco. Esses números inteiros representam os tempos de viagem entre cada
par de locais na instância, medidos em minutos e calculados usando o Ferramenta
Open Source Routing Machine (OSRM).

## Funções para leitura dos dados

Primeiramente, foi considerado que valores negativos nunca serão inseridos incorretamente, ou seja, os campos de SIZE, ROUTE-TIME, TIME-WINDOW, CAPACITY, id, etw e dur sempre serão considerados positivos; não foi criado regras de exceção ou detecção de erros para os mesmos.

Regras de exceção e detecção de erros foram realizados para:
* as janelas de tempo, sendo que ltw não pode ultrapassar o valor de ROUTE_TIME;
* demanda do vertice, sendo que uma demanda não pode ultrapassar o valor de CAPACITY;
* integridade com 'p' e 'd', definido pelas fórmulas já citadas.

A estratégia empregada para a leitura dos dados se baseia em leitura de arquivos pela biblioteca fstream + programação orientada a objetos. No programa há 2 classes principais: lines e nodes. Por lines se faz a leitura das 10 primeiras linhas do arquivo e por nodes se faz a leitura de todos os vértices do grafo e as distâncias respectivas de cada vértice para todos, além disso a classe nodes herda os atributos de lines por ser amiga dela, garantindo a privacidade dos atributos do arquivo que não podem ser acessados **diretamente** pela função main.

Os códigos foram disponibilizados nessa macroentrega, para maior detalhamento e entendimento, favor acessá-los.

## Proposição de um algoritmo
  
PDF EM BREVE.
