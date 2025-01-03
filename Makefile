# Nome do programa
TARGET_DIR = output
TARGET = $(TARGET_DIR)/main

# Fonte do programa
SRC_DIR = src
INCLUDE_DIR = include
SOURCES = $(SRC_DIR)/main/main.c  $(SRC_DIR)/login/login.c  $(SRC_DIR)/admin/admin.c $(SRC_DIR)/admin/medicos.c $(SRC_DIR)/admin/recepcionista.c $(SRC_DIR)/medico/prontuario.c $(SRC_DIR)/financeiro/financeiro_inicial.c $(SRC_DIR)/recepcionista/consulta.c

# Compilador e flags
CC = gcc
# CFLAGS = `pkg-config --cflags gtk+-3.0` -I$(INCLUDE_DIR)
# LDFLAGS = `pkg-config --libs gtk+-3.0`

# Regra padrão
all: $(TARGET)

# Como compilar o programa
$(TARGET): $(SOURCES) | $(TARGET_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Cria o diretório de saída, se não existir
.PHONY: $(TARGET_DIR)
$(TARGET_DIR):
	mkdir -p $(TARGET_DIR)

# Limpar arquivos gerados
clean:
	rm -f $(TARGET)

# Executar o programa na pasta output
run: $(TARGET)
	cd $(TARGET_DIR) && ./main
