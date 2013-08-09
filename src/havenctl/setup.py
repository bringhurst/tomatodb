#!/usr/bin/env python

from setuptools import setup, find_packages

setup (
    name               = 'havenctl',
    version            = '0.0.1-alpha.1',
    author             = 'Jon Bringhurst',
    author_email       = 'jonb@lanl.gov',
    url                = 'http://bringhurst.org/haven',
    scripts            = ['havenctl/bin/havenctl'],
    description        = 'A utility to control a haven daemon.',
    packages           = find_packages(),
)

#from pylint.lint import Run
#Run(['--errors-only', 'havenctl']) 
