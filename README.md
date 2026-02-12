# 🏭 Célula de Manufatura - Sistemas em Tempo Real (STR)

## 📝 Descrição do Projeto
[cite_start]Este projeto consiste na simulação de uma célula de manufatura automatizada desenvolvida em **Linguagem C**, utilizando a biblioteca **POSIX Threads (pthread)** para o gerenciamento de concorrência. [cite_start]O sistema modela o comportamento de duas máquinas de processamento, um robô de transporte e um agente de logística externo, focando na sincronização de tarefas e na integridade de recursos compartilhados em ambiente de tempo real.

---

## 🏗️ Arquitetura de Software
O sistema é composto por quatro agentes independentes que operam em paralelo:

* [cite_start]**Máquinas (M1 e M2):** Atuam como produtores primários, executando tarefas de processamento com tempos variáveis simulados por `rand()`.
* [cite_start]**Robô de Transporte:** Atua como o elemento de coordenação central, responsável por coletar peças finalizadas e depositá-las em um buffer de saída.
* [cite_start]**Buffer de Saída (Esteira):** Estrutura de dados do tipo fila circular (FIFO) com capacidade limitada a 2 posições.
* [cite_start]**Agente Externo:** Atua como o consumidor final, removendo peças do buffer para permitir a continuidade do fluxo produtivo e evitar deadlocks por transbordamento de buffer.

---

## 🔐 Mecanismos de Sincronização e Controle
[cite_start]Para garantir o determinismo e evitar condições de corrida (*Race Conditions*), foram implementadas as seguintes primitivas de sincronização:

| Primitiva | Variável no Código | Função |
| :--- | :--- | :--- |
| **Exclusão Mútua** | `mutex_buffer` | [cite_start]Protege o acesso exclusivo às variáveis de índice do buffer (`in` e `out`). |
| **Semáforos de Condição** | `vazio_buffer` e `cheio_buffer` | [cite_start]Controlam a ocupação do buffer, garantindo que o robô não deposite em esteira cheia e o agente não colete de vazia. |
| **Semáforo de Evento** | `sinal_robo` | [cite_start]Implementa a espera passiva do robô, que permanece suspenso até o fim de um processo de máquina. |

### Lógica de Gerenciamento de Fila
[cite_start]A retirada de peças segue rigorosamente a ordem de chegada, implementada através de aritmética modular para o gerenciamento de ponteiros no buffer circular:
$$out = (out + 1) \pmod{BUFFER\_SIZE}$$

---

## 🛠️ Instruções de Build e Execução

### Compilação
[cite_start]O projeto utiliza um **Makefile** para gerenciar as dependências e garantir a inclusão das flags `-Wall` e `-pthread`[cite: 6, 7]:

```bash
make
