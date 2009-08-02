# file      : build/bootstrap.make
# author    : Boris Kolpackov <boris@codesynthesis.com>
# copyright : Copyright (c) 2009 Code Synthesis Tools CC
# license   : TBD

project_name := CLI

include build-0.3/bootstrap.make


# Aliases
#
ifdef %interactive%

.PHONY: test $(out_base)/.test \
        install $(out_base)/.install \
        dist $(out_base)/.dist \
        clean $(out_base)/.clean

test: $(out_base)/.test
install: $(out_base)/.install
dist: $(out_base)/.dist
clean: $(out_base)/.clean

ifneq ($(filter $(.DEFAULT_GOAL),test install dist clean),)
.DEFAULT_GOAL :=
endif

endif


# Make sure the distribution prefix is set if the goal is dist.
#
ifneq ($(filter $(MAKECMDGOALS),dist),)
ifeq ($(dist_prefix),)
$(error dist_prefix is not set)
endif
endif


# Don't include dependency info for certain targets.
#
define include-dep
$(call -include,$1)
endef

ifneq ($(filter $(MAKECMDGOALS),clean disfigure),)
include-dep =
endif
