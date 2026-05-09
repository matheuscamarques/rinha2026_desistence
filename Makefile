CC ?= cc
PYTHON ?= python3

# Strict flags applied only to OUR code (main.c)
APP_CFLAGS := -O3 -mavx2 -march=haswell -mtune=haswell -ffast-math -flto \
              -std=c11 -pthread -Wall -Wextra -Wpedantic \
              -Wno-stringop-overflow
# Relaxed flags for vendored third-party (facil.io, yyjson)
VENDOR_CFLAGS := -O3 -mavx2 -march=haswell -mtune=haswell -ffast-math -flto \
                 -std=c11 -pthread -w

CFLAGS ?= $(APP_CFLAGS)

FACIL_INC := \
    -Ithird_party/facil.io/lib/facil \
    -Ithird_party/facil.io/lib/facil/fiobj \
    -Ithird_party/facil.io/lib/facil/http \
    -Ithird_party/facil.io/lib/facil/http/parsers \
    -Ithird_party/facil.io/lib/facil/cli

CPPFLAGS ?= -D_GNU_SOURCE -D_DEFAULT_SOURCE -D_POSIX_C_SOURCE=200809L \
            -D_FILE_OFFSET_BITS=64 -DNDEBUG \
            -I. -Ithird_party/yyjson/src $(FACIL_INC)

LDFLAGS ?= -flto -pthread -Wno-stringop-overflow
LDLIBS ?= -lm

TARGET := fraud_api

# facil.io sources
FACIL_SRC := $(shell find third_party/facil.io/lib/facil \
                     -type f -name '*.c' \
                     -not -path '*/legacy/*' \
                     -not -path '*/redis/*')

# Apenas main.c agora!
APP_SRC := main.c
VENDOR_SRC := third_party/yyjson/src/yyjson.c $(FACIL_SRC)

APP_OBJ := $(APP_SRC:.c=.o)
VENDOR_OBJ := $(VENDOR_SRC:.c=.o)

OBJ := $(APP_OBJ) $(VENDOR_OBJ)

.PHONY: all clean run deps

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $(OBJ) $(LDLIBS)

$(APP_OBJ): %.o: %.c
	$(CC) $(CPPFLAGS) $(APP_CFLAGS) -c $< -o $@

third_party/%.o: third_party/%.c
	$(CC) $(CPPFLAGS) $(VENDOR_CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

deps:
	@if [ ! -d third_party/yyjson ]; then \
	    git clone --depth 1 https://github.com/ibireme/yyjson.git third_party/yyjson; \
	fi
	@if [ ! -d third_party/facil.io ]; then \
	    git clone --depth 1 --branch 0.7.6 https://github.com/boazsegev/facil.io.git third_party/facil.io; \
	fi

clean:
	rm -f $(TARGET) $(APP_OBJ) $(VENDOR_OBJ)