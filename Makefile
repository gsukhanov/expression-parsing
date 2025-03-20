default_target:
	cmake -S . -B build && cd build && make

test: default_target
	cd build && ./tests

differentiator: default_target
	cd build && ./differentiator $(ARGS)

install: default_target
	cd build && sudo make install

uninstall:
	sudo rm -f ~/bin/differentiator

clear:
	rm -rf ./build