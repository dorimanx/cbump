OBJS= main.o lib.o
TARGET= cbump
CC= gcc
CFLAGS= -Wall -Wextra

all: $(TARGET) strip

$(TARGET) : $(OBJS) ; $(CC) -o $@ $^ $(CFLAGS)

strip : ; strip -s $(TARGET)

clean : ; $(RM) $(TARGET) $(OBJS)

.PHONY : clean strip