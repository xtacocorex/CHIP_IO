## CHIP_IO.PWM
Import the PWM module as follows

  ```python
  import CHIP_IO.PWM as PWM
  ```

For the CHIP, this requires the PWM0 DTBO loaded via the OverlayManager or other means.
For the CHIP, PWM1 is unavaiable
For the CHIP Pro, PWM0 and PWM1 are setup in the base DTB by default

### toggle_debug()
Enable/Disable the Debug

* Parameters
  
  None

* Examples

  ```python
  PWM.toggle_debug()
  ```

### is_chip_pro()
Function to report to the calling script if the SBC is a CHIP or a CHIP Pro

* Parameters
  
  None

* Returns

  int - 1 for CHIP Pro, 0 for CHIP

* Examples

  ```python
  is_chip_pro = PWM.is_chip_pro()
  ```

### start(channel, duty_cycle=0.0, frequency=2000.0, polarity=0)
Start the Software PWM

* Parameters
  
  channel - pin for software PWM is configured 
  duty_cycle - initial duty cycle of the PWM (optional)
  frequency - frequency of the PWM (optional)
  polarity - signal polarity of the PWM (optional)

* Returns

  None

* Examples

  ```python
  PWM.start("PWM0")
  PWM.start("PWM0", 37.0)
  PWM.start("PWM0", 10.0, 500.0)
  PWM.start("PWM0", 50.0, 1000.0, 1)
  ```

### stop(channel)
Stop the PWM

* Parameters
  
  channel - pin PWM is configured 

* Returns

  None

* Examples

  ```python
  PWM.stop("PWM0")
  ```

### set_duty_cycle(channel, duty_cycle)
Set the duty cycle of the PWM

* Parameters
  
  channel - pin PWM is configured 
  duty_cycle - duty cycle of the PWM (0.0 to 100.0)

* Returns

  None

* Examples

  ```python
  PWM.set_duty_cycle("PWM0", 25.0)
  ```

### set_pulse_width_ns(channel, pulse_width_ns)
Set the width of the PWM pulse in nano seconds

* Parameters
  
  channel - pin PWM is configured 
  pulse_width_ns - pulse width of the PWM in nanoseconds

* Returns

  None

* Examples

  ```python
  PWM.set_pulse_width_ns("PWM0", 2500.0)
  ```

### set_frequency(channel, frequency)
Set the frequency of the PWM in Hertz

* Parameters
  
  channel - pin software PWM is configured
  frequency - frequency of the PWM

* Returns

  None

* Examples

  ```python
  PWM.set_frequency("PWM0", 450.0)
  ```

### set_period_ns(channel, pulse_width_ns)
Set the period of the PWM pulse in nano seconds

* Parameters
  
  channel - pin PWM is configured 
  period_ns - period of the PWM in nanoseconds

* Returns

  None

* Examples

  ```python
  PWM.set_period_ns("PWM0", 130.0)
  ```

### cleanup(channel)
Cleanup PWM.  If not channel input, all PWM will be cleaned up

* Parameters
  
  channel - pin PWM is configured (optional)

* Returns

  None

* Examples

  ```python
  PWM.cleanup()
  PWM.cleanup("PWM0")
  ```

[home](./index.md)
