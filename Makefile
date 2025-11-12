#GENERAL VARIABLES

MAKEFLAGS := --no-print-directory
.DEFAULT_GOAL := all

RM := rm -rf
MKDIR := mkdir -p

NAME := miniRT
NAME_CAPITAL := MiniRT

CC := cc
CFLAGS := -Wall -Wextra -Werror -g3 -fsanitize=address

#INCLUDE

LIBFT_PATH := libft/
LIBFT := $(LIBFT_PATH)libft.a
LIBFT_FLAG := -L $(LIBFT_PATH) -l ft

MLX_PATH := MLX42
MLX_BUILD := $(MLX_PATH)/build
MLX_NAME := $(MLX_BUILD)/libmlx42.a
MLX_INCLUDE := $(MLX_PATH)/include/MLX42
MLX_FLAG := -L $(MLX_BUILD) -l mlx42 -l glfw -l dl -l m -pthread

#FILES

SRC_DIR := src/
OBJ_DIR := obj/
INC_DIR := include/
HEADER := $(INC_DIR)/minirt.h

VPATH :=  $(SRC_DIR) $(addprefix $(SRC_DIR), \
				mlx\
				parse\
				parse/read\
				render\
				render/objects\
				vector\
			)

SOURCES :=	\
			minirt.c\
			deg_to_rad.c\
			free_exit.c\
			rad_to_deg.c\
			rgb_to_uint.c\
#mlx
SOURCES +=	\
			close_hook.c\
			key_hook.c\
			loop_hook.c\
#parse
SOURCES +=	\
			parse_ambient.c\
			parse_camera.c\
			parse_cylinder.c\
			parse_file.c\
			parse_light.c\
			parse_line.c\
			parse_plane.c\
			parse_sphere.c\
#parse/read
SOURCES +=	\
			read_color.c\
			read_double_positive.c\
			read_double_ratio.c\
			read_double.c\
			read_int_maxed.c\
			read_v3_normalized.c\
			read_v3.c\
			skip_spaces.c\
#render
SOURCES +=	\
			light.c\
			move_camera.c\
			render_utils.c\
			render.c\
			rotate_camera_horizontally.c\
			rotate_camera_vertically.c\
			update_camera_axis.c\
#render/objects
SOURCES +=	\
			cylinder.c\
			plane.c\
			sphere.c\
#vector
SOURCES +=	\
			v3_add.c\
			v3_build.c\
			v3_dot_product.c\
			v3_cross_product.c\
			v3_magnitude.c\
			v3_normalize.c\
			v3_scale.c\
			v3_substract.c\

OBJECTS := $(addprefix $(OBJ_DIR), $(SOURCES:.c=.o))

#COLOURS

RED := \033[31m
GREEN := \033[32m
YELLOW := \033[33m
BLUE := \033[34m

PDEL-OLM := \033[38;5;207m
DAXFERNA := \033[38;5;76m
CROWN := \033[38;5;220m

RESET := \033[0m

#EXTRA VARIABLES

VALGRIND_DIR := valgrind/

#RULES

$(NAME): $(LIBFT) $(MLX_NAME) $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(LIBFT_FLAG) $(MLX_FLAG) -o $(NAME)
	@$(MAKE) msg_credits

$(LIBFT):
	@$(MAKE) -C $(LIBFT_PATH)

$(MLX_NAME): $(MLX_PATH)
	@$(MAKE) msg_mlx_start
	cmake $(MLX_PATH) -B $(MLX_BUILD)
	make -C $(MLX_BUILD) -j4
	@$(MAKE) msg_mlx_end

$(MLX_PATH):
	@$(MAKE) msg_mlx_clone_start
	git clone https://github.com/codam-coding-college/MLX42.git
	@$(MAKE) msg_mlx_clone_end

$(OBJ_DIR)%.o: %.c $(HEADER) | $(OBJ_DIR)
	$(CC) $(CFLAGS) -I $(INC_DIR) -I $(LIBFT_PATH) -I $(MLX_INCLUDE) -c $< -o $@

$(OBJ_DIR):
	$(MKDIR) $(OBJ_DIR)
	
.PHONY: a all
a: all
all: $(NAME)

.PHONY: c clean
c: clean
clean: msg_clean_start
	$(RM) $(OBJ_DIR)
	@$(MAKE) -C $(LIBFT_PATH) clean
	@$(MAKE) msg_clean_end

.PHONY: f fclean
f: fclean
fclean: msg_fclean_start
	$(RM) $(OBJ_DIR) $(MLX_BUILD) $(NAME)
	@$(MAKE) -C $(LIBFT_PATH) fclean
	@$(MAKE) msg_fclean_end

.PHONY: r re
r: re
re: fclean all

#EXTRA RULES

.PHONY: e exec axis
e: exec
exec: all
	-./$(NAME) scenes/minimalist.rt
axis: all
	-./$(NAME) scenes/axis.rt

.PHONY: n norminette normi
n: norminette
norminette:
	@echo "norminette $(SRC_DIR) $(INC_DIR) $(LIBFT_PATH) | grep Error\n"
	@if norminette $(SRC_DIR) $(INC_DIR) $(LIBFT_PATH) | grep -q "Error"; then echo "$(RED)$$(norminette $(SRC_DIR) $(INC_DIR) $(LIBFT_PATH) | grep "Error" | sed -z 's/\nError/\n\$(YELLOW)  Error/g' | sed -z 's/\n/\n\$(RED)/g')$(RESET)"; else echo "$(GREEN)Everything OK!$(RESET)"; fi
normi:
	@if norminette $(SRC_DIR) $(INC_DIR) $(LIBFT_PATH) | grep -q "Error"; then echo "\n$(RED)$$(norminette $(SRC_DIR) $(INC_DIR) $(LIBFT_PATH) | grep "Error" | grep -v -e "TOO_MANY_FUNCS" -e "WRONG_SCOPE_COMMENT" -e "EMPTY_LINE_FUNCTION" -e "LINE_TOO_LONG" -e "TOO_MANY_LINES" -e "CONSECUTIVE_NEWLINES" -e "INVALID_HEADER" | sed -z 's/\nError/\n\$(YELLOW)  Error/g' | sed -z 's/\n/\n\$(RED)/g')$(RESET)"; else echo "$(GREEN)Run full norminette!$(RESET)"; fi

.PHONY: v valgrind valgrind_no_flags
v: valgrind
valgrind:
	@$(MAKE) CFLAGS= valgrind_no_flags
valgrind_no_flags: clean $(LIBFT) $(OBJECTS) | $(VALGRIND_DIR)
	@$(CC) $(CFLAGS) $(OBJECTS) $(LIBFT_FLAG) $(MLX_FLAG) -o $(NAME)
	@-valgrind --leak-check=full --show-leak-kinds=all --log-file=$(VALGRIND_DIR)$$(date +"%y%m%d%H%M%S").txt ././$(NAME) scenes/minimalist.rt
	@echo "$(BLUE)$$(grep -e "ERROR SUMMARY" -e "lost:" -e "reachable:" $(VALGRIND_DIR)/$$(ls valgrind | tail -1))"
	@$(MAKE) clean

$(VALGRIND_DIR):
	$(MKDIR) $(VALGRIND_DIR)

.PHONY: last_valgrind
last_valgrind:
	@cat $(VALGRIND_DIR)$$(ls valgrind | tail -1)

.PHONY: clean_valgrind
clean_valgrind:
	$(RM) $(VALGRIND_DIR)

#MESSAGES

.PHONY: msg_mlx_clone_start, msg_mlx_clone_end, msg_mlx_start msg_mlx_end msg_clean_start msg_clean_end msg_fclean_start msg_fclean_end msg_credits

msg_mlx_clone_start:
	@echo "$(YELLOW)Cloning MLX42 repository$(RESET)"

msg_mlx_clone_end:
	@echo "$(GREEN)MLX42 clone complete$(RESET)"

msg_mlx_start:
	@echo "$(YELLOW)Starting MLX42 compilation$(RESET)"

msg_mlx_end:
	@echo "$(GREEN)MLX42 compilation complete$(RESET)"

msg_clean_start:
	@echo "$(YELLOW)Cleaning $(NAME) objects$(RED)"

msg_clean_end:
	@echo "$(GREEN)$(NAME_CAPITAL) objects cleaned$(RESET)"

msg_fclean_start:
	@echo "$(YELLOW)Cleaning $(NAME)$(RED)"

msg_fclean_end:
	@echo "$(GREEN)$(NAME_CAPITAL) cleaned$(RESET)"

msg_credits:
	@echo "$(CROWN)\n\
            ▟╗▟▙╖▙╖\n\
            ██████║\n\$(PDEL-OLM)\
██████╗ ████▇▇╗ ▇▇▇████╗██╗            ██████╗ ██╗     ███╗   ███╗\n\
██╔══██╗██╔══██╗██╔════╝██║           ██╔═══██╗██║     ████╗ ████║\n\
██████╔╝██║  ██║█████╗  ██║    █████╗ ██║   ██║██║     ██╔████╔██║\n\
██╔═══╝ ██║  ██║██╔══╝  ██║    ╚════╝ ██║   ██║██║     ██║╚██╔╝██║\n\
██║     ██████╔╝███████╗███████╗      ╚██████╔╝███████╗██║ ╚═╝ ██║\n\
╚═╝     ╚═════╝ ╚══════╝╚══════╝       ╚═════╝ ╚══════╝╚═╝     ╚═╝\n\
$(CROWN)\
                                           ▟╗▟▙╖▙╖\n\
                                           ██████║\n$(DAXFERNA)\
██████╗  █████╗ ██╗  ██╗███████╗███████╗███▇▇▇╗ ▇▇█╗   ██╗ █████╗ \n\
██╔══██╗██╔══██╗╚██╗██╔╝██╔════╝██╔════╝██╔══██╗████╗  ██║██╔══██╗\n\
██║  ██║███████║ ╚███╔╝ █████╗  █████╗  ██████╔╝██╔██╗ ██║███████║\n\
██║  ██║██╔══██║ ██╔██╗ ██╔══╝  ██╔══╝  ██╔══██╗██║╚██╗██║██╔══██║\n\
██████╔╝██║  ██║██╔╝╚██╗██║     ███████╗██║  ██║██║ ╚████║██║  ██║\n\
╚═════╝ ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝     ╚══════╝╚═╝  ╚═╝╚═╝  ╚═══╝╚═╝  ╚═╝\n\
$(RESET)"
