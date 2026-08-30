NAME = PmergeMe

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -pedantic -std=c++98 -O3

SRCS = main.cpp PmergeMe.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

-include $(OBJS:.o=.d)

clean:
	$(RM) $(OBJS) $(OBJS:.o=.d)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
