# Makefile — Buffer Overflow demo
#
# Targets:
#   make            -> build the vulnerable server
#   make secure     -> build a hardened version for comparison
#   make clean      -> remove built binaries

CC      = gcc
CFLAGS_VULN = -m32 -fno-stack-protector -z execstack -no-pie -g -w
CFLAGS_SAFE = -m32 -fstack-protector-all -D_FORTIFY_SOURCE=2 -O2 -pie -fPIE -g

all: server

server: server.c
	$(CC) $(CFLAGS_VULN) -o server server.c
	@echo
	@echo "[!] Built vulnerable server with the following protections DISABLED:"
	@echo "    - stack canaries  (-fno-stack-protector)"
	@echo "    - NX / DEP        (-z execstack)"
	@echo "    - PIE             (-no-pie)"
	@echo "    Remember: also disable ASLR before exploiting."

secure: server.c
	$(CC) $(CFLAGS_SAFE) -o server_secure server.c
	@echo "[+] Built hardened binary as ./server_secure"

clean:
	rm -f server server_secure

.PHONY: all secure clean
