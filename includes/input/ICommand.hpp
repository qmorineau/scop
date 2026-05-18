#ifndef ICOMMAND_HPP
#define ICOMMAND_HPP

class Application;

class ICommand
{
	public:
		virtual ~ICommand() {};
		virtual void execute(Application* app) const = 0;
};

#endif