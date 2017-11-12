import distribute_setup
distribute_setup.use_setuptools()
from setuptools import setup, Extension, find_packages

classifiers = ['Development Status :: 3 - Alpha',
               'Operating System :: POSIX :: Linux',
               'License :: OSI Approved :: MIT License',
               'Intended Audience :: Developers',
               'Programming Language :: Python :: 2.7',
               'Programming Language :: Python :: 3',
               'Topic :: Software Development',
               'Topic :: Home Automation',
               'Topic :: System :: Hardware']

setup(name             = 'CHIP_IO',
      version          = '0.7.1',
      author           = 'Robert Wolterman',
      author_email     = 'robert.wolterman@gmail.com',
      description      = 'A module to control CHIP IO channels',
      long_description = open('README.rst').read() + open('CHANGELOG.rst').read(),
      license          = 'MIT',
      keywords         = 'CHIP NextThingCo IO GPIO PWM ADC GR8 R8',
      url              = 'https://github.com/xtacocorex/CHIP_IO/',
      classifiers      = classifiers,
      packages         = find_packages(),
      ext_modules      = [Extension('CHIP_IO.GPIO', ['source/py_gpio.c', 'source/event_gpio.c', 'source/constants.c', 'source/common.c'], extra_compile_args=['-Wno-format-security']),
                          Extension('CHIP_IO.PWM', ['source/py_pwm.c', 'source/c_pwm.c', 'source/constants.c', 'source/common.c'], extra_compile_args=['-Wno-format-security']),
                          Extension('CHIP_IO.SOFTPWM', ['source/py_softpwm.c', 'source/c_softpwm.c', 'source/constants.c', 'source/common.c', 'source/event_gpio.c'], extra_compile_args=['-Wno-format-security']),
                          Extension('CHIP_IO.SERVO', ['source/py_servo.c', 'source/c_softservo.c', 'source/constants.c', 'source/common.c', 'source/event_gpio.c'], extra_compile_args=['-Wno-format-security'])]) #,
#                          Extension('CHIP_IO.ADC', ['source/py_adc.c', 'source/c_adc.c', 'source/constants.c', 'source/common.c'], extra_compile_args=['-Wno-format-security']),
