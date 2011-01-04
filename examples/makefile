# file      : examples/makefile
# author    : Boris Kolpackov <boris@codesynthesis.com>
# copyright : Copyright (c) 2009-2011 Code Synthesis Tools CC
# license   : MIT; see accompanying LICENSE file

include $(dir $(lastword $(MAKEFILE_LIST)))../build/bootstrap.make

examples := hello features file

default   := $(out_base)/
install   := $(out_base)/.install
clean     := $(out_base)/.clean

$(default): $(addprefix $(out_base)/,$(addsuffix /,$(examples)))
$(install): $(addprefix $(out_base)/,$(addsuffix /.install,$(examples)))
	$(call install-data,$(src_base)/README,$(install_doc_dir)/cli/examples/README)
$(clean): $(addprefix $(out_base)/,$(addsuffix /.clean,$(examples)))

$(foreach e,$(examples),$(call import,$(src_base)/$e/makefile))
