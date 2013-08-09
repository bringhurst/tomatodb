from setuptools import setup, find_packages

setup (
    name               = 'havenctl',
    version            = '0.0.1.alpha',
    author             = 'Jon Bringhurst',
    author_email       = 'jonb@lanl.gov',
    url                = 'https://www.github.com/hpc/haven',
    scripts            = ['bin/havenctl'],
    description        = 'A utility to control a haven daemon.',
    packages           = find_packages(),
)

#from pylint.lint import Run
#Run(['--errors-only', 'havenctl']) 
