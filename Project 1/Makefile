all: hash.o spellcheck.o spellcheck.out

spellcheck.out: spellcheck.o hash.o
	g++ -o spellcheck.out spellcheck.o hash.o

spellcheck.o:
	g++ -c spellcheck.cpp

hash.o:
	g++ -c hash.cpp

debug:
	g++ -g -o spellDebug.out spellcheck.cpp hash.cpp

clean:
	rm -f *.out *.o *.stackdump *~

backup:
	test -d backups || mkdir backups
	cp *.cpp backups
	cp *.h backups
	cp Makefile backups