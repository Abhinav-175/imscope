CXX = g++

EXE = imscope_v00
IMGUI_DIR = ../imgui

SOURCES = ../main.cpp
SOURCES += $(IMGUI_DIR)/imgui.cpp $(IMGUI_DIR)/imgui_demo.cpp $(IMGUI_DIR)/imgui_draw.cpp $(IMGUI_DIR)/imgui_tables.cpp $(IMGUI_DIR)/imgui_widgets.cpp
SOURCES += $(IMGUI_DIR)/backends/imgui_impl_glfw.cpp $(IMGUI_DIR)/backends/imgui_impl_opengl3.cpp
OBJS = $(addsuffix .o, $(basename $(notdir $(SOURCES))))

CXXFLAGS = -std=c++11 -I$(IMGUI_DIR) -I$(IMGUI_DIR)/backends
CXXFLAGS += -g -Wall -Wformat
LIBS = -lGL `pkg-config --static --libs glfw3`
CXXFLAGS += `pkg-config --cflags glfw3`

VPATH = . $(IMGUI_DIR) $(IMGUI_DIR)/backends ../

# --- Build Rules ---

all: $(EXE)
	@echo "ImScope: build successful"

$(EXE): $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(EXE) $(OBJS) imgui.ini