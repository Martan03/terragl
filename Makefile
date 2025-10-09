BUILD_DIR=build

.PHONY: build
build:
	cd $(BUILD_DIR) && make

.PHONY: run
run:
	cd $(BUILD_DIR) && make && ./terragl
	
.PHONY: clean
clean:
	cd $(BUILD_DIR) && make clean
