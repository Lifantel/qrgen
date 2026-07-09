# -------------------------------------------------------------------
# qrgen - Makefile
# -------------------------------------------------------------------
CC       = gcc
CFLAGS   = -Wall -Wextra -O2 -Iinclude
LDFLAGS  = -lqrencode -lm

SRC_DIR  = src
BIN      = qrgen

SRCS     = $(SRC_DIR)/qrgen.c $(SRC_DIR)/main.c
OBJS     = $(SRCS:.c=.o)

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(OBJS) -o $(BIN) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(SRC_DIR)/*.o $(BIN) *.png *.jpg

.PHONY: all clean
