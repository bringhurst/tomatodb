include $(top_srcdir)/common.mk

all-local:
	echo 'ALL LOCAL WAS CALLED***************'

clean-local:
	-(cd $(top_srcdir)/src/havenctl && rm -rf build)
	-(cd $(top_srcdir)/src/havenctl && rm -rf dist)
	-(cd $(top_srcdir)/src/havenctl && rm -rf havenctl.egg-info)
