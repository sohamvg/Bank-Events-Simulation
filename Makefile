CC = gcc

SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
EXE_DIR = bin
OUT_DIR = output
EXE = qSim

LIBS = -lm # math library

SRC_FILES := $(wildcard $(SRC_DIR)/*)
OBJ_FILES := $(subst $(SRC_DIR),$(OBJ_DIR),$(SRC_FILES:%.c=%.o))

all: $(EXE_DIR)/$(EXE)

run: $(EXE_DIR)/$(EXE)
	./$<

$(EXE_DIR)/$(EXE): $(OBJ_FILES)
	$(CC) -o $@ $^ $(LIBS)

compile: $(OBJ_FILES)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(OBJ_DIR)
	$(CC) -c $< -o $@ -I $(INC_DIR) $(LIBS)

clean:
	rm -rf $(OBJ_DIR)/*