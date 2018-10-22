CFLAGS = -Wall -Wextra -DDEBUG -g -pedantic -std=c++14
P = predictors

all: $(P)

$(P): $(P).o
	g++ $(CFLAGS) $(P).o -o $(P)
$(P).o: $(P).cpp
	g++ $(CFLAGS) -c $(P).cpp -o $(P).o


clean:
	rm -rf *.o $(P)
run: all
	./$(P)
checkmem: all
	valgrind -v --leak-check=full --track-origins=yes ./$(P)
