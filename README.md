Célula de Manufatura - Sistemas em Tempo Real (STR)
Descrição do Projeto

Este projeto consiste na simulação de uma célula de manufatura automatizada desenvolvida em linguagem C, utilizando a biblioteca POSIX Threads (pthread) para o gerenciamento de concorrência. O sistema modela o comportamento de duas máquinas de processamento, um robô de transporte e um agente de logística externo, focando na sincronização de tarefas e na integridade de recursos compartilhados em ambiente de tempo real.
Arquitetura de Software

O sistema é composto por quatro agentes independentes que operam em paralelo:

    Máquinas (M1 e M2): Atuam como produtores primários, executando tarefas de processamento com tempos variáveis simulados por rand().

    Robô de Transporte: Atua como o elemento de coordenação central, responsável por coletar peças finalizadas e depositá-las em um buffer de saída.

    Buffer de Saída (Esteira): Estrutura de dados do tipo fila circular (FIFO) com capacidade limitada a 2 posições.

    Agente Externo: Atua como o consumidor final, removendo peças do buffer para permitir a continuidade do fluxo produtivo e evitar deadlocks por transbordamento de buffer.

Mecanismos de Sincronização e Controle

Para garantir o determinismo e evitar condições de corrida (Race Conditions), foram implementadas as seguintes primitivas de sincronização:

    Exclusão Mútua (Mutex): Utilizado para proteger o acesso às variáveis de índice do buffer (in e out).

    Semáforos de Condição (vazio_buffer e cheio_buffer): Controlam a ocupação do buffer, garantindo que o robô não deposite peças em uma esteira cheia e que o agente externo não tente coletar de uma esteira vazia.

    Semáforo de Evento (sinal_robo): Implementa a espera passiva do robô, que permanece em estado de suspensão até que uma sinalização de "fim de processo" seja emitida por uma das máquinas.

Lógica de Gerenciamento de Fila

A retirada de peças segue rigorosamente a ordem de chegada, implementada através de aritmética modular para o gerenciamento de ponteiros no buffer circular:
out=(out+1)(modBUFFER_SIZE)
Instruções de Build e Execução
Compilação

O projeto utiliza um Makefile para gerenciar as dependências e garantir a inclusão das flags -Wall e -pthread durante o processo de build:
Bash

make

Execução e Monitoramento

Para validar o escalonamento das threads e o consumo de recursos, recomenda-se o uso da ferramenta htop durante a execução:

    Execute o binário: ./celula_manufatura.

    No htop, utilize o filtro (F4) para localizar o processo.

    Verifique os estados das threads (S para Sleeping e R para Running) e as nomenclaturas definidas via pthread_setname_np para cada componente da célula.
