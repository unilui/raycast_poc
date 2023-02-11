SRCS 		= background.c raycast.c load_hooks.c load_mlx.c render.c exit_raycast.c

OBJS		=	${SRCS:%.c=$(OBJS_DIR)%.o}
OBJS_DIR	=	objects/
NAME		=	raycast
CC			=	cc
CFLAGS	= -O0 -g3
LIBS		=	-lmlx -lX11 -lXext -lm
RM			=	rm -rf

$(OBJS_DIR)%.o:	%.c
			@mkdir -p $(dir $@)
			@$(CC) $(CFLAGS) -c $< -o $@ -I includes

all:		$(NAME)

$(NAME):	$(OBJS)
			@$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LIBS)

clean:
			@$(RM) $(OBJS_DIR)

fclean:
			@$(RM) $(NAME)
			@$(RM) $(OBJS_DIR)

re:			fclean all

.PHONY:		all clean fclean re run libft
