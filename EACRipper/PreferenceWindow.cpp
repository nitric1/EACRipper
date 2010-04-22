#include "Defaults.h"

#include "MainController.h"
#include "MainWindow.h"
#include "PreferenceWindow.h"

using namespace std;

namespace EACRipper
{
	PreferenceWindow::PreferenceWindow()
		: Window(nullptr), conf(&Configure::instance())
	{
	}

	PreferenceWindow::~PreferenceWindow()
	{
	}

	bool PreferenceWindow::show()
	{
		return false;
	}
}
