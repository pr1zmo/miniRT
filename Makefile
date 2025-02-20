NAME		=	miniRT
SRC_DIR		=	src
PARSE_DIR	=	parsing
SRC_FILES	=	main.c free.c debug.c raytracer.c vectors.c events/mouse.c rays.c
PARSE_FILES	=	file.c init.c parsing.c atoi_double.c parsing_utils.c \
            set_objects.c parse_objects.c
FILES		=	$(addprefix $(SRC_DIR)/, $(SRC_FILES)) \
            $(addprefix $(PARSE_DIR)/, $(PARSE_FILES))
OBJ			=	$(FILES:.c=.o)
FLAGS		=	-Wall -Wextra -Werror
ASAN		=	-g3 -pg -fsanitize=address
MLX			=	mlx/libmlx.a -lXext -lX11 -lm
LIBFT		=	libft/libft.a
INCLUDES	=	-Imlx -I.

all: $(NAME)

$(NAME): $(OBJ)
	make -C libft
	cc $(FLAGS) -o $(NAME) $(OBJ) $(LIBFT) $(MLX)

sanitize: clean
	make -C libft
	cc $(FLAGS) $(ASAN) -o $(NAME) $(FILES) $(LIBFT) $(MLX)

%.o: %.c
	cc $(FLAGS) $(INCLUDES) -c $< -o $@

clean:
	make -C libft clean
	rm -f $(OBJ)

fclean: clean
	make -C libft fclean
	rm -f $(NAME)

valgrind: all
	valgrind --leak-check=full --track-origins=yes ./$(NAME) scenes/mandatory.rt

re: fclean all

.PHONY: all clean fclean re sanitize