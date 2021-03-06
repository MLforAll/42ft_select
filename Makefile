# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kelian <kelian@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/12/20 21:41:19 by kdumarai          #+#    #+#              #
#    Updated: 2019/01/26 22:41:42 by kelian           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ft_select

CC_FLAGS = -Wall -Werror -Wextra
CC_LIB = -I includes -I libft/includes
LD_FLAGS = -Llibft -lft -ltermcap

LIBFT = libft/libft.a

INCDIR = includes
INCFILES = ft_select.h
INCLUDES = $(addprefix $(INCDIR)/, $(INCFILES))

SRCDIR = srcs
SRCFILES = ft_select.c \
		   interact.c \
		   navigation.c \
		   print.c \
		   terminal.c \
		   display.c \
		   chlst.c \
		   misc.c
SRCS = $(addprefix $(SRCDIR)/, $(SRCFILES))
NSRC = $(shell echo "$(SRCFILES)" | awk '{print NF}')
ifeq ($(shell [ ! -z "`which bc`" ] && [ ! -z "`which awk`" ] && echo true),true)
	CSRC = 1
else
	CSRC = 0
endif

OBJDIR = objs
OBJS = $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

PROJTEXT = \033[1;31m$(NAME): \033[0;39m

all: $(NAME)

$(LIBFT): force
	@ make -C $(dir $(LIBFT))

$(NAME): $(LIBFT) $(OBJS) $(INCLUDES)
	@ printf "\033[K$(PROJTEXT)Compiling\n"
	@ printf "$(PROJTEXT)Linking\n"
	@ gcc -o $(NAME) $(LD_FLAGS) $(OBJS)
	@ printf "$(PROJTEXT)\033[1;32mProgram built at $(NAME)\033[0;39m\n"

noflags:
	@ printf "\033[1;31m----------------------\n"
	@ printf "| CC_FLAGS DISABLED! |\n"
	@ printf "\r----------------------\033[0;39m\n"
	@ make all CC_FLAGS=""

fsanitize:
	@ printf "\033[1;31m------------------------------\n"
	@ printf "\r| ADDRESS SANITIZER ENABLED! |\n"
	@ printf "\r------------------------------\033[0;39m\n"
	@ make all LD_FLAGS="$(LD_FLAGS) -fsanitize=address"

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@ if [ ! -d $(dir $@) ]; then mkdir -p $(dir $@); fi
	@ printf "\033[K$(PROJTEXT)Compiling \033[1;33m$<"
ifneq ($(CSRC),0)
	@ printf " %.0s" {1..$(shell expr 44 - $(shell printf "$<" | wc -m))}
	@ export LC_ALL=C; printf "\033[1;34m[%3.0f%%]" "$(shell bc <<< "scale=1; $(CSRC) / $(NSRC) * 100")"
	@ $(eval CSRC = $(shell expr $(CSRC) + 1))
endif
	@ printf "\033[0;39m\r"
	@ gcc $(CC_FLAGS) $(CC_LIB) -c $< -o $@

cleanp:
	@ rm -rf $(OBJDIR)
	@ printf "$(PROJTEXT)Removed objects\n"

fcleanp: cleanp
	@ rm -f $(NAME)
	@ printf "$(PROJTEXT)Removed $(NAME)\n"

rep: fcleanp all

clean: cleanp
	@ make clean -C $(dir $(LIBFT))

fclean: fcleanp
	@ make fclean -C $(dir $(LIBFT))

re: fclean all

force:
	@ true

.PHONY: all noflags fsanitize clean fclean re
