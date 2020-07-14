Server Launch
-Run the executable
	-Port is set to 30000
Client Launch
-Console Command
	-"FlotillaClient.exe [ip] [port]" sets target server to [ip] at port [port]
	-"FlotillaClient.exe [ip]" sets target server to [ip] at port 30000
	-"FlotillaClient.exe" sets target server to 127.0.0.1 at port 30000
-Batch file
	-One is included with my testing server. Edit as needed, but also maybe try it as is?

Project Running Dependencies
-Windows Computer
-Ability to run OpenGL 3.3 or higher

Project Build Dependencies
-glad
-GLFW
-glm

Client Controls
-WASD: movement
-Space: remove blue circle
-Escape: quit client