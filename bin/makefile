# clang++ -std=c++17 ../src/*.cpp -o main.exe -I"C:\Program Files\OpenSSL-Win64\include" -L"C:\Program Files\OpenSSL-Win64\lib" -lssl -lcrypto -lstdc++fs

CC:=clang++
CXX:=c++17
CINCLUDE:=
CLIB:=
CFLAGS:=-lssl -lcrypto -lstdc++fs 

SRC_DIR:=../src

CXX_SRC += \
$(SRC_DIR)/filecopy.cpp \
$(SRC_DIR)/main.cpp \
$(SRC_DIR)/sha256_hash.cpp \
$(SRC_DIR)/tree.cpp \
$(SRC_DIR)/config_app.cpp \
$(SRC_DIR)/debug_logger.cpp \

all: release

debug: CFLAGS+=-DDEBUG_VAR
debug: release

release: os_detect build_app_dbg

os_detect:

ifeq ($(OS),Windows_NT) #will include files for windows-specific functions
    CFLAGS+=-DPLATFORM_WIN
    CINCLUDE+=-I"C:\Program Files\OpenSSL-Win64\include"
    CLIB+=-L"C:\Program Files\OpenSSL-Win64\lib"
    CXX_SRC+=$(SRC_DIR)/WIN/win_app.cpp
    CINCLUDE+=-I"$(SRC_DIR)/WIN"
    OSS=WIN
else
    UNAME_S:=$(shell uname -s)
    ifeq ($(UNAME_S),Linux) #will include files for linux-specific functions
        CFLAGS+=-DPLATFORM_LIN
        CXX_SRC+=$(SRC_DIR)/LIN/lin_app.cpp
        CINCLUDE+=-I"$(SRC_DIR)/LIN"
        OSS=LIN
    endif

    ifeq ($(UNAME_S),Darwin) #will include files for OSX-specific functions
        CFLAGS+=-DPLATFORM_OSX
        CXX_SRC+=$(SRC_DIR)/OSX/osx_app.cpp
        CINCLUDE+=-I"$(SRC_DIR)/OSX"
        OSS=OSX
    endif
endif

build_app:
ifeq ($(OSS),WIN)
	@echo "Windows"
	$(CC) -std=$(CXX) $(CXX_SRC) -o main.exe $(CINCLUDE) $(CLIB) $(CFLAGS)
else ifeq ($(OSS),LIN)
	@echo "Linux"
	$(CC) -std=$(CXX) $(CXX_SRC) -o main $(CINCLUDE) $(CLIB) $(CFLAGS)
else ifeq ($(OSS),OSX)
	@echo "OSX"
	$(CC) -std=$(CXX) $(CXX_SRC) -o main $(CINCLUDE) $(CLIB) $(CFLAGS)
else
	@echo "Unknown OS"
endif

build_app_dbg:
ifeq ($(OSS),WIN)
	@echo "Windows"
	$(CC) -std=$(CXX) $(CXX_SRC) -o main_debug.exe $(CINCLUDE) $(CLIB) $(CFLAGS)
else ifeq ($(OSS),LIN)
	@echo "Linux"
	$(CC) -std=$(CXX) $(CXX_SRC) -o main_debug $(CINCLUDE) $(CLIB) $(CFLAGS)
else ifeq ($(OSS),OSX)
	@echo "OSX"
	$(CC) -std=$(CXX) $(CXX_SRC) -o main_debug $(CINCLUDE) $(CLIB) $(CFLAGS)
else
	@echo "Unknown OS"
endif