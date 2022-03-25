#include "pch.h"
#include "context.h"

int main()
{
	try
	{
		ctx().start();
	}
	catch (const std::exception& ex)
	{
		cout << ex.what() << endl;
	}

	return 0;
}
