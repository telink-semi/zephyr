.. _telink-b91-suspend-sample:

Telink B91 Suspend demo
#####################

Overview
********

This sample can be used for basic power measurement and as an example of
deep sleep on Telink B91 platforms.  The functional behavior is:

* Sleep for 2 seconds with device power control
* Print the current system time after wakeup

A power monitor will be able to distinguish among these states.

Requirements
************

This application uses tlsr9518adk80d board for the demo.

Building, Flashing and Running
******************************

.. zephyr-app-commands::
   :zephyr-app: samples/boards/telink/tlsr9518adk80d/pm_suspend
   :board: tlsr9518adk80d
   :goals: build flash
   :compact:

Running:

1. Open UART terminal.
2. Power Cycle Device.
3. Device will demonstrate two activity levels which can be measured.

Sample Output
=================
tlsr9518 core output
-----------------

.. code-block:: console

   *** Booting Zephyr OS build v2.3.0-rc1-204-g5f2eb85f728d  ***

   tlsr9518adk80d pm_suspend demo
   WakeUp system time: 4014 ms
   System time: 4015 ms
   Sleeping 2 s
   WakeUp system time: 6020 ms
   System time: 6021 ms
   Sleeping 2 s

