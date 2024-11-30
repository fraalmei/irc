# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fraalmei <fraalmei@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/30 19:06:15 by fraalmei          #+#    #+#              #
#    Updated: 2024/11/30 21:03:29 by fraalmei         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#	file's name
NAME	= irc

#	compilation
CXX			= c++

CXXFLAGS 	= -Wall -Wextra -Werror -std=c++98 #-MD

LEAK_FLAGS	= -fsanitize=address -g3 -pedantic #-fsanitize=thread

#	options to force the remove
RM			= /bin/rm -f

#	Directories
SRC_DIR		= srcs
INCDIR		= include
BIN_DIR		= bin

# Header files
HEADERS		= -I $(INCDIR)

# Directories
SRCS		= main.cpp $(PARSE) $(UTILS) $(CLASES)

PARSE		=

CLASES		=

UTILS		=

# Convert source files to binary
OBJS = $(SRCS:%.cpp=$(BIN_DIR)/%.o) $(UTILS:%.cpp=$(BIN_DIR)/%.o)

all: $(NAME)

$(NAME):	$(BIN) $(OBJS)
	@echo "$(GREEN)Compiling $(NAME)... \n$(RESET)"
	$(CXX) $(CXXFLAGS) $(OBJS) $(HEADERS) -o $(NAME)
	@echo "\n$(RESET)Done !"

#	Objects construction
$(OBJS):	$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	@printf "$(CYAN)Generating $(NAME) objects... %-33.33s\r" $@
	@$(CXX) $(CXXFLAGS) $(HEADERS) -c $< -o $@

re: fclean all

leaks: $(BIN) $(OBJS)
	@echo "\033[0;32mCompiling $(NAME) with sanitizer..."
	$(CXX) $(CXXFLAGS) $(LEAK_FLAGS) $(OBJS) -o $(NAME)
	@echo "\n\033[0mDone !"

clean:
	@echo "\nRemoving binaries..."
	@$(RM) $(OBJS)
	@$(RM) -r $(BIN_DIR)

fclean: clean
	@echo "\nDeleting executable..."
	@$(RM) $(NAME)

releaks: fclean leaks

UNAME = $(shell uname)

show:
	@printf "UNAME		: $(UNAME)\n"
	@printf "NAME		: $(NAME)\n"
	@printf "CC  		: $(CXX)\n"
	@printf "CFLAGS		: $(CXXFLAGS)\n"
	@printf "SRCS		: $(SRCS)\n"
	@printf "OBJS		: $(OBJS)\n"
	@printf "PARSE		: $(PARSE)\n"
	@printf "UTILS		: $(UTILS)\n"
	@printf "UTILS		: $(CLASES)\n"

showf: re printfunctions fclean

printfunctions:
	nm -C $(NAME) | grep -e ' T ' -e ' U ' | awk '{print $$5,$$4,$$3,$$2,$$1}' | awk '{$$1=$$1};1' | awk '{print $$1}' | uniq -u | sort

.PHONY: all clean fclean re leaks releaks

##Colors #
RESET		= 	\033[1;0m
NC			=	\033[0m
TITLE		=	\033[38;5;33m
DEFAULT		=	\033[0;39m
GRAY		=	\033[2;37m
RED			=	\033[0;91m
GREEN		=	\033[32m
YELLOW		=	\033[33m
BLUE		=	\033[0;94m
LRED		=	\033[1;31m
LGREEN		=	\033[1;32m
LBLUE		=	\033[1;34m
MAGENTA		=	\033[0;95m
CYAN		=	\033[0;96m
WHITE		=	\033[0;97m
CURSIVE		=	\033[3m
