
UNITY_REPO = https://github.com/ThrowTheSwitch/Unity
UNITY_DIR = lib/Unity
UNITY_SRC = $(UNITY_DIR)/src
UNITY_FIXTURE = $(UNITY_DIR)/extras/fixture
UNITY_EXISTS = $(shell [ -d $(UNITY_DIR) ] && echo 1)

GBDK_TAR = https://github.com/gbdk-2020/gbdk-2020/releases/download/4.3.0/gbdk-macos.tar.gz
GBDK_DIR = lib/gbdk
GBDK_EXISTS = $(shell [ -d $(GBDK_DIR) ] && echo 1)

LCC = $(GBDK_DIR)/bin/lcc

C_SRC = $(shell find src -name "*.c")

TEST_SRC = $(wildcard test/*.c) $(filter-out src/main.c, $(C_SRC))
TEST_RUNNER = test_runner

BUILD_DIR = builds
TIMESTAMP = $(shell date +"%Y-%m-%d_%H-%M-%S")
OUTPUT = $(BUILD_DIR)/output-$(TIMESTAMP).gb

SAMEBOY_DIR = /Applications/SameBoy.app
SAMEBOY_EXISTS = $(shell [ -d $(SAMEBOY_DIR) ] && echo 1)

all: prepare_build_dir clone_unity download_gbdk run_tests build open clean

prepare_build_dir:
	@mkdir -p $(BUILD_DIR)

clone_unity:
ifeq ($(UNITY_EXISTS),)
	@echo "Cloning Unity..."
	git clone $(UNITY_REPO) $(UNITY_DIR)
else
	@echo "Unity already exists!"
endif

download_gbdk:
ifeq ($(GBDK_EXISTS),)
	@echo "Downloading GBDK-2020..."
	curl -L $(GBDK_TAR) -o gbdk.tar.gz
	@echo "Extracting GBDK-2020..."
	tar -xzf gbdk.tar.gz -C lib
	rm gbdk.tar.gz
else
	@echo "GBDK-2020 already exists!"
endif

run_tests:
	rm -rf $(TEST_RUNNER)
	gcc -DTEST -I$(UNITY_SRC) -I$(UNITY_FIXTURE) -I./src -I./test -o $(TEST_RUNNER) $(UNITY_SRC)/unity.c $(TEST_SRC)
	./$(TEST_RUNNER)

build: $(C_SRC)
	$(LCC) -o $(OUTPUT) $(C_SRC)

open:
ifeq ($(SAMEBOY_EXISTS),1)
	open -a $(SAMEBOY_DIR) $(shell ls -t $(BUILD_DIR)/*.gb | head -n 1)
endif

clean:
	rm -rf ./*.asm ./*.ihx ./*.lst ./*.o ./*.s2 ./*.sym $(TEST_RUNNER)
