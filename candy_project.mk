# ------------------------------------------------------------------------------ RE2 dependency

RE2_DIR = re2/
RE2_OBJ_DIR = $(RE2_DIR)obj/
RE2_LIBRARY = $(RE2_OBJ_DIR)libre2.a
RE2_INCLUDE = $(RE2_DIR)

$(RE2_LIBRARY):
	$(call history_rule,compiling external library,$@)
	$(CMD_PREFIX)make -C $(RE2_DIR) $(patsubst $(RE2_DIR)%,%,$@) > /dev/null

# ------------------------------------------------------------------------------ Application's configuration

# ------------------------------------------------------------ default configuration
override config=debug

PROJECT_CXXFLAGS := -g -DAPP_DEBUG -DYYDEBUG -I $(BUILD_SRC_GEN_DIR) -std=c++11 -I $(RE2_INCLUDE) -Wall -Wextra
PROJECT_BISONFLAGS := --debug
PROJECT_FLEXFLAGS := --debug
PROJECT_LDFLAGS := -lpthread

# ------------------------------------------------------------ debug configuration
$(call trash_configs, debug)
$(call hook_precommit_configs, debug)

# ------------------------------------------------------------ 'make test'
.PHONY: test
test: test/full

# ------------------------------------------------------------------------------ Flex and Bison's binaries
# ------------------------------------------------------------ Flex's file
APP_FLEX_FILE = src/Xml/XmlParser.lex
APP_FLEX_TARGET = $(patsubst %,$(BUILD_SRC_GEN_DIR)%.cpp, $(notdir $(APP_FLEX_FILE)))

$(APP_FLEX_TARGET): $(APP_FLEX_FILE)
	$(call history_rule,compiling flex file,$<)
	$(CMD_MKDIR_ALL) $(dir $@)
	$(CMD_PREFIX)flex -o $@ $(PROJECT_FLEXFLAGS) $<


# ------------------------------------------------------------ Bison's file
APP_BISON_FILE = src/Xml/XmlParser.y
APP_BISON_TARGET = $(patsubst %,$(BUILD_SRC_GEN_DIR)%.cpp, $(notdir $(APP_BISON_FILE)))

$(APP_BISON_TARGET): $(APP_BISON_FILE)
	$(call history_rule,compiling bison file,$<)
	$(CMD_MKDIR_ALL) $(dir $@)
	$(CMD_PREFIX)bison -o $@ -d $(PROJECT_BISONFLAGS) $<

$(APP_FLEX_TARGET): $(APP_BISON_TARGET)


# ------------------------------------------------------------ Flex and Bison's binaries
APP_FB_BINARIES := $(call bin_object_files,$(APP_FLEX_TARGET) $(APP_BISON_TARGET))
$(APP_FB_BINARIES): CXXFLAGS = $(PROJECT_CXXFLAGS) -Wno-deprecated -I src/Xml/


# ------------------------------------------------------------------------------ Application's binaries

APP_CPP_FILES := $(call filelist,./src/application_lib.flist)
APP_OBJECT_BINARIES := $(call bin_object_files,$(APP_CPP_FILES)) $(APP_FB_BINARIES)
$(APP_OBJECT_BINARIES): CXXFLAGS += $(PROJECT_CXXFLAGS)

# ------------------------------------------------------------------------------ Application's main

APP_EXEC_PRODUCT := $(call product_create,BINEXEC,xmltool)
APP_EXEC_TARGET := $(call product_target,$(APP_EXEC_PRODUCT))
$(call product_public,$(APP_EXEC_PRODUCT))

APP_EXEC_CPP_FILES := ./src/main.cpp
APP_EXEC_BINARIES := $(call bin_object_files,$(APP_EXEC_CPP_FILES))

$(APP_EXEC_BINARIES): CXXFLAGS += $(PROJECT_CXXFLAGS)
$(APP_EXEC_TARGET): $(APP_OBJECT_BINARIES) $(APP_EXEC_BINARIES)
$(APP_EXEC_TARGET): LDFLAGS += $(APP_OBJECT_BINARIES) $(APP_EXEC_BINARIES)

$(APP_EXEC_TARGET): $(RE2_LIBRARY)
$(APP_EXEC_TARGET): LDFLAGS += $(RE2_LIBRARY)
$(APP_EXEC_TARGET): LDFLAGS += $(PROJECT_LDFLAGS)
# ------------------------------------------------------------------------------ Application's tests

TEST_APP_FILES := $(call filelist,tests/test.flist)
TEST_APP_CPP_FILES := $(filter %.cpp,$(TEST_APP_FILES))
TEST_APP_SCRIPTS := $(filter-out %.cpp,$(TEST_APP_FILES))

# ------------------------------------------------------------ C++ test targets
TEST_APP_TARGETS := $(call test_bins,$(TEST_APP_CPP_FILES))

# ------------------------------------------------------------ C++ configuration
$(TEST_APP_TARGETS): CXXDEPS += $(APP_HEADERS_TARGET)
$(TEST_APP_TARGETS): CXXFLAGS += $(PROJECT_CXXFLAGS)
$(TEST_APP_TARGETS): CXXFLAGS += -I $(test_apis_dir)

# ------------------------------------------------------------ link configuration
$(TEST_APP_TARGETS): $(APP_OBJECT_BINARIES)
$(TEST_APP_TARGETS): LDFLAGS += $(APP_OBJECT_BINARIES)
$(TEST_APP_TARGETS): LDFLAGS += $(PROJECT_LDFLAGS)

$(TEST_APP_TARGETS): $(RE2_LIBRARY)
$(TEST_APP_TARGETS): LDFLAGS += $(RE2_LIBRARY)

# ------------------------------------------------------------------------------ Application's integration tests

TEST_INTEGRATION_DIRS := $(call filelist,tests_integration/test_integration.flist)
TEST_INTEGRATION_DEST = $(BUILD_DIR)test_integration/
TEST_INTEGRATION_TARGETS := $(patsubst %,$(TEST_INTEGRATION_DEST)%.stdout.log,$(notdir $(TEST_INTEGRATION_DIRS)))

$(foreach TEST_DIR,$(TEST_INTEGRATION_DIRS), \
	$(eval $(patsubst %,$(TEST_INTEGRATION_DEST)%.stdout.log,$(notdir $(TEST_DIR))): _TEST_DIR = $(TEST_DIR))\
)

$(TEST_INTEGRATION_TARGETS): %: $(APP_EXEC_TARGET)
	$(call history_colored_rule,integration test,$(_TEST_DIR),GREEN)
	$(CMD_MKDIR_ALL) $(TEST_INTEGRATION_DEST)
	$(CMD_PREFIX)$(call run_script_cmd,tests_integration/test_invocation.sh) $(APP_EXEC_TARGET) $(TEST_INTEGRATION_DEST) $(_TEST_DIR)

#_TEST_LOG_TARGETS += $(TEST_INTEGRATION_TARGETS)
