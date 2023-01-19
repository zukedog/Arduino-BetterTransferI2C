# Better Transfer I2C
Overhauled by Zukedog2000.
Forked from the library [Easy Transfer]() by [Bill Porter](www.billporter.info) and Mathieu Alorent.

Better Transfer I2C is a library for trasfering entire structs from one Arduino to another using the I2C bus. Based on the I2C implementation from the library [Easy Transfer](), Better Transfer I2C has been overhauled to enable transfering the structs not only from the I2C Master to the I2C Slave but from the I2C Slave to the I2C Master as well. This enables bi-directional communication and the ability to keep variables on 2 Arduinos in sync. Better Transfer I2C has also improved upon the protocol used to communicate to enable multiple different structs to be sent between the Arduinos. This means that you can have multiple structs synced between multiple Arduinos.



This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
<http://www.gnu.org/licenses/>

This work is licensed under the Creative Commons Attribution-ShareAlike 3.0 Unported License.
To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/3.0/ or send a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
