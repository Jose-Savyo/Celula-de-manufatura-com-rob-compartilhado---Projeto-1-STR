#Célula de Manufatura - Sistemas em Tempo Real (STR)

Este projeto implementa a simulação de uma Célula de Manufatura Automatizada utilizando linguagem C e a biblioteca POSIX Threads (pthread). O objetivo é demonstrar conceitos avançados de computação concorrente, como exclusão mútua, sincronização de tarefas e gerenciamento de buffers compartilhados em um ambiente de missão crítica.
🏗️ Arquitetura do Sistema

O sistema é composto por quatro agentes principais que interagem de forma assíncrona:
Componente	Função	Lógica de Tempo
Máquinas (M1 e M2)	Processam peças e sinalizam prontidão via sensores lógicos.	Variável (rand() % 3 + 1).
Robô (Coordenador)	Gerencia o transporte entre as máquinas e a esteira de saída (buffer).	Sequencial com prioridade para M1.
Buffer (Esteira)	Armazenamento temporário com capacidade limitada (2 posições).	Fila Circular (FIFO).
Agente Externo	Consome os itens finalizados, liberando espaço no fluxo produtivo.	Intermitente (rand() % 5 + 5).
🛠️ Mecanismos de Sincronização

A integridade dos dados e o controle do fluxo físico são garantidos por semáforos e exclusão mútua:

    mutex_buffer: Garante que apenas um agente (Robô ou Externo) acesse o buffer por vez, evitando condições de corrida.

    vazio_buffer / cheio_buffer: Controlam a ocupação da esteira, bloqueando o Robô se estiver cheia ou o Agente se estiver vazia.

    sinal_robo: Implementa a espera passiva do Robô, economizando CPU enquanto não há chamados das máquinas.

🚀 Como Executar
Pré-requisitos

    Compilador GCC.

    Ambiente Linux ou WSL (Windows Subsystem for Linux).

Compilação

O projeto utiliza um Makefile para automatizar o build com as flags de otimização e a biblioteca de threads:
Bash

make

Execução
Bash

./celula_manufatura

📊 Monitoramento em Tempo Real

O projeto foi desenvolvido para ser monitorado via htop. Graças à implementação de pthread_setname_np, é possível observar o estado de cada thread individualmente:

    Abra o htop.

    Pressione F4 e filtre por celula.

    Pressione F5 para o modo árvore.

    Observe as threads Maquina_1, Maquina_2, Robo e Agente_Ext transitando entre os estados de execução e espera.

    Nota Técnica: A lógica de retirada do buffer utiliza a operação matemática de módulo para garantir a persistência da fila:
    out=(out+1)(modBUFFER_SIZE)
