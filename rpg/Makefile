ALL: RPG Server RPG_WIN
	
RPG: 
	g++ -Wall main.cpp gl.cpp client.cpp world.cpp world_define.h -o rpg -lGL -lGLU -lglut -lpthread

Server:
	g++ -Wall server_main.cpp server.cpp world.cpp world_define.h -o server -lpthread

RPG_WIN:
	x86_64-w64-mingw32-g++ -Wall main.cpp gl.cpp client.cpp world.cpp world_define.h -o rpg_win.exe -lopengl32 -lfreeglut -lglu32 -lpthread -L"/home/dickyhaha/freeglut_win/lib/" -I"/home/dickyhaha/freeglut_win/include/"

Clean:
	rm rpg server rpg_win.exe
