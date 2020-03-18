# Flooder

Simple script made to flood destination server with ICMP Echo packets. (2009y)

## usage
gcc -o flooder flooder.c

./flooder ip_dest ip_source
## example
./flooder 127.0.0.1 127.0.0.1 > no.txt

too see how much packet was send:

wc -c no.txt 


