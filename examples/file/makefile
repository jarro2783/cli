# file      : examples/file/makefile
# author    : Boris Kolpackov <boris@codesynthesis.com>
# copyright : Copyright (c) 2009-2010 Code Synthesis Tools CC
# license   : MIT; see accompanying LICENSE file

include $(dir $(lastword $(MAKEFILE_LIST)))../../build/bootstrap.make

cli := options.cli
cxx := driver.cxx

obj := $(addprefix $(out_base)/,$(cxx:.cxx=.o) $(cli:.cli=.o))
dep := $(obj:.o=.o.d)

driver   := $(out_base)/driver
install  := $(out_base)/.install
clean    := $(out_base)/.clean

# Build.
#
$(driver): $(obj)

$(obj) $(dep): cpp_options := -I$(out_base)

genf := $(cli:.cli=.hxx) $(cli:.cli=.ixx) $(cli:.cli=.cxx)
gen  := $(addprefix $(out_base)/,$(genf))

$(gen): cli := $(out_root)/cli/cli
$(gen): cli_options += --generate-file-scanner
$(gen): $(out_root)/cli/cli

$(call include-dep,$(dep),$(obj),$(gen))

# Convenience alias for default target.
#
$(out_base)/: $(driver)

# Install
#
$(install): path := $(subst $(src_root)/,,$(src_base))
$(install):
	$(call install-data,$(src_base)/driver.cxx,$(install_doc_dir)/cli/$(path)/driver.cxx)
	$(call install-data,$(src_base)/options.cli,$(install_doc_dir)/cli/$(path)/options.cli)
	$(call install-data,$(src_base)/README,$(install_doc_dir)/cli/$(path)/README)
	$(call install-data,$(src_base)/test.ops,$(install_doc_dir)/cli/$(path)/test.ops)

# Clean.
#
$(clean): $(driver).o.clean                            \
  $(addsuffix .cxx.clean,$(obj))                       \
  $(addsuffix .cxx.clean,$(dep))                       \
  $(addprefix $(out_base)/,$(cli:.cli=.cxx.cli.clean))

# Generated .gitignore.
#
ifeq ($(out_base),$(src_base))
$(gen): | $(out_base)/.gitignore
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
