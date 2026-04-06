#include "Application.hpp"

GLFWwindow* initOpenGL();

int main(int argc, char **argv)
{
	if (argc == 2)
	{
			Application app(argv[1]);
			app.run();
		try
		{
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << '\n';
		}
	}
	else
	{
		std::cerr << "Need one *.obj file as input" << std::endl;
		return (0);
	}
}