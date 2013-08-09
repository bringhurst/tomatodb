include $(top_srcdir)/common.mk

havenctl_srcdir = $(top_srcdir)/src/havenctl
all:
	-(cd $(havenctl_srcdir) && $(PYTHON) setup.py build)

install:
	-(cd $(havenctl_srcdir) && $(PYTHON) setup.py install)

develop:
	-(cd $(havenctl_srcdir) && $(PYTHON) setup.py develop)

clean-local:
	-(cd $(havenctl_srcdir) && rm -rf build dist havenctl.egg-info)
