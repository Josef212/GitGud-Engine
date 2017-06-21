#include "Console.h"
#include "Globals.h"
#include "App.h"


Console::Console()
{
	AddCommand(&cListCmd);
	AddCommand(&cHelp);
}


Console::~Console()
{
}

void Console::OnCmdSubmision(const char * str)
{
	if (!str)
		return;

	std::vector<std::string> args;
	std::string cmd(str);
	unsigned int start = 0;
	bool inSpace = false;

	for (unsigned int i = 0; i < cmd.length(); ++i)
	{
		if (!inSpace)
		{
			if (cmd[i] == ' ')
			{
				args.push_back(cmd.substr(start, i - start));
				inSpace = true;
			}
		}
		else
		{
			if (cmd[i] != ' ')
			{
				inSpace = false;
				start = i;
			}
		}
	}

	if (!inSpace)
	{
		args.push_back(cmd.substr(start, cmd.length() - start));
	}

	if (!args.empty())
	{
		Command* c = GetCommandByStr(args[0].c_str());
		if (c)
		{
			_LOG(cmd.c_str());
			args.erase(args.begin());
			c->Function(args);
			//TODO: Push command to vector
		}
		else
		{
			_LOG("ERROR: Could not find command [%s].", args[0].c_str());
		}
	}
	
}

bool Console::AddCommand(Command * cmd)
{
	bool ret = false;

	if (cmd)
	{
		std::map<std::string, Command*>::iterator it = commands.find(cmd->commandStr);
		if (it != commands.end())
		{
			//Error trying to add an existin command
			_LOG("CONSOLE_ERROR: Trying to add an existing commandstr.");
		}
		else
		{
			commands.insert(std::pair<std::string, Command*>(cmd->commandStr, cmd));
			ret = true;
		}
	}

	return ret;
}

Command * Console::GetCommandByName(const char * commandName)
{
	std::map<std::string, Command*>::iterator it = commands.begin();
	for (; it != commands.end(); ++it)
	{
		if (it->second->commandName == commandName)
			return it->second;
	}

	return nullptr;
}

Command * Console::GetCommandByStr(const char * commandStr)
{
	std::map<std::string, Command*>::iterator it = commands.find(commandStr);
	return (it != commands.end()) ? it->second : nullptr;
}

void Console::CListCmd::Function(std::vector<std::string>& args)
{
	std::vector<std::string>::iterator tmp = std::find(args.begin(), args.end(), "-d");

	bool showDesc = tmp != args.end();

	for (std::map<std::string, Command*>::iterator it = app->console->commands.begin(); it != app->console->commands.end(); ++it)
	{
		Command* c = it->second;
		if (c)
		{
			_LOG("Command name: %s.  Str: %s.", c->commandName.c_str(), c->commandStr.c_str());
			if (showDesc)
			{
				_LOG("Description: %s.", c->commandDescription.c_str());
			}
		}
		_LOG("-------------");
	}
	_LOG("==================================");
}

void Console::CHelp::Function(std::vector<std::string>& args)
{
	if (args.empty())
	{
		app->console->cListCmd.Function(std::vector<std::string>(0));
	}
	else
	{
		Command* c = app->console->GetCommandByStr(args[0].c_str());
		if (c)
		{
			_LOG("Command name: %s.", c->commandName.c_str());
			_LOG("Command str: %s.", c->commandStr.c_str());
			_LOG("Command description: %s.", c->commandDescription.c_str());
		}
	}
}
