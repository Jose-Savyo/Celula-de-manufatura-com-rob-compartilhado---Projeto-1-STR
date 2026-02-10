#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 2      // Capacidade da esteira de saída
#define NUM_MAQUINAS 2     // M1 e M2

// Recursos do Buffer (Seguindo seu modelo de produtor_consumidor.c)
int buffer[BUFFER_SIZE];
int in = 0, out = 0;

// Semáforos e Mutex
sem_t mutex_buffer, cheio_buffer, vazio_buffer;
sem_t sinal_robo;          // Semáforo para acordar o robô

// Sensores das máquinas (0: vazia, 1: com peça pronta)
int peca_na_maquina[NUM_MAQUINAS + 1];

void *task_maquina(void *arg) {
    int id = *(int *)arg;
    char nome[20];
    sprintf(nome, "Maquina_%d", id);
    pthread_setname_np(pthread_self(), nome); // Visibilidade no htop

    while (1) {
        printf("[M%d] Iniciando processamento...\n", id); // A titulo de testes
        // Simula processamento aleatório
        sleep(rand() % 3 + 1); 

        peca_na_maquina[id] = 1; // Ativa o "sensor" de peça pronta
        printf("[M%d] Processamento concluído! Solicitando transporte...\n", id);

        sem_post(&sinal_robo); // Notifica o robô

        // Aguarda o robô coletar a peça antes de reiniciar
        printf("[M%d] Aguardando coleta do robô (Sensor: %d)...\n", id, peca_na_maquina[id]); // A titulo de testes
        while(peca_na_maquina[id] == 1) {
            usleep(100000); 
        }
    }
}

void *task_robo(void *arg) {
    pthread_setname_np(pthread_self(), "Robo");

    while (1) {
        // 1. ESPERA CHAMADO: O robô fica parado até que M1 ou M2 sinalizem
        sem_wait(&sinal_robo); 

        // 2. DECISÃO: Identifica qual máquina atender (Prioridade M1)
        int id_alvo = (peca_na_maquina[1] == 1) ? 1 : 2;

        // 3. COLETA IMEDIATA: O robô vai até a máquina e retira a peça
        printf("[ROBÔ] Coletando peça da M%d. Máquina agora está livre!\n", id_alvo);
        sleep(1); // Tempo de "Pick"
        
        // LIBERA A MÁQUINA: Agora a máquina pode voltar a processar enquanto o robô segura a peça
        peca_na_maquina[id_alvo] = 0; 

        // 4. ESPERA VAGA NO BUFFER: Se a esteira estiver cheia, o robô para aqui com a peça na mão
        printf("[ROBÔ] Com a peça da M%d em mãos. Aguardando vaga no buffer...\n", id_alvo);
        sem_wait(&vazio_buffer); 

        // 5. DEPÓSITO NO BUFFER (Seção Crítica)
        sem_wait(&mutex_buffer);
        buffer[in] = id_alvo;
        in = (in + 1) % BUFFER_SIZE;
        sem_post(&mutex_buffer);

        sem_post(&cheio_buffer); // Sinaliza para o Agente Externo
        printf("[ROBÔ] Peça da M%d entregue no buffer. Voltando para posição de repouso.\n", id_alvo);
        
        // Somente aqui, ao chegar no fim do while, ele poderá atender um novo sinal_robo.
    }
}

void *task_agente_externo(void *arg) {
    // Define o nome para identificação clara no seu htop
    pthread_setname_np(pthread_self(), "Agente_Ext");

    while (1) {
        // O agente externo não está na célula o tempo todo. 
        // Ele "passa" para coletar as peças em intervalos (ex: a cada 8 segundos)
        sleep(rand() % 5 + 5); 

        printf("[EXTERNO] Cheguei na célula. Verificando peças na esteira...\n");

        // 1. ESPERA PEÇA: O agente dorme até que o Robô coloque algo no buffer
        sem_wait(&cheio_buffer);

        // 2. SEÇÃO CRÍTICA: Acesso exclusivo para retirar do buffer
        sem_wait(&mutex_buffer);
        
        int item = buffer[out];
        printf("[EXTERNO] Coletando peça da M%d...\n", item);
        out = (out + 1) % BUFFER_SIZE; // Garante que o agente externo pegue a peça que chegou primeiro
        
        sem_post(&mutex_buffer);

        // 3. LIBERA VAGA: Informa ao sistema que o buffer tem um espaço vazio
        // Isso é o que permite o Robô "acordar" se ele estiver esperando com uma peça na mão.
        sem_post(&vazio_buffer);

        printf("[EXTERNO] Peça removida com sucesso. Retornando ao estoque.\n");
    }
}

int main() {
    srand(time(NULL)); // Semente para os tempos aleatórios

    // 1. Inicialização dos Semáforos
    sem_init(&mutex_buffer, 0, 1);
    sem_init(&cheio_buffer, 0, 0);
    sem_init(&vazio_buffer, 0, BUFFER_SIZE);
    sem_init(&sinal_robo, 0, 0); // O robô começa esperando

    pthread_t maquinas[NUM_MAQUINAS];
    pthread_t t_robo, t_externo;
    int ids[NUM_MAQUINAS];

    // 2. Criação das Máquinas (IDs 1 e 2)
    for (int i = 0; i < NUM_MAQUINAS; i++) {
        ids[i] = i + 1;
        pthread_create(&maquinas[i], NULL, task_maquina, &ids[i]);
    }

    // 3. Criação do Robô e do Agente Externo
    pthread_create(&t_robo, NULL, task_robo, NULL);
    pthread_create(&t_externo, NULL, task_agente_externo, NULL);

    // 4. Aguardar as threads (Join)
    // Em sistemas de tempo real, elas rodam para sempre, então o join as manterá vivas.
    for (int i = 0; i < NUM_MAQUINAS; i++) {
        pthread_join(maquinas[i], NULL);
    }
    pthread_join(t_robo, NULL);
    pthread_join(t_externo, NULL);

    // 5. Limpeza (Só acontece se o programa for encerrado)
    sem_destroy(&mutex_buffer);
    sem_destroy(&cheio_buffer);
    sem_destroy(&vazio_buffer);
    sem_destroy(&sinal_robo);

    return 0;
}

