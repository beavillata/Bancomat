CFLAGS = -std=c++17

all: bancomat

bancomat: csv.o main.o
	g++ main.o io.o login.o operations.o user.o csv_cell.o csv_dim.o csv_file.o admin.o $(CFLAGS)

main.o: main.cpp io.cpp login.cpp operations.cpp user.cpp admin.cpp
	g++ -c main.cpp io.cpp login.cpp operations.cpp user.cpp admin.cpp $(CFLAGS)

csv.o: csv/csv_cell.cpp csv/csv_dim.cpp csv/csv_file.cpp
	g++ -c csv/csv_cell.cpp csv/csv_dim.cpp csv/csv_file.cpp $(CFLAGS)

encryption: persistent/encrypter.cpp
	g++ -c persistent/encrypter.cpp

clean:
	rm -rf *.o
	echo Clean done
