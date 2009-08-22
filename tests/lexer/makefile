# file      : tests/lexer/makefile
# author    : Boris Kolpackov <boris@codesynthesis.com>
# copyright : Copyright (c) 2009 Code Synthesis Tools CC
# license   : MIT; see accompanying LICENSE file

include $(dir $(lastword $(MAKEFILE_LIST)))../../build/bootstrap.make

cxx_tun := driver.cxx

tests := 000 001 002 003 004 005

#
#
cxx_obj  := $(addprefix $(out_base)/,$(cxx_tun:.cxx=.o))
cxx_od   := $(cxx_obj:.o=.o.d)

driver   := $(out_base)/driver
test     := $(out_base)/.test
clean    := $(out_base)/.clean


# Build.
#
$(driver): $(cxx_obj) $(out_root)/cli/lexer.o

$(cxx_obj) $(cxx_od): cpp_options := -I$(src_base) -I$(src_root)

$(call include-dep,$(cxx_od))


# Alias for default target.
#
.PHONY: $(out_base)/
$(out_base)/: $(driver)


# Test.
#
test_targets := $(addprefix $(out_base)/.test-,$(tests))

.PHONY: $(test)
$(test): $(test_targets)


$(test_targets): driver := $(driver)

.PHONY: $(out_base)/.test-%
$(out_base)/.test-%: $(driver) $(src_base)/test-%.cli $(src_base)/test-%.std
	$(call message,test $(out_base)/$*,$(driver) $(src_base)/test-$*.cli | diff -u $(src_base)/test-$*.std -)


# Clean.
#
.PHONY: $(clean)

$(clean):                            \
  $(driver).o.clean                  \
  $(addsuffix .cxx.clean,$(cxx_obj)) \
  $(addsuffix .cxx.clean,$(cxx_od))


# Generated .gitignore.
#
ifeq ($(out_base),$(src_base))
$(driver): | $(out_base)/.gitignore

$(out_base)/.gitignore: files := driver
$(clean): $(out_base)/.gitignore.clean

$(call include,$(bld_root)/git/gitignore.make)
endif


# How to.
#
$(call include,$(bld_root)/cxx/o-e.make)
$(call include,$(bld_root)/cxx/cxx-o.make)
$(call include,$(bld_root)/cxx/cxx-d.make)
$(call include,$(bld_root)/install.make)

# Dependencies.
#
$(call import,$(src_root)/cli/makefile)
