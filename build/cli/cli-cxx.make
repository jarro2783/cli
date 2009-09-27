# file      : build/cli/cli-cxx.make
# author    : Boris Kolpackov <boris@codesynthesis.com>
# copyright : Copyright (c) 2009 Code Synthesis Tools CC
# license   : MIT; see accompanying LICENSE file

#@@ Need to use extensions from cxx config.
#

cli_pattern :=        \
$(out_base)/%.cxx     \
$(out_base)/%.hxx     \
$(out_base)/%.ixx     \
$(out_base)/%-fwd.hxx

$(cli_pattern): cli := cli
$(cli_pattern): cli_options :=

.PRECIOUS: $(cli_pattern)

$(cli_pattern): $(out_base)/%.cli | $$(dir $$@).
	$(call message,cli $<,$(cli) $(cli_options) --output-dir $(dir $@) $<)

ifneq ($(out_base),$(src_base))
$(cli_pattern): $(src_base)/%.cli | $$(dir $$@).
	$(call message,cli $<,$(cli) $(cli_options) --output-dir $(dir $@) $<)
endif

.PHONY: $(out_base)/%.cxx.cli.clean

$(out_base)/%.cxx.cli.clean:
	$(call message,rm $(@:.cxx.cli.clean=.cxx),rm -f $(@:.cxx.cli.clean=.cxx))
	$(call message,rm $(@:.cxx.cli.clean=.hxx),rm -f $(@:.cxx.cli.clean=.hxx))
	$(call message,rm $(@:.cxx.cli.clean=.ixx),rm -f $(@:.cxx.cli.clean=.ixx))
	$(call message,rm $(@:.cxx.cli.clean=-fwd.hxx),rm -f $(@:.cxx.cli.clean=-fwd.hxx))
