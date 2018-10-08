#ifndef _COMMAND_ARGUMENT_H
#define _COMMAND_ARGUMENT_H

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <vector>
#include <sstream>
#include <string>

class cmd_helper
{
public:
	enum cmd_type
	{
		option 	= 0,
		parameter,
	};

public:
	cmd_helper()
	{
	}
	
protected:
	template<class var_t, class def_t>
	int	_parse(var_t &var, const def_t &def, const char *abbr, const char *opt, cmd_helper::cmd_type type)
	{
		// parameters without option		
		if ((!abbr || (abbr && 0 == abbr[0])) &&
			(!opt) || (opt  && 0 == opt[0]) )
			return -1 ;

		bool found = false ;
		const char *pval = NULL ;
		int i = 0 ;
		for (i = 1 ; i < m_argc ; i++)
		{
			const char *p = m_argv[i] ;
			if ('-' == p[0])
			{
				if ('-' == p[1] )			// long option(--)
				{
					if (opt && 0 == strcasecmp(opt, p + 2))
					{
						found = true ;
						m_argv_none[i] = true ;
						if (i + 1 < m_argc)
						{
							pval = m_argv[i+1] ;
							
							if (cmd_helper::parameter == type)
							{
								m_argv_none[i+1] = true ;
							}
						}
						break ;
					}
				}							// short option(-)
				else if (abbr && abbr[0] == p[1])
				{
					found = true ;
					m_argv_none[i] = true ;
					if (*(p+2))
					{
						if ('=' == *(p+2))	// short: -s=`value'
							pval = p + 3 ;
						else
							pval = p + 2 ;	// short: -s`value'
					}
					else if (i + 1 < m_argc)// short: -s `value'
					{
						pval = m_argv[i+1] ;
						
						if (cmd_helper::parameter == type)
						{
							m_argv_none[i+1] = true ;
						}
					}
					break ;
				}
			}
		}

		std::stringstream ss ;
		if (false == found)
		{
			var = def ;
			return 0 ;
		}
		switch (type)
		{
		case cmd_helper::option:
			var = true ;
			break ;
		case cmd_helper::parameter:
			if (pval)
			{
				ss << pval ;
				ss >> var ;
			}
			break ;
		default:
			return -1 ;
		}
		return 0 ;
	}
	
public:
	virtual int check()
	{
		const char *opt = unknown() ;
		if (opt)
		{
			printf("invalid optiont: %s\n", opt) ;
			exit(1) ;
		}
		return 0 ;
	}
	
	const char *unknown() const
	{
		for (int i = 1 ; i < m_argc ; i++)
		{
			if (false == m_argv_none[i] && m_argv[i][0] == '-')
			{
				return m_argv[i] ;
			}
		}
		return NULL ;
	}
	
	template<class list_t>
	void get_param_without_option(list_t &lst)
	{
		for (int i = 1 ; i < m_argc ; i++)
		{
			if (m_argv_none[i])
				continue ;

			if ('-' == m_argv[i][0])	// some parameters cannot be recogized
			{
				lst.clear() ;
				return ;
			}
			
			std::stringstream ss ;
			ss << m_argv[i] ;
			lst.resize(lst.size() + 1) ;
			ss >> lst.back() ;
		}
	}

public:
	virtual void print_help()
	{
		std::vector<cmd_help>::const_iterator iter = m_cmd_help.begin() ;
		std::vector<cmd_help>::const_iterator end  = m_cmd_help.end() ;
		for ( ; iter != end ; ++iter)
		{
			if (false == iter->_abbr.empty())
				printf("-%s", iter->_abbr.c_str()) ;
			if (false == iter->_opt.empty())
				printf("\t--%s", iter->_opt.c_str()) ;
			const int count = 3 - (2 + iter->_opt.size()) / 4 ;
			for (int i = 0 ; i < count ; i++)
				printf("\t") ;
			if (false == iter->_help.empty())
				printf("%s", iter->_help.c_str()) ;
			printf("\n") ;
		}
	}
protected:
	struct cmd_help
	{
		std::string		_def ;
		std::string		_abbr ;
		std::string		_opt ;
		std::string		_help ;
	};
	std::vector<cmd_help>	m_cmd_help ;
	int 					m_argc ;
	char **					m_argv ;
	bool					m_help ;
	
	std::vector<bool>		m_argv_none ;
};

#define CMD_PARSE_BEGIN														\
public:																		\
int	parse(int argc, char *argv[])	{										\
	m_argc = argc ;															\
	m_argv = argv ;															\
	m_argv_none.resize(argc, false) ;										\
	m_argv_none[0] = true ;													\
																			\
	std::stringstream ss ;													\
	struct cmd_help cmd_help ;												\
	cmd_help._abbr = "h" ;													\
	cmd_help._opt = "help" ;												\
	cmd_help._help = "command information" ;								\
	m_cmd_help.push_back(cmd_help) ;										\

#define CMD_PARSE_END														\
	if (-1 == _parse(m_help, false, "h", "help", cmd_helper::option)) 		\
		return -1 ;															\
	if (m_help)	{print_help() ; exit(0) ;}									\
	return check() ;														\
}

#define DEFINE_OPT(name, def, abbr, opt, type, help)	{					\
	ss << def ;																\
	cmd_help._def = ss.str() ;												\
	if (abbr) cmd_help._abbr = (char*)abbr ;else cmd_help._abbr.clear() ;	\
	if (opt) cmd_help._opt = (char*)opt ;	else cmd_help._opt.clear() ;	\
	if (help) cmd_help._help = (char*)help ;else cmd_help._help.clear() ;	\
	m_cmd_help.push_back(cmd_help) ;										\
	if (-1 == _parse(name, def, abbr, opt, type)) return -1 ;				\
}

#endif // _COMMAND_ARGUMENT_H
