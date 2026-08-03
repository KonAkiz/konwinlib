.POSIX:

BROWSER=firefox

.PHONY: test clean

web-example: test

index.html:
	emcc -Wall -Wextra main.c -o index.html \
	-s ALLOW_MEMORY_GROWTH=1 \
	--shell-file shell.html

test: index.html
	${BROWSER} localhost:8000
	python3 -m http.server 8000

clean:
	rm -vf index.html index.js index.wasm
