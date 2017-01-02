package: clean
	python setup.py sdist

publish: package
	twine upload dist/*

clean:
	rm -rf CHIP_IO.* build dist
	rm -f *.pyo *.pyc
	rm -f *.egg
	rm -f overlays/*.pyo overlays/*.pyc
tests:
	py.test

build:
	python setup.py build --force

install: build
	python setup.py install --force
