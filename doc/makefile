# file      : doc/makefile
# author    : Boris Kolpackov <boris@codesynthesis.com>
# copyright : Copyright (c) 2009 Code Synthesis Tools CC
# license   : MIT; see accompanying LICENSE file

include $(dir $(lastword $(MAKEFILE_LIST)))../build/bootstrap.make

default  := $(out_base)/
install  := $(out_base)/.install
cleandoc := $(out_base)/.cleandoc

$(default):

# Install.
#
$(install):
	$(call install-data,$(src_base)/default.css,$(install_doc_dir)/cli/default.css)
	$(call install-data,$(src_base)/cli.xhtml,$(install_doc_dir)/cli/cli.xhtml)
	$(call install-data,$(src_base)/cli.1,$(install_man_dir)/man1/cli.1)

# Clean.
#
$(cleandoc):

$(call include,$(bld_root)/install.make)
