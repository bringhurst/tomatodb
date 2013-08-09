include $(top_srcdir)/common.mk

all:
	-(cd $(top_srcdir)/src/havenctl && $(PYTHON) setup.py build)

install:
	-(cd $(top_srcdir)/src/havenctl && $(PYTHON) setup.py install)

develop:
	-(cd $(top_srcdir)/src/havenctl && $(PYTHON) setup.py develop)

clean-local:
	-(cd $(top_srcdir)/src/havenctl && rm -rf build)
	-(cd $(top_srcdir)/src/havenctl && rm -rf dist)
	-(cd $(top_srcdir)/src/havenctl && rm -rf havenctl.egg-info)
