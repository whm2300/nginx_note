
default:	build

clean:
	rm -rf Makefile objs

build:
	$(MAKE) -f objs/Makefile
	$(MAKE) -f objs/Makefile manpage

install:
	$(MAKE) -f objs/Makefile install

upgrade:
	/home/MichaelChen/code/nginx-1.0.14/test/sbin/nginx -t

	kill -USR2 `cat /home/MichaelChen/code/nginx-1.0.14/test/logs/nginx.pid`
	sleep 1
	test -f /home/MichaelChen/code/nginx-1.0.14/test/logs/nginx.pid.oldbin

	kill -QUIT `cat /home/MichaelChen/code/nginx-1.0.14/test/logs/nginx.pid.oldbin`
