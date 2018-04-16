CC=gcc
LINKER=gcc -o
CFLAGS=-Wall -g

GCOVFLAGS=-O0 --coverage -lgcov -Wall -g
LCOV_REPORT=report.info

SRC_DIR=src
INC_DIR=include
BIN_DIR=bin
OBJ_DIR=obj
DOC_DIR=doc
TEST_DIR=tests
GCOV_DIR=gcov

LFLAGS=-I $(INC_DIR)

SRC=$(wildcard $(SRC_DIR)/*.c)
INC=$(wildcard $(INC_DIR)/*.h)
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
MODULES := $(filter-out $(OBJ_DIR)/main.o, $(OBJ)) # all object files excpet main.o

TEST_SRC=$(wildcard $(TEST_DIR)/$(SRC_DIR)/*.c)
TEST_OBJ := $(TEST_SRC:$(TEST_DIR)/$(SRC_DIR)/%.c=$(TEST_DIR)/$(OBJ_DIR)/%.o)
TEST_EXECS := $(TEST_SRC:$(TEST_DIR)/$(SRC_DIR)/%.c=$(TEST_DIR)/$(BIN_DIR)/%)

EXEC=Shell0
GEXEC=$(EXEC).cov

AR_NAME=archive_$(EXEC).tar.gz

all: $(SRC) $(INC) $(BIN_DIR)/$(EXEC) $(MAN)

$(OBJ): $(OBJ_DIR)/%.o : $(SRC_DIR)/%.c $(INC_DIR)/%.h
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully!"
	

$(BIN_DIR)/$(EXEC): $(OBJ)
	$(LINKER) $@ $(LFLAGS) $(OBJ)
	@echo "Linking complete!"

$(GEXEC):
	$(CC) $(GCOVFLAGS) -o $(GCOV_DIR)/$@ -Wall $(LDFLAGS) $(SRC)

tests: $(SRC) $(INC) $(TEST_EXECS)

$(TEST_OBJ): $(TEST_DIR)/$(OBJ_DIR)/%.o : $(TEST_DIR)/$(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully!"

$(TEST_EXECS): $(MODULES) $(TEST_OBJ)
	$(LINKER) $@ $(LFLAGS) $(MODULES) $(TEST_DIR)/$(OBJ_DIR)/$(notdir $@).o -L/usr/lib -lcmocka
	@echo "Linking complete!"

gcov: $(GEXEC)
	# generate some data for gcov by calling the generated binary with various options
	$(GCOV_DIR)/$(GEXEC) -h
	$(GCOV_DIR)/$(GEXEC) -i input -o output -v

	find ./ -maxdepth 1 -name *.gcno -exec mv {} $(GCOV_DIR) \;
	find ./ -maxdepth 1 -name *.gcda -exec mv {} $(GCOV_DIR) \;

	gcov -o $(GCOV_DIR) $(GEXEC)
	lcov -o $(GCOV_DIR)/$(LCOV_REPORT) -c -f -d $(GCOV_DIR)
	genhtml -o $(GCOV_DIR)/report $(GCOV_DIR)/$(LCOV_REPORT)

package: gcov doc all
	rm -r $(AR_NAME)
	tar cvfz $(AR_NAME) ./*

man: 
	gzip Shell0.1
	sudo mkdir -p /usr/share/man/man1
	sudo cp Shell0.1.gz /usr/share/man/man1 

doc:
	doxygen $(DOC_DIR)/doxygen.conf
	@echo "Documentation generation complete!"

clean:
	rm -rf ./$(OBJ_DIR)/*
	rm -rf ./$(TEST_DIR)/$(OBJ_DIR)/*
	@echo "Cleanup complete!"

mrproper: clean
	rm -rf ./$(BIN_DIR)/*
	rm -rf ./$(TEST_DIR)/$(BIN_DIR)/*
	rm -rf ./$(DOC_DIR)/latex/
	rm -rf ./$(DOC_DIR)/html/
	rm -rf ./$(GCOV_DIR)/*
	@echo "Project reinitilized"

.PHONY: clean, mrproper, doc
