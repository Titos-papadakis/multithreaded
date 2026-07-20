HY-345 Operating Systems - Assignment 2
Name: Papadakis Ioannis Titos
AM: 5200, username: csd5200

# perigrafi
Prosomoiwsh leoforeioy pou pigainei foithtes apo th stash A (Xaniwporta) sth
stash B (Panepistimioupolh) kai piso. Kathe foithths kai to leoforeio einai
kai apo ena thread. Sygxronismos me mia FIFO oura (linked list) se kathe
stash (mutex protected) kai duo semaphores ana foithth (gia to boarding kai
gia thn afixh sthn epomenh stash).

# boarding / N-4 rule
Otan to leoforeio ftasei se stash, diatrexei th lista me th seira pou
irthan oi foithtes. An o foithths xwraei sto tmhma tou (mexri N/4 apo
kathe sxolh) mpainei. An oxi, menei sth lista gia thn epomenh fora kai to
leoforeio synexizei ston epomeno, opote den mplokarei osous einai piso tou.
Etsi kratietai kai h proteraiothta (FIFO) kai o kanonas N/4.
To leoforeio fugei apo mia stash otan h lista adeiasei h gemisoun ta
tmhmata/to leoforeio. An h stash einai adeia, fugei kai palio sto xrono tou.

# starvation kai termination
Kanenas den menei gia panta se mia stash, giati to N/4 mhdenizetai kathe
taxidi kai to leoforeio synexizei na kanei drpomologia. Otan olo oi
foithtes ftasoun spiti, to bus thread to katalavainei (metrhths) kai
termatizei, kai to main kanei join se ola ta threads.

# defaults kai ektelesh
N=12, T=10 sec, bus wait se kathe stash=3 sec, xronos diabasmatos 5-15 sec.
make all
./a.out   (dinei arithmo foithtwn apo stdin, kalytera >12)
