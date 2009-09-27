# file      : makefile
# author    : Boris Kolpackov <boris@codesynthesis.com>
# copyright : Copyright (c) 2009 Code Synthesis Tools CC
# license   : MIT; see accompanying LICENSE file

include $(dir $(lastword $(MAKEFILE_LIST)))build/bootstrap.make

default  := $(out_base)/
test     := $(out_base)/.test
install  := $(out_base)/.install
dist     := $(out_base)/.dist
clean    := $(out_base)/.clean


.PHONY: $(default) $(test) $(install) $(dist) $(clean)

# Build.
#
$(default): $(out_base)/cli/ $(out_base)/tests/ $(out_base)/examples/

# Test.
#
$(test): $(out_base)/tests/.test

# Install.
#
$(install): $(out_base)/cli/.install           \
            $(out_base)/documentation/.install
	$(call install-data,$(src_base)/LICENSE,$(install_doc_dir)/cli/LICENSE)
	$(call install-data,$(src_base)/NEWS,$(install_doc_dir)/cli/NEWS)
	$(call install-data,$(src_base)/README,$(install_doc_dir)/cli/README)

# Dist.
#

# @@ TODO

# Clean.
#
$(clean): $(out_base)/cli/.clean $(out_base)/tests/.clean

$(call include,$(bld_root)/install.make)

$(call import,$(src_base)/cli/makefile)
$(call import,$(src_base)/tests/makefile)
$(call import,$(src_base)/examples/makefile)
