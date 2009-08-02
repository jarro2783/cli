# file      : cli/makefile
# author    : Boris Kolpackov <boris@codesynthesis.com>
# copyright : Copyright (c) 2009 Code Synthesis Tools CC
# license   : TBD

include $(dir $(lastword $(MAKEFILE_LIST)))../build/bootstrap.make

cxx_tun := cli.cxx

#
#
cxx_obj  := $(addprefix $(out_base)/,$(cxx_tun:.cxx=.o))
cxx_od   := $(cxx_obj:.o=.o.d)

cli      := $(out_base)/cli
dist     := $(out_base)/.dist
clean    := $(out_base)/.clean
install  := $(out_base)/.install

# Build.
#
$(cli): $(cxx_obj)

$(cxx_obj) $(cxx_od): cpp_options := -I$(src_base)

$(call include-dep,$(cxx_od))

# Alias for default target.
#
.PHONY: $(out_base)/
$(out_base)/: $(cli)

# Dist.
#
.PHONY: $(dist)

# @@ TODO

# Install.
#
.PHONY: $(install)

$(install): $(cli)
	$(call install-exec,$<,$(install_bin_dir)/cli)

# Clean.
#
.PHONY: $(clean)

$(clean):                            \
  $(cli).o.clean                    \
  $(addsuffix .cxx.clean,$(cxx_obj)) \
  $(addsuffix .cxx.clean,$(cxx_od))


# Generated .gitignore.
#
ifeq ($(out_base),$(src_base))
$(cli): | $(out_base)/.gitignore

$(out_base)/.gitignore: files := cli
$(clean): $(out_base)/.gitignore.clean

$(call include,$(bld_root)/git/gitignore.make)
endif


# how to
#
$(call include,$(bld_root)/cxx/o-e.make)
$(call include,$(bld_root)/cxx/cxx-o.make)
$(call include,$(bld_root)/cxx/cxx-d.make)
$(call include,$(bld_root)/install.make)
