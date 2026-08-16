CC = g++ -std=c++17
CFLAGS = -Wall -Wextra -O2 -static-libgcc -static-libstdc++
IMGUI = rlImGui/rlImGui.cpp imgui/imgui.cpp imgui/misc/cpp/imgui_stdlib.cpp imgui/imgui_draw.cpp imgui/imgui_tables.cpp imgui/imgui_widgets.cpp
LIBS = -lraylib -lGL -ldl -lX11 -lm
static: LIBS = /usr/local/lib/libraylib.a -lGL -ldl -lpthread -lm -lX11 -lXrandr -lXinerama -lXcursor -lXi -lXxf86vm
INCLUDE = -I. -I./imgui
TARGET = main.cpp

static: LIBS = /usr/local/lib/libraylib.a -lGL -ldl -lpthread -lm -lX11 -lXrandr -lXinerama -lXcursor -lXi -lXxf86vm
static: OBJS = *.o

all:
	$(CC) $(INCLUDE) $(IMGUI) $(TARGET) $(LIBS) $(CFLAGS)

static:
	$(CC) $(CFLAGS) $(INCLUDE) -c $(TARGET) $(IMGUI)
	$(CC) $(OBJS) $(LIBS) $(CFLAGS)
	rm $(OBJS)

release:
	mkdir -p ./release
	cp a.out install-payload extract-xiso LICENSE Xbox_logo.png RunMe.sh release/ && \
	printf "\033[1;37mPlease open the app via 'RunMe.sh' or else it might break\033[0m\n"
	tar -czf release.tar.gz -C release . && \
	printf "\033[1;37mCreated a tarball called 'release.tar.gz'\033[0m\n"
	rm -r release && \
	printf "\033[1;37mRemoved the release folder.\033[0m\n"