HY-345 Operating Systems - Assignment 2
Name: Papadakis Ioannis Titos
AM: 5200

# perigrafi
O kwdikas prosomoiwnei to leoforeio pou pigainei foithtes apo th stash A (Xaniwporta)
sth stash B (Panepistimioupolh) kai piso. Kathe foithths kai to leoforeio einai kai
apo ena thread. O sygxronismos ginetai me mia FIFO oura (linked list) se kathe stash
kai me duo semaphores ana foithth (ena gia to boarding kai ena gia thn afixh).

# pws douleuei to boarding
Otan to leoforeio ftasei se mia stash, diatrexei th lista me th seira pou irthan oi
foithtes (FIFO). An o foithths xwraei sto tmhma tou (mexri N/4 apo kathe sxolh),
mpainei kai bgainei apo th lista. An oxi, menei sth lista gia thn epomenh fora kai
to leoforeio synexizei ston epomeno - etsi den mplokarei oloi oi opisthen tou gia
ena atomo pou den xwraei. Auto krataei kai thn proteraiothta (FIFO) kai ton kanona
tou N/4 mazi.

To leoforeio fugei apo mia stash otan h lista adeiasei h otan den xwraei allo
atomo (gemisan ola ta tmhmata h to leoforeio). An h stash einai adeia, fugei kai
palio sto kanoniko tou xrono.

# starvation
Kanenas foithths den menei gia panta se mia stash: to leoforeio kanei synexeia
drpomologia kai to N/4 mhdenizetai kathe taxidi, opote argh h grhgora tha erthei
h seira tou (efoson o synolikos arithmos twn foithtwn einai peperasmenos).

# termination
Otan olo foithtes ftasoun spiti tous, to leoforeio to kata laveinei (elegxei enan
metrhth) kai stamataei to thread tou. To main kanei join se ola ta threads.

# defaults
N = 12, T = 10 sec (xronos diadromhs), bus wait sto kathe stash = 3 sec
xronos diabasmatos: 5-15 sec (random)

# ektelesh
make all
./a.out
(dinei ton arithmo twn foithtwn apo to stdin, kalytera >12 gia na fanei h oura)
