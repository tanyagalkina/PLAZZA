##
## EPITECH PROJECT, 2019
## d
## File description:
## d
##

END        			= \033[0m
BOLD        		= \033[1m
GREY        		= \033[30m
RED        			= \033[31m
GREEN        		= \033[32m
YELLOW        		= \033[33m
BLUE        		= \033[34m
PURPLE        		= \033[35m
CYAN        		= \033[36m
WHITE        		= \033[37m

EXTENSION 			= .cpp
CC 					= @gcc
CXX 				= @g++
#CFLAGS 			+= -Wall -Werror -Wextra



all: 				plazza

NAME 				= plazza

SRC 				= src/main.cpp 	\

OBJ  				= $(SRC:.cpp=.o)
LIBS 				= -lpthread

plazza: $(OBJ)
	@$(CXX) -o $(NAME) $(OBJ) $(LIBS)
	@echo -e "$(GREEN)* * * * * COMPLETED * * * * *$(END)\n"

clean:
	@rm -f $(OBJ)
	@echo -e "$(RED)$(BOLD)* * * * *  CLEANED  * * * * *$(END)"

fclean: clean
	@rm -f $(NAME)
	@echo -e "$(RED)$(BOLD)* * * * *  CLEANED ALL  * * * * *$(END)"


src/%.o:	src/%$(EXTENSION)
	@$(CXX) $(CFLAGS) $(LIBS) -c -o $@ $<							\
	&& echo -e "$(GREEN)[- - OK - - - -]$(BOLD)" $< "$(END)"			\
	|| echo -e "$(RED)[- - - - KO - -]$(BOLD)" $< "$(END)"


re: 	fclean all

.PHONY: all clean fclean re
