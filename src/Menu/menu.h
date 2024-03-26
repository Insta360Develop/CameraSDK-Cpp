#include <memory>

#include "../LeticoCamera/leticoCamera.h"

class Menu
{
public:
	Menu(std::shared_ptr<LeticoCamera> camera);
	void showOptions();
	void displayMenu();
	int getUserInput();

private:
	std::shared_ptr<LeticoCamera> mCamera;

	void handleOption(int option);
};