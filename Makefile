# PyPi Packaging
package: clean
	@echo " ** PACKAGING FOR PYPI **"
	python setup.py sdist

# PyPi Publishing
publish: package
	@echo " ** UPLOADING TO PYPI **"
	twine upload dist/*

# Clean all the things
clean:
	@echo " ** CLEANING CHIP_IO **"
	rm -rf CHIP_IO.* build dist
	rm -f *.pyo *.pyc
	rm -f *.egg
	rm -rf __pycache__
	rm -rf test/__pycache__/
	rm -rf debian/python-chip-io*
	rm -rf debian/python3-chip-io*

# Run all the tests
tests: pytest2 pytest3

# Run the tests with Python 2
pytest2:
	@echo " ** RUNING CHIP_IO TESTS UNDER PYTHON 2 **"
	pushd test; python -m pytest; popd

# Run the tests with Python 3
pytest3:
	@echo " ** RUNING CHIP_IO TESTS UNDER PYTHON 3 **"
	pushd test; python3 -m pytest; popd

# Build all the things
build:
	@echo " ** BUILDING CHIP_IO: PYTHON 2 **"
	python setup.py build --force

# Install all the things
install: build
	@echo " ** INSTALLING CHIP_IO: PYTHON 2 **"
	python setup.py install --force

# Build for Python 3
build3:
	@echo " ** BUILDING CHIP_IO: PYTHON 3 **"
	python3 setup.py build --force

# Install for Python 3
install3: build3
	@echo " ** INSTALLING CHIP_IO: PYTHON 3 **"
	python3 setup.py install --force

# Install for both Python 2 and 3
all: install install3

# Create a deb file
debfile:
	@echo " ** BUILDING DEBIAN PACKAGES **"
	dpkg-buildpackage -rfakeroot -uc -b
