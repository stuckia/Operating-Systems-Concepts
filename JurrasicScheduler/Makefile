jurassic_scheduler: scheduler.c
	gcc -g -o jurassic_scheduler scheduler.c

install: jurassic_shell
	sudo cp jurassic_scheduler /bin/

uninstall:
	sudo rm -rf /bin/jurassic_scheduler

clean:
	rm -rf jurassic_scheduler