
# ------------------------------------------------------------------------------ Application's configuration

# ------------------------------------------------------------ default configuration
override config=debug

PROJECT_CXXFLAGS := -Wall -Wextra -g -DAPP_DEBUG -DYYDEBUG
PROJECT_BISONFLAGS := --debug
PROJECT_FLEXFLAGS := --debug

# ------------------------------------------------------------ debug configuration
$(call trash_configs, debug)
$(call hook_precommit_configs, debug)


# ------------------------------------------------------------------------------ Flex's file

APP_FLEX_FILE = src/LexicalAnalyser.lex
APP_FLEX_TARGET = $(patsubst %,$(BUILD_SRC_GEN_DIR)%.cpp, $(notdir $(APP_FLEX_FILE)))

$(APP_FLEX_TARGET): $(APP_FLEX_FILE)
	$(call history_rule,compiling flex file,$<)
	$(CMD_MKDIR_ALL) $(dir $@)
	$(CMD_PREFIX)flex -o $@ $(PROJECT_FLEXFLAGS) $<


# ------------------------------------------------------------------------------ Bison's file

APP_BISON_FILE = src/GrammarSpecification.y
APP_BISON_TARGET = $(patsubst %,$(BUILD_SRC_GEN_DIR)%.cpp, $(notdir $(APP_BISON_FILE)))

$(APP_BISON_TARGET): $(APP_BISON_FILE)
	$(call history_rule,compiling bison file,$<)
	$(CMD_MKDIR_ALL) $(dir $@)
	$(CMD_PREFIX)bison -o $@ -d $(PROJECT_BISONFLAGS) $<


# ------------------------------------------------------------------------------ Flex and Bison's binaries

APP_FB_BINARIES := $(call bin_object_files,$(APP_FLEX_TARGET) $(APP_BISON_TARGET))
$(APP_FB_BINARIES): CXXFLAGS = -I src/

# ------------------------------------------------------------------------------ Application's binaries

APP_BINARIES_PRODUCT := $(call product_create,BINLIBSTATIC,application_lib)
APP_BINARIES_TARGET := $(call product_target,$(APP_BINARIES_PRODUCT))
$(call product_public,$(APP_BINARIES_PRODUCT))

APP_CPP_FILES := $(call filelist,./src/application_lib.flist)
APP_OBJECT_BINARIES := $(call bin_object_files,$(APP_CPP_FILES))

# ------------------------------------------------------------ compilation/link configuration
$(APP_OBJECT_BINARIES): CXXFLAGS += $(PROJECT_CXXFLAGS)
$(APP_BINARIES_TARGET): $(APP_OBJECT_BINARIES) $(APP_FB_BINARIES)
$(APP_BINARIES_TARGET): ARFLAGS += $(APP_OBJECT_BINARIES)


# ------------------------------------------------------------------------------ Application's tests

TEST_APP_FILES := $(call filelist,tests/test.flist)
TEST_APP_CPP_FILES := $(filter %.cpp,$(TEST_APP_FILES))
TEST_APP_SCRIPTS := $(filter-out %.c,$(TEST_APP_FILES))

# ------------------------------------------------------------ C++ test targets
TEST_APP_TARGETS := $(call test_bins,$(TEST_APP_CPP_FILES))

# ------------------------------------------------------------ C++ configuration
$(TEST_APP_TARGETS): CXXDEPS += $(APP_HEADERS_TARGET)
$(TEST_APP_TARGETS): CXXFLAGS += $(PROJECT_CXXFLAGS)
$(TEST_APP_TARGETS): CXXFLAGS += -I $(test_apis_dir)

# ------------------------------------------------------------ link configuration
$(TEST_APP_TARGETS): $(APP_BINARIES_TARGET)
$(TEST_APP_TARGETS): LDFLAGS += $(APP_BINARIES_TARGET)
$(TEST_APP_TARGETS): LDFLAGS += $(PROJECT_LDFLAGS)
