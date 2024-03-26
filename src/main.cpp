#include "HttpServer/httpServer.h"
#include "LeticoCamera/leticoCamera.h"
#include "Menu/menu.h"

int main()
{
	LeticoCamera camera;

	HttpServer http({std::make_shared<LeticoCamera>(camera)});

	Menu menu(std::make_shared<LeticoCamera>(camera));
	menu.showOptions();

	return 0;
}
