# file      : build/export/cli/stub.make
# author    : Boris Kolpackov <boris@codesynthesis.com>
# copyright : Copyright (c) 2009-2010 Code Synthesis Tools CC
# license   : MIT; see accompanying LICENSE file

$(call include-once,$(src_root)/cli/makefile,$(out_root))

# Use the rules file from cli's import directory instead of the
# importing project's one.
#
$(call export,\
  cli: $(out_root)/cli/cli,\
  cli-rules: $(scf_root)/import/cli/cli-cxx.make)
