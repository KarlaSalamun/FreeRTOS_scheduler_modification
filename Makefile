TARGET=exec
BUILD_DIR ?= ./build


# -------------------------
FREERTOS_VERSION=V10.2.1
FREERTOS_INCLUDE_DIRS := \
FreeRTOS-Kernel/include
FREERTOS_SOURCES := \
FreeRTOS-Kernel/croutine.c \
FreeRTOS-Kernel/event_groups.c\
FreeRTOS-Kernel/list.c\
FreeRTOS-Kernel/queue.c \
FreeRTOS-Kernel/stream_buffer.c \
FreeRTOS-Kernel/tasks.c \
FreeRTOS-Kernel/timers.c
# -------------------------
SRCS += $(FREERTOS_SOURCES)
INCLUDE_DIRS += $(FREERTOS_INCLUDE_DIRS)

# -------------------------
EXAMPLE_INCLUDE_DIRS := \
scheduling_example
EXAMPLE_SOURCES := \
scheduling_example/main.c
# -------------------------
SRCS += $(EXAMPLE_SOURCES)
INCLUDE_DIRS += $(EXAMPLE_INCLUDE_DIRS)

# -------------------------
PORT_INCLUDE_DIRS := \
portable/GCC/Linux/
PORT_SOURCES := \
portable/GCC/Linux/port.c\
portable/GCC/Linux/heap_4.c

# -------------------------
SRCS += $(PORT_SOURCES)
INCLUDE_DIRS += $(PORT_INCLUDE_DIRS)

LDFLAGS += -lrt -lpthread -lstdc++ -T freertos_linux.ld
CPPFLAGS += -g -Wall -Wextra -Wundef -Wshadow

# Build files
OBJS   := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS   := $(OBJS:.o=.d)

INC_FLAGS := $(addprefix -I,$(INCLUDE_DIRS))
$(info srcs [${INC_FLAGS}])

# Link
$(BUILD_DIR)/$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

# Build C sources
$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(INC_FLAGS) -c $< -o $@

# Build C++ sources
$(BUILD_DIR)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(INC_FLAGS) -c $< -o $@

.PHONY: get_freertos clean

# Checkout FreeRTOS from SVN Repository
# get_freertos:
# 	git clone --recursive --branch $(FREERTOS_VERSION) https://github.com/FreeRTOS/FreeRTOS-Kernel.git FreeRTOS/Source

clean:
	$(RM) -r $(BUILD_DIR)

-include $(DEPS)

