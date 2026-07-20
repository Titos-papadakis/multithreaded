#to onoma toy ektelesimoy
exec = a.out

#to arxeio
src = hy345.c

#paragei to a.out
all: $(exec)

#compile
$(exec): $(src)
	gcc -pthread -o $(exec) $(src)

clean:
	rm -f $(exec) *.o
