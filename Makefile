CC=gcc
CFLAGS=-c `pkg-config --cflags gtk+-3.0` -I.
LDFLAGS=`pkg-config --libs gtk+-3.0`
FORM=form.c form.h
DRAW=draw.c draw.h
OBJECTS=app.o draw.o form.o

all: app

app: $(OBJECTS)
	$(CC) -o app *.o $(LDFLAGS)

form.o: $(FORM)
	$(CC) $(CFLAGS) form.c

draw.o: $(DRAW)
	$(CC) $(CFLAGS) draw.c

app.o: app.c
	$(CC) $(CFLAGS) app.c

clean:
	rm -rf *.o app
