############
# Settings #
############

# Build all test applications:
#   make
#

# Location of build tools and atomthreads sources
KERNEL_DIR=../../kernel
TESTS_DIR=../../tests
BSP_DIR = ./mcs_bsp
CC=mb-gcc
OBJCOPY=mb-objcopy
SIZE=mb-size
LITTLE_ENDIAN=true

# Enable stack-checking.
STACK_CHECK=true

# Directory for built objects
BUILD_DIR=build

# Port/application object files
APP_OBJECTS = atomport.o tests-main.o
APP_ASM_OBJECTS = atomport-asm.o
BSP_OBJECTS = xiomodule.o
BSP_LDSCRIPT = linker_bram_64k.ld

# Kernel object files
KERNEL_OBJECTS = atomkernel.o atomsem.o atommutex.o atomtimer.o atomqueue.o

# Collection of built objects (excluding test applications)
ALL_OBJECTS = $(APP_OBJECTS) $(APP_ASM_OBJECTS) $(KERNEL_OBJECTS) $(BSP_OBJECTS)
BUILT_OBJECTS = $(patsubst %,$(BUILD_DIR)/%,$(ALL_OBJECTS))

# Test object files (dealt with separately as only one per application build)
TEST_OBJECTS = $(notdir $(patsubst %.c,%.o,$(wildcard $(TESTS_DIR)/*.c)))

# Target application filenames (.elf and .hex) for each test object
TEST_ELFS = $(patsubst %.o,%.elf,$(TEST_OBJECTS))
TEST_HEXS = $(patsubst %.o,%.hex,$(TEST_OBJECTS))

# Search build/output directory for dependencies
vpath %.o ./$(BUILD_DIR)
vpath %.elf ./$(BUILD_DIR)
vpath %.hex ./$(BUILD_DIR)

# GCC flags
CFLAGS=-g -Os -Wall -Werror -mlittle-endian

# Enable stack-checking (disable if not required)
ifeq ($(STACK_CHECK),true)
CFLAGS += -DATOM_STACK_CHECKING
endif

#################
# Build targets #
#################

# All tests
all: $(BUILD_DIR) $(TEST_HEXS) Makefile

# Make build/output directory
$(BUILD_DIR):
	mkdir $(BUILD_DIR)

# Test HEX files (one application build for each test)
$(TEST_HEXS): %.hex: %.elf
	@echo Building $@
	$(OBJCOPY) -j .text -j .data -O ihex $(BUILD_DIR)/$< $(BUILD_DIR)/$@

# Test ELF files (one application build for each test)
$(TEST_ELFS): %.elf: %.o $(KERNEL_OBJECTS) $(BSP_OBJECTS) $(APP_OBJECTS) $(APP_ASM_OBJECTS)
	$(CC) $(CFLAGS) $(BUILD_DIR)/$(notdir $<) $(BUILT_OBJECTS) --output $(BUILD_DIR)/$@ -Wl,-Map,$(BUILD_DIR)/$(basename $@).map -T$(BSP_DIR)/$(BSP_LDSCRIPT)

# Kernel objects builder
$(KERNEL_OBJECTS): %.o: $(KERNEL_DIR)/%.c
	$(CC) -c $(CFLAGS) -I. -I$(BSP_DIR) $< -o $(BUILD_DIR)/$(notdir $@)

# Test objects builder
$(TEST_OBJECTS): %.o: $(TESTS_DIR)/%.c
	$(CC) -c $(CFLAGS) -I. -I$(KERNEL_DIR) -I$(BSP_DIR) $< -o $(BUILD_DIR)/$(notdir $@)

# Application C objects builder
$(APP_OBJECTS): %.o: ./%.c
	$(CC) -c $(CFLAGS) -I. -I$(KERNEL_DIR) -I$(TESTS_DIR) -I$(BSP_DIR) $< -o $(BUILD_DIR)/$(notdir $@)

# Application asm objects builder
$(APP_ASM_OBJECTS): %.o: ./%.S
	$(CC) -c $(CFLAGS) -x assembler-with-cpp -I. -I$(KERNEL_DIR) -I$(BSP_DIR) $< -o $(BUILD_DIR)/$(notdir $@)

$(BSP_OBJECTS): %.o: $(BSP_DIR)/%.c
	$(CC) -c $(CFLAGS) -I. -I$(KERNEL_DIR) -I$(TESTS_DIR) -I$(BSP_DIR) $< -o $(BUILD_DIR)/$(notdir $@)
	
# .lst file builder
%.lst: %.c
	$(CC) $(CFLAGS) -I. -I$(KERNEL_DIR) -I$(TESTS_DIR) -I$(BSP_DIR) -Wa,-al $< > $@

# Clean
clean:
	rm -fr build
	rm -fr doxygen-kernel
	rm -fr doxygen-microblaze

doxygen:
	doxygen $(KERNEL_DIR)/Doxyfile
	doxygen ./Doxyfile
