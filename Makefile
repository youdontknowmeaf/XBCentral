CC = g++
CFLAGS = -Wall -Wextra
IMGUI = miniz/miniz.c rlImGui/rlImGui.cpp imgui/imgui.cpp imgui/misc/cpp/imgui_stdlib.cpp imgui/imgui_draw.cpp imgui/imgui_tables.cpp imgui/imgui_widgets.cpp
LIBS = -lraylib -lGL -ldl -lX11 -lm
INCLUDE = -I. -I./imgui -I./miniz
TARGET = main.cpp

all:
	$(CC) $(INCLUDE) $(IMGUI) $(TARGET) $(LIBS) $(CFLAGS)