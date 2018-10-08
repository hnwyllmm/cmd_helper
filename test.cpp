#include <cstdio>
#include <list>
#include "cmd_helper.h"

using namespace std ;

class cmd_param : public cmd_helper
{
public:
	CMD_PARSE_BEGIN
		DEFINE_OPT(m_debug, false, "d", "debug", cmd_helper::option, "open debug mode") ;
		DEFINE_OPT(m_cdrfile, "cdr.csv", "r", "cdr", cmd_helper::parameter, "cdr filename") ;
		DEFINE_OPT(m_loglvl, "info", NULL, "log-level", cmd_helper::parameter, "log level") ;
		DEFINE_OPT(m_logfile, "arg.log", "l", NULL, cmd_helper::parameter, "log file") ;
		DEFINE_OPT(m_cdrsize, 330, "s", NULL, cmd_helper::parameter, "cdr size") ;
	CMD_PARSE_END

	virtual int check()
	{
		const char *opt = unknown() ;
		if (opt)
			printf("invalid option: %s\n", opt) ;
		return 0 ;
	}
public:
	bool				m_debug ;
	std::string			m_cdrfile ;
	std::string			m_loglvl ;
	std::string			m_logfile ;
	int					m_cdrsize ;
	std::vector<int>	m_xidlist ;
};

int main(int argc, char *argv[])
{
	cmd_param cmd ;
	cmd.parse(argc, argv) ;
	printf("%d\n%s\n%s\n%s\n%d\n", 
			cmd.m_debug, 
			cmd.m_cdrfile.c_str(), 
			cmd.m_loglvl.c_str(), 
			cmd.m_logfile.c_str(), 
			cmd.m_cdrsize
			) ;

	cmd.get_param_without_option(cmd.m_xidlist) ;
	const int size = cmd.m_xidlist.size() ;
	for (int i = 0 ; i < size ; i++)
		printf("xid[%d]: %lld\n", i, cmd.m_xidlist[i]) ;
	// cmd.print_help() ;
	return 0 ;
}
