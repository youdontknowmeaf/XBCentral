CC = g++ -std=c++17
CFLAGS = -Wall -Wextra -O2 -static-libgcc -static-libstdc++
IMGUI = rlImGui/rlImGui.cpp imgui/imgui.cpp imgui/misc/cpp/imgui_stdlib.cpp imgui/imgui_draw.cpp imgui/imgui_tables.cpp imgui/imgui_widgets.cpp
LIBS = -lraylib -lGL -ldl -lX11 -lm
INCLUDE = -I. -I./imgui
TARGET = main.cpp

static: LIBS = /usr/local/lib/libraylib.a -lglfw -lGL -ldl -lpthread -lm -lX11 -lXrandr -lXinerama -lXcursor -lXi -lXxf86vm
static: OBJS = main.o

all:
	$(CC) $(INCLUDE) $(IMGUI) $(TARGET) $(LIBS) $(CFLAGS)

static:
	$(CC) $(CFLAGS) -c $(TARGET) -o $(OBJS)
	$(CC) $(OBJS) $(LIBS)
	rm $(OBJS)