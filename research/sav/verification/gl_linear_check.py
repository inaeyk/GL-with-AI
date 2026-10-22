#!/usr/bin/env python3
"""Entry point for exact nonproduction GL-mode checks.

The initial direct 5D SymPy expression simplification was terminated as
unnecessarily expensive; the retained implementation uses exact rational
functions and independently evaluates all reduced 5D Ricci components.
"""
import platform
import sympy
from gl_linear_exact import check

if __name__=='__main__':
    print('Python',platform.python_version(),'SymPy',sympy.__version__,flush=True)
    check()
