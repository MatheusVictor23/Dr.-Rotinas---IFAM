# Dr. Rotinas

Sistema de gerenciamento para clínicas, com o objetivo de tornar a aplicação mais eficiente e rápida utilizando estrutura de dados, linguagem C e arquivos binários.

### Equipe:
- Alysson Gabriel
- Larissa Souza
- Marcelo Vitor
- Matheus Victor
- Silas Salan
  
## Para compilar e executar o projeto (Necessário ter o compilador gcc na máquina)

1. Abrir o PowerShell na pasta raíz do projeto

2. Executar o script de compilação
```bash
.\compilar.ps1
```

3. Caso o arquivo seja impedido de executar por segurança do windows:
```bash
powershell -ExecutionPolicy Bypass -File .\compilar.ps1
```
---

# Documentação do Projeto **Dr. Rotinas - IFAM**

## Descrição do Projeto

O **Dr. Rotinas - IFAM** é um sistema de gerenciamento de clínica desenvolvido como parte de um projeto acadêmico. Ele foi criado para auxiliar na gestão de uma clínica, oferecendo funcionalidades como autenticação de usuários, gerenciamento de médicos, prontuários de pacientes, solicitação de exames, encaminhamentos e muito mais. O sistema é desenvolvido em linguagem C e utiliza estruturas de dados como listas encadeadas para armazenar informações de forma dinâmica e eficiente.

O projeto é modular, com diferentes tipos de usuários (admin, médico e recepcionista), cada um com permissões e funcionalidades específicas. Os dados são armazenados em arquivos binários para persistência.

---

## Funcionalidades Principais

1. **Autenticação de Usuários**:
    - Login com diferentes cargos (admin, médico, recepcionista).
    - Recuperação de senha com verificação de CPF.
2. **Gerenciamento de Médicos**:
    - Cadastro de médicos com informações como nome, CPF, telefone e especialidade.
    - Listagem e exclusão de médicos.
    - Geração automática de credenciais (usuário e senha) para médicos.
3. **Prontuário Eletrônico**:
    - Cadastro de prontuários de pacientes com informações como nome, data de nascimento, sexo, peso, altura e observações.
    - Visualização de prontuários.
4. **Solicitação de Exames**:
    - Solicitação de exames para pacientes, com informações como tipo de exame, caráter da requisição (eletiva, urgência, emergência), suspeita clínica e observações.
    - Visualização de exames solicitados.
5. **Encaminhamentos**:
    - Solicitação de encaminhamentos para pacientes, com informações como motivo, destino e observações.
    - Visualização de encaminhamentos.
6. **Interface de Menu Dinâmico**:
    - Menus interativos com navegação por setas e seleção por Enter.
    - Cores no console para melhorar a experiência do usuário.

---

## Estrutura do Projeto

O projeto é organizado em módulos, cada um responsável por uma funcionalidade específica. Abaixo está a estrutura geral do código:

### Módulos Principais

1. **Login**:
    - Responsável pela autenticação de usuários.
    - Verifica credenciais e redireciona para o menu correspondente ao cargo do usuário.
2. **Admin**:
    - Permite o gerenciamento de médicos (cadastro, listagem, exclusão).
    - Gera credenciais automáticas para novos médicos.
3. **Médico**:
    - Permite o gerenciamento de prontuários de pacientes.
    - Solicitação de exames e encaminhamentos.
4. **Recepcionista**:
    - Funcionalidades específicas para recepcionistas (a serem implementadas).
5. **Prontuário**:
    - Estrutura de dados para armazenar informações de pacientes, exames e encaminhamentos.
    - Utiliza listas encadeadas para armazenar múltiplos prontuários, exames e encaminhamentos.
6. **Arquivos Binários**:
    - Armazenamento persistente de dados (usuários, médicos, prontuários, etc.).
    - Leitura e escrita em arquivos binários para garantir a persistência dos dados.

---

## Estrutura de Dados

O projeto utiliza várias estruturas de dados para armazenar informações:

1. **Usuário**:
    
    
    ```c
    typedef struct _user {
        char username[50];
        char senha[50];
        char cargo[20];
        char cpf[15];
    } Usuario;
    ```
    
2. **Médico**:
    
    
    ```c
    typedef struct Medico {
        int id;
        char nome[50];
        char telefone[15];
        char cpf[15];
        char especialidade[30];
        char cargo[20];
        char username[50];
        char senha[50];
        struct Medico *proximo; // Ponteiro para o próximo nó da lista
    } Medico;
    ```
    
3. **Prontuário**:
    
    
    ```c
    typedef struct {
        char nome[100];
        char datanasc[20];
        char sexo[20];
        char peso[10];
        char altura[10];
        char observacao[100];
    } ProntuarioPaciente;
    ```
    
4. **Exame**:
    
    
    ```c
    typedef struct {
        char caraterRequisicao[20];
        char suspeitaClinica[100];
        char tipoExame[30];
        char Observacoes[80];
    } SolicitarExame;
    ```
    
5. **Encaminhamento**:
    
    
    ```c
    typedef struct {
        char motivo[100];
        char destino[100];
        char observacoes[80];
    } Encaminhamento;
    ```
    
6. **Listas Encadeadas**:
    - Listas encadeadas são utilizadas para armazenar prontuários, exames e encaminhamentos de forma dinâmica.

---

## Como Executar o Projeto

### Pré-requisitos

1. **Compilador C**: Instale um compilador C, como GCC.
2. **Sistema Operacional**: O projeto foi desenvolvido para ser executado em Windows (com suporte a `conio.h`) e Linux (com suporte a `termios.h`).

### Passos para Compilar e Executar

1. **Clone o Repositório**:
    
    
    ```bash
    git clone https://github.com/MatheusVictor23/Dr.-Rotinas---IFAM.git
    cd Dr.-Rotinas---IFAM
    ```
    
2. **Compile o Projeto**:
    
    
    ```bash
    gcc -o dr_rotinas main.c include/*.c -Iinclude
    ```
    
3. **Execute o Programa**:
    
    
    ```bash
    ./dr_rotinas
    ```
    
4. **Siga as Instruções no Console**:
    - Escolha o cargo (admin, médico, recepcionista).
    - Utilize as credenciais de teste fornecidas no código ou crie novos usuários.

---

## Exemplos de Uso

### Login

1. Ao iniciar o programa, o usuário escolhe o cargo (admin, médico, recepcionista).
2. Insere as credenciais (usuário e senha).
3. Se as credenciais estiverem corretas, o usuário é redirecionado para o menu correspondente.

### Cadastro de Médico (Admin)

1. No menu do admin, selecione "Cadastrar Médico".
2. Insira as informações do médico (nome, CPF, telefone, especialidade).
3. O sistema gera automaticamente um usuário e senha para o médico.

### Prontuário Eletrônico (Médico)

1. No menu do médico, selecione "Prontuário Eletrônico".
2. Escolha entre cadastrar um novo prontuário ou visualizar um existente.
3. Insira ou consulte as informações do paciente.

### Solicitação de Exame (Médico)

1. No menu do médico, selecione "Exames".
2. Escolha solicitar um novo exame ou visualizar exames existentes.
3. Insira as informações do exame (tipo, caráter, suspeita clínica, observações).

4. O arquivo será criado na pasta output. Navegar até a pasta e executar o programa
```bash
cd .\output\

.\clinica.exe
```
