CFLAGS = -std=c++17

all: bancomat

bancomat: main.o
	g++ -c main.o csv_cell.o csv_file.o csv_dim.o login.o user.o operations.o io.o &(CFLAGS)

main.o: main.cpp
	g++ main.cpp $(CFLAGS)

csv_cell.o: csv/csv_cell.cpp
	g++ csv/csv_cell.cpp $(CFLAGS)

csv_dim.o: csv/csv_dim.cpp
	g++ csv/csv_dim.cpp $(CFLAGS)

csv_file.o: csv/csv_file.cpp
	g++ csv/csv_file.cpp $(CFLAGS)

login.o: login.cpp
	g++ login.cpp $(CFLAGS)

user.o: user.cpp
	g++ user.cpp $(CFLAGS)

operations.o: operations.cpp
	g++ operations.cpp $(CFLAGS)

io.o: io.cpp
	g++ io.cpp $(CFLAGS)

clean: rm -rf *o bancomat
