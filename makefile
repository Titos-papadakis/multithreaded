#Papadakis Ioannis Titos, AM 5200, username csd5200

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
