
# ------------------------------------------------------------------------------ Application's configuration

# ------------------------------------------------------------ default configuration
override config=debug

PROJECT_CXXFLAGS := -Wall -Wextra -g -DAPP_DEBUG

# ------------------------------------------------------------ debug configuration
$(call trash_configs, debug)
$(call hook_precommit_configs, debug)


# ------------------------------------------------------------------------------ Application's binaries

APP_BINARIES_PRODUCT := $(call product_create,BINLIBSTATIC,application_lib)
APP_BINARIES_TARGET := $(call product_target,$(APP_BINARIES_PRODUCT))
$(call product_public,$(APP_BINARIES_PRODUCT))

APP_OBJECT_BINARIES := $(call bin_object_files,$(call filelist,./src/application_lib.flist))

# ------------------------------------------------------------ compilation/link configuration
$(APP_BINARIES_TARGET): $(APP_OBJECT_BINARIES)
$(APP_BINARIES_TARGET): CXXFLAGS += $(PROJECT_CXXFLAGS)
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
