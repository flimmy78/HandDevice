This repo consists of three applications:
	1. for concentrator, communicating with concentrator: 
		reading and setting parameters, 
		reading and setting meter file info, 
		reading history data, etc.
		its source code is under directory "Source.Concentrator",
		its MDK's files are under directory "Project.Concentrator".
	2. for heat meter and valve controller(only for delu's meter and YiLin's valve now), 
		reading heat meter and valve controller's data, 
		setting heat meter's flow-coefficient,
		opening and closing valve controller,
		reading valve controller's opening time, etc.
		its source code is under directory "Source.MBus",
		its MDK's files are under directory "Project.MBus".
	3. for comhelper
		user can send data to the bus(RS485 if existing, or MBus) directly.
		its source code is under directory "Source.ComHelper",
		its MDK's files are under directory "Project.ComHelper".
#############################################################################################

All three applications above use the same header files 
under the directory "Include".
#############################################################################################

"bin" contains database file with DBase format.
"docs" contains all documents for the whole repo.
"release" contains all file distributed to user.
#############################################################################################

git is an awesome tool!
#############################################################################################

For The Horde!
#########################################----END----#########################################