
CC = c++

CFLAGS = -std=c++20 -Wall -Wextra -Werror

LIB = libftpp.a

SRCS = srcs/DataStructures/data_buffer.cpp \
		srcs/DataStructures/pool.cpp \
		srcs/DesignPatterns/memento.cpp \
		srcs/DesignPatterns/observer.cpp \
		srcs/DesignPatterns/singleton.cpp \
		srcs/DesignPatterns/state_machine.cpp \
		srcs/Threading/thread_safe_iostream.cpp \
		srcs/Threading/thread_safe_queue.cpp \
		srcs/Threading/thread.cpp \
		srcs/Threading/worker_pool.cpp \
		srcs/Threading/persistent_worker.cpp \
		srcs/Network/message.cpp \
		srcs/Network/connection.cpp \
		srcs/Network/client.cpp \
		srcs/Network/server.cpp \
		srcs/Mathematics/ivector2.cpp \
		srcs/Mathematics/ivector3.cpp \
		srcs/Mathematics/random_2D_coordinate_generator.cpp \
		srcs/Mathematics/perlin_noise_2D.cpp \
		srcs/Bonus/timer.cpp \
		srcs/Bonus/chronometer.cpp \
		srcs/Bonus/spinlock.cpp

OBJS = $(SRCS:.cpp=.o)

$(LIB): $(OBJS)
	ar rcs -o $(LIB) $(OBJS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

re:	fclean $(LIB)

fclean:	clean
	rm -f $(LIB)

clean:
	rm -f $(OBJS)

.PHONY: clean fclean re