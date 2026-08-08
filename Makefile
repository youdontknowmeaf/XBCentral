CC = g++
IMGUI = rlImGui/rlImGui.cpp imgui/imgui.cpp imgui/imgui_draw.cpp imgui/imgui_tables.cpp imgui/imgui_widgets.cpp
LIBS = -lraylib -lGL -ldl -lX11 -lm
INCLUDE = -I. -I./imgui
TARGET = main.cpp

all:
	$(CC) $(INCLUDE) $(IMGUI) $(TARGET) $(LIBS)