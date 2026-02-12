# 🏭 Célula de Manufatura — Sistemas em Tempo Real (STR)

## 📖 Descrição do Projeto

Este projeto consiste na **simulação de uma célula de manufatura automatizada** desenvolvida em linguagem **C**, utilizando a biblioteca **POSIX Threads (pthread)** para gerenciamento de concorrência.

O sistema modela o comportamento de:

* Duas máquinas de processamento
* Um robô de transporte
* Um agente de logística externo

O foco principal está na **sincronização de tarefas**, **compartilhamento seguro de recursos** e no comportamento de sistemas concorrentes em **ambiente de tempo real**.

---

## 🧠 Arquitetura de Software

O sistema é composto por **quatro agentes independentes**, executados em paralelo por meio de threads:

### 🔧 Máquinas (M1 e M2)

* Atuam como **produtores primários**.
* Executam tarefas de processamento com tempos variáveis.
* Os tempos são simulados utilizando a função `rand()`.

### 🤖 Robô de Transporte

* Atua como **coordenador central** do fluxo produtivo.
* Responsável por:

  * Coletar peças finalizadas das máquinas.
  * Transportá-las até o buffer de saída.

### 📦 Buffer de Saída (Esteira)

* Estrutura de dados do tipo **fila circular (FIFO)**.
* Capacidade limitada a **2 posições**.
* Controla o fluxo entre produção e consumo.

### 🚚 Agente Externo

* Atua como **consumidor final**.
* Remove peças do buffer para:

  * Liberar espaço.
  * Evitar deadlocks por transbordamento.

---

## 🔐 Mecanismos de Sincronização e Controle

Para garantir **determinismo** e evitar **Race Conditions**, foram utilizadas as seguintes primitivas:

### 🔒 Mutex (Exclusão Mútua)

Protege o acesso às variáveis críticas do buffer:

* Índice de entrada (`in`)
* Índice de saída (`out`)

---

### 🚦 Semáforos de Condição

* **`vazio_buffer`** → Controla espaços livres no buffer.
* **`cheio_buffer`** → Controla a quantidade de peças disponíveis.

Garantem que:

* O robô **não deposite** em buffer cheio.
* O agente externo **não remova** de buffer vazio.

---

### 📡 Semáforo de Evento — `sinal_robo`

Implementa a **espera passiva do robô**:

* O robô permanece suspenso.
* Só é ativado quando uma máquina sinaliza **fim de processamento**.

Isso evita polling e desperdício de CPU.

---

## 🔄 Lógica de Gerenciamento de Fila

A retirada de peças segue rigorosamente a **ordem de chegada (FIFO)**.

O controle é feito com **aritmética modular** no buffer circular:

```c
out = (out + 1) % BUFFER_SIZE;
```

Isso permite:

* Reutilização das posições
* Controle eficiente de memória
* Fluxo contínuo de produção

---

## 🛠️ Instruções de Build e Execução

### 📦 Compilação

O projeto utiliza **Makefile** para automação do build, incluindo as flags:

* `-Wall` → Exibição de warnings
* `-pthread` → Suporte a threads POSIX

Execute:

```bash
make
```

---

### ▶️ Execução

Após compilar:

```bash
./celula_manufatura
```

---

## 📊 Monitoramento das Threads

Para análise de escalonamento e estados das threads, utilize o **htop**:

### Passos:

1. Execute o programa:

   ```bash
   ./celula_manufatura
   ```

2. Abra o htop:

   ```bash
   htop
   ```

3. Pressione **F4** e filtre pelo nome do processo.

4. Observe os estados:

* **S** → Sleeping
* **R** → Running

As threads são nomeadas via:

```c
pthread_setname_np();
```

---

## 🎯 Objetivos Acadêmicos

Este projeto demonstra na prática:

* Sincronização entre múltiplas threads
* Problema Produtor × Consumidor
* Uso de semáforos e mutex
* Gerenciamento de buffer circular
* Coordenação de sistemas concorrentes em tempo real

---

## 📚 Tecnologias Utilizadas

* Linguagem **C**
* **POSIX Threads (pthread)**
* **Semáforos POSIX**
* Ambiente **Linux**
* **Makefile**

---

## 👨‍💻 Autor

Desenvolvido para a disciplina de **Sistemas em Tempo Real (STR)**.
