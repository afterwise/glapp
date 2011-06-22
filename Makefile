
all:
	gcc -std=gnu99 -Wall -Wextra -Werror -O2 -lGL -lGLU -lglut *.c -oglapp

.PHONY : clean
clean:
	rm -f glapp
