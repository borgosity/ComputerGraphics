// standard library includes
#include <iostream>
// source includes
#include "tests\AssessmentApp.h"

/// main function
int main()
{
	// iniitlaise application
	AssessmentApp * app = new AssessmentApp();

	// run app if not null
	if (app != nullptr)
	{
		app->run();
	}

	// delete app when done
	delete app;

	return 0;
}

