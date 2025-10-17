BUILD_DIR=build

.PHONY: build
build:
	cd $(BUILD_DIR) && make

.PHONY: run
run:
	cd $(BUILD_DIR) && make && ASAN_OPTIONS=detect_leaks=0 ./terragl

.PHONY: clean
clean:
	cd $(BUILD_DIR) && make clean
