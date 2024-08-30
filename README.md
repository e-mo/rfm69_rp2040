# RFM69_RP2040 - 0.9.2
A zero-assumptions library for interfacing the RP2040 microcontroller with the RFM69 family of transceivers.
### Note on using this library
This library exposes a low level interface for interacting with the Rfm69 family of transceivers with the RP2040 microcontroller. Usage of this library assumes intimate knowledge of your radio's datasheet, and beyond some [configuration notes](docs/configuration.md) talking about a few specific pain points which I believe are poorly represented in the documentation, I do very little to explain how the Rfm69 transceivers function.  

---
## Low Level Interface
The core of this library is a low level interface with provides a thin level of abstraction over the SPI calls needed to configure the RFM69 family of transceivers.

[Low Level Interface Documentation](docs/interface.md)

note: this interface is incomplete and does not provide a helper function for setting/reading all registers, only those that were relevant to me at the time of writing the drivers. I welcome pull requests, and will likely fill in the remaining functionality myself at some point.

---
## RUDP Interface (beta)
This library also exposes a higher level RUDP (Reliable UDP) interface which greatly simplifies usage of the transceiver and provides a more robust protocol with a greater degree of transmission reliability.

[Reliable UDP Interface Documentation](docs/rudp_interface.md)

note: While the current state of this interface is a bit in flux, it currently works very well. Hopefully it stays that way. Does anyone actually know what they are doing?

---
## Examples
[low level tx/rx]  
[reliable UDP tx/rx](https://github.com/e-mo/rfm69_rp2040/tree/main/examples/rudp)

---
## Other Helpful Stuff
[Notes on Configuring RFM69](docs/configuration.md)  
[RFM69HCW Datasheet](https://cdn.sparkfun.com/datasheets/Wireless/General/RFM69HCW-V1.1.pdf)

---
If you need help or have a suggestion/question of any kind, contact me:  
<emorse@tech.dev>
