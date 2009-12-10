# file      : tests/file/makefile
# author    : Boris Kolpackov <boris@codesynthesis.com>
# copyright : Copyright (c) 2009 Code Synthesis Tools CC
# license   : MIT; see accompanying LICENSE file

include $(dir $(lastword $(MAKEFILE_LIST)))../../build/bootstrap.make

cxx_tun := driver.cxx
cli_tun := test.cli

tests := 000 001 002 003

#
#
cxx_obj  := $(addprefix $(out_base)/,$(cxx_tun:.cxx=.o) $(cli_tun:.cli=.o))
cxx_od   := $(cxx_obj:.o=.o.d)

driver   := $(out_base)/driver
test     := $(out_base)/.test
clean    := $(out_base)/.clean

# Build.
#
$(driver): $(cxx_obj)
$(cxx_obj) $(cxx_od): cpp_options := -I$(out_base)

genf := $(cli_tun:.cli=.hxx) $(cli_tun:.cli=.ixx) $(cli_tun:.cli=.cxx)
gen  := $(addprefix $(out_base)/,$(genf))

$(gen): $(out_root)/cli/cli
$(gen): cli := $(out_root)/cli/cli
$(gen): cli_options := --generate-file-scanner

$(call include-dep,$(cxx_od),$(cxx_obj),$(gen))

# Alias for default target.
#
$(out_base)/: $(driver)

# Test.
#
test_targets := $(addprefix $(out_base)/.test-,$(tests))

$(test): $(test_targets)
$(test_targets): driver := $(driver)

.PHONY: $(out_base)/.test-%

$(out_base)/.test-000: $(driver) $(src_base)/test-000.ops
	$(call message,test $(out_base)/000,$(driver) -a 1 \
--file $(src_base)/empty.ops -b 1 --file $(src_base)/base.ops \
--file $(src_base)/test-000.ops b | diff -u $(src_base)/test-000.std -)

$(out_base)/.test-001: $(driver) $(src_base)/test-001.ops
	$(call message,test $(out_base)/001,$(driver) -a 1 -- --file \
$(src_base)/test-001.ops b | diff -u $(src_base)/test-001.std -)

$(out_base)/.test-002: $(driver) $(src_base)/test-002.ops
	$(call message,test $(out_base)/002,$(driver) -a 1 --file \
$(src_base)/test-002.ops --file $(src_base)/empty.ops b | \
diff -u $(src_base)/test-002.std -)

$(out_base)/.test-003: $(driver)
	$(call message,test $(out_base)/003,$(driver) -a 1 --file \
$(src_base)/base.ops --file test-003.ops b | diff -u $(src_base)/test-003.std -)

# Clean.
#
$(clean):                            \
  $(driver).o.clean                  \
  $(addsuffix .cxx.clean,$(cxx_obj)) \
  $(addsuffix .cxx.clean,$(cxx_od))  \
  $(addprefix $(out_base)/,$(cli_tun:.cli=.cxx.cli.clean))

# Generated .gitignore.
#
ifeq ($(out_base),$(src_base))
$(driver): | $(out_base)/.gitignore

$(out_base)/.gitignore: files := driver $(genf)
$(clean): $(out_base)/.gitignore.clean

$(call include,$(bld_root)/git/gitignore.make)
endif

# How to.
#
$(call include,$(bld_root)/cxx/o-e.make)
$(call include,$(bld_root)/cxx/cxx-o.make)
$(call include,$(bld_root)/cxx/cxx-d.make)
$(call include,$(scf_root)/cli/cli-cxx.make)

# Dependencies.
#
$(call import,$(src_root)/cli/makefile)

