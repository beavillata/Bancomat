CFLAGS = -std=c++17

all: bancomat

bancomat:
	g++ main.cpp csv/csv_cell.cpp csv/csv_file.cpp csv/csv_dim.cpp login.cpp user.cpp operations.cpp io.cpp $(CFLAGS)

clean: rm -rf *.out
