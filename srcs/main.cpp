#include "Object.hpp"
#include "scop.hpp"

int main(int argc, char **argv)
{
	if (argc == 2)
	{
		Object *obj = new Object(std::string(argv[1]));
		try
		{
			obj->parse();
			obj->test();
			obj->render();
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << std::endl;
			return (1);
		}
		return (0);
	}
	else
	{
		std::cerr << "Need a .obj file as input" << std::endl;
		return (0);
	}
}