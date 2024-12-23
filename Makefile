NAME		:= computor
INC			:= $(shell find srcs -type d | sed 's/^/-I /')
HEADERS		:= $(shell find srcs -name "*.hpp"| sed 's/^/.\//')
SRCS_DIR	:= ./srcs
SRCS	    := $(shell find srcs -name "*.cpp"| sed 's/^/.\//')
OBJS_DIR	:= ./objs
OBJS		:= $(subst $(SRCS_DIR), $(OBJS_DIR), $(SRCS:.cpp=.o))
DEPS		:= $(subst $(SRCS_DIR), $(OBJS_DIR), $(SRCS:.cpp=.d))

CXX			:= c++
CXXFLAGS	:= -Wall -Wextra -Werror -std=c++11 $(INC) -MMD -MP

# Debug
ifeq ($(MAKECMDGOALS), debug)
	CXXFLAGS += -DDEBUG
endif

ifeq ($(MAKECMDGOALS), address)
	CXXFLAGS += -g -fsanitize=address
endif

all			: $(NAME)

$(NAME)	: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

$(OBJS_DIR)/%.o: srcs/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -o $@ -c $<

clean		:
	rm -rf $(OBJS_DIR)

fclean		: clean
	$(RM) $(NAME)

re			: fclean all

debug		: re

address		: re

format		:
	clang-format -i $(SRCS) $(HEADERS)

.PHONY		: all clean fclean re address debug format

-include $(DEPS)
