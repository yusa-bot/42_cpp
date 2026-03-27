NAME = account
CXX = c++
CXXFLAGS = -Wall  -Wextra -Werror -std=c++98
DEPFLAGS = -MMD -MP

SRCS = Account.cpp test.cpp

OBJS = $(SRCS:.cpp=.o)
DEPS = $(SRCS:.cpp=.d)

all:$(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(DEPFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(DEPS)

fclean: clean
	rm -f $(NAME)

re: fclaen all

-include $(DEPS)

.PHONY: all clean fclean re
