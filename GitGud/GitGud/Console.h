#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include <string>
#include <vector>
#include <map>

struct Command
{
	Command()
	{}

	Command(const char* commandName, const char* commandStr, const char* commandDescription = nullptr) : commandName(commandName), commandStr(commandStr), commandDescription((commandDescription != nullptr) ? commandDescription : "")
	{}

	virtual void Function(std::vector<std::string>& args) = 0;
	virtual void Undo()
	{}

	std::string commandName;
	std::string commandStr;
	std::string commandDescription;
};


class Console
{
public:
	Console();
	virtual ~Console();

	void OnCmdSubmision(const char* str);

	bool AddCommand(Command* cmd);
	Command* GetCommandByName(const char* commandName);
	Command* GetCommandByStr(const char* commandStr);

private:
	std::map<std::string, Command*> commands;

	struct CListCmd : public Command
	{
		CListCmd() : Command("List commands", "list_cmd", "List all commands. -d: show description")
		{}
		void Function(std::vector<std::string>& args)override;
	}cListCmd;

	struct CHelp : public Command
	{
		CHelp() : Command("Help", "help", "If no arguments list all commands. If there is an argument show that command description")
		{}
		void Function(std::vector<std::string>& args)override;
	}cHelp;
};


#endif // !__CONSOLE_H__