CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
LDFLAGS = 

TARGET = run_grafo

SRCS = main.cpp grafo.cpp unionFind.cpp
OBJS = $(SRCS:.cpp=.o)

HEADERS = grafo.h unionFind.h

# ==========================================
# Configuracao Automatica do Python
# ==========================================
# Tenta obter as flags automaticamente. 
# Se der erro, verifique se 'python3-dev' ou 'python3-devel' esta instalado.

PYTHON_CFLAGS := $(shell python3-config --cflags)

PYTHON_LDFLAGS := $(shell python3-config --ldflags --embed 2>/dev/null || python3-config --ldflags)

CXXFLAGS += $(PYTHON_CFLAGS)
LDFLAGS += $(PYTHON_LDFLAGS)

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	@echo "Linkando o executavel..."
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)
	@echo "Compilacao concluida com sucesso!"

%.o: %.cpp $(HEADERS)
	@echo "Compilando $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	@echo "Iniciando o programa..."
	@# O '.' adiciona o diretorio atual ao PYTHONPATH para achar o visualizacao.py
	@export PYTHONPATH=.:$$PYTHONPATH && ./$(TARGET)

clean:
	@echo "Limpando arquivos temporarios..."
	rm -f $(OBJS) $(TARGET)