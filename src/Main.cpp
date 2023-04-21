#include "Engine.h"


int main(int argc, char* argv[])
{
	Engine app;
	bool success = app.Init();
	if (success)
	{
		app.Run();
	}
	app.Quit();
	return 0;
}
