/*
 * (P3) Recursive Descent Parser_v1 (�Ĥ���) 2017.06.28
 *		- ���{���HP4�{�����򩳼��g�睊�A�����禡���ҧ��
 * Function:
 *		- Parsing�G�ھڮ���left recursion�᪺G1��k�غc�P�_���A���R��J��statement�O�_�X�k
 *	�@	- next_Token()�G�I�sP4��Lexical Analyzer�����U�@��token
 *		- match()�G�N��etoken���O�_���T�A�éI�snext_Token()�����U��token
 *		- �Y�J�줣�X�k���p�A�|����Parsing�A�æL�X��e���涥�q
 *		- [�P��P4�{��]����token�ɷ|����B��syntax error�^��(�HRegular expression�P�w�榡�O�_�ŦX)
 * Input:
 *	�@	-PPT����test statements�G
 *			- (s1) a = b + c * d
 *			- (s2) day = (1461*2) + 4.5 + (153*6+2) < 5 + 3
 *			- (s3) 123 =  (123 +* 4 ) 5 * 6
 *			- (s4) �t�_�����ѡA�G���������K�K�W
 * Output:
 *		-�̷ӵ{�������X��e������i��
 *			-[Matching]function_name
 *			-terminal Matched
 *			-Production Rule
 *		-�Y�J�줣�X�k���p�A�|��XError�T��
 *			-[Syntax Error]...
 *			-Parsing stops at:...
 *		-���t�檩���ΥH��X�����
 * Return:
 *		-0:�{������
 */
#include <iostream>
#include <string>
#include <regex>
using namespace std;
void expr();
void rel();
void rel_tail();
void add();
void add_tail();
void term();
void term_tail();
void factor();
string type_list[10]={"","num","id","=","<","<=","+","*","(",")"};
string peek;
string str,lookahead;
string error_str;
void init()	//��l�ƥ����ܼ�
{
	peek="";
	str="";
	lookahead="";
	error_str="";
}
void del_comment()	//�簣����
{
	string s="";
	for(int i=0;i<str.length();i++)
	{
		if(str[i]=='/' && str[i+1]=='/') {break;}	//�Y���ѬO"//"���榡�A�h��泣����
		else if(str[i]=='/' && str[i+1]=='*')		//�Y���ѬO"/*...*/"���榡
		{
			for(int j=i+2;j<str.length();j++)
			{
				if(str[j-1]=='*' && str[j]=='/')	//�����Ѫ�����
				{
					i=j;	//�j��U�@���N�q�L���U�@�Ӷ}�l�~��B�@
					break;
				}
			}
		}
		else{s+=str[i];}	//��l�r���Ҥ���
	}
	str=s;
	//cout<<"(1)�R������:"<<str<<endl;
}
void del_space()	//�簣�ť�
{
	string s="";
	for(int i=0;i<str.length();i++)
	{
		if(str[i]==' '){continue;}	//�J��ťմN���L
		else{s+=str[i];}			//��l�r���Ҥ���
	}
	str=s;
	//cout<<"(2)�R���ť�:"<<str<<endl;
}
string next_Token()	//�B�z�Ʀr�B�r��B�Ÿ��A���Xtoken�G�Ѧ�PPT(02)page.101�νҥ�page2-43
{
	if(str.length()==0){return "EOF.(1)";}	//��yInput�w�P�_����
	else
	{
		int i(0),t_type(0);
		peek="";	//�M��peek
		string s="";
		if(isdigit(str[i]))	//�B�z�Ʀr�G�ѦҤ��媩�ҥ���2.6.3�� (page.2-42)
		{	
			peek+=str[i];
			while(i+1<str.length())
			{
				//�wŪ�U�Ӧr���A�Y���Ʀr�N�[�Jpeek
				if(isdigit(str[i+1]) || str[i+1]=='.')
				{
					i++;
					peek+=str[i];
				}
				else{break;}
			}
			//�HRegular Expression�P�_�O�_match NUM���榡
			regex pattern("[0-9]+(\.[0-9]+)?");
			bool match=regex_match(peek,pattern);
			if(match){
				//cout<<"NUM matched - "<<peek<<endl;
				t_type=1;
			}
			else{cout<<peek<<"\tError-NUM not matched."<<endl;}	//��match�ɡA�L�XError Msg
		}
		else if(isalpha(str[i]))	//�B�zID�G�ѦҤ��媩�ҥ���2.6.4�� (page.2-43)
		{
			peek+=str[i];
			while(i+1<str.length())
			{
				//�wŪ�A�Y���r��/�Ʀr/���u�N�[�Jpeek
				if(isalpha(str[i+1]) || isdigit(str[i+1]) || str[i+1]=='_')
				{
					i++;
					peek+=str[i];
				}
				else{break;}
			}
			//�HRE�P�_�O�_match ID���榡
			regex pattern("[a-zA-Z][a-zA-Z_0-9]*");
			bool match=regex_match(peek,pattern);
			if(match){
				//cout<<"ID matched - "<<peek<<endl;
				t_type=2;
			}
			else{cout<<peek<<"\tError-ID not matched."<<endl;}	//��match�ɡA�L�XError Msg
		}
		else if(str[i]=='='){t_type=3;}
		else if(str[i]=='<' && str[i+1]!='='){t_type=4;}
		else if(str[i]=='<' && str[i+1]=='='){t_type=5;i=i+1;}
		else if(str[i]=='+'){t_type=6;}
		else if(str[i]=='*'){t_type=7;}
		else if(str[i]=='('){t_type=8;}
		else if(str[i]==')'){t_type=9;}

		for(int j=i+1;j<str.length();j++) {s+=str[j];}	//��s�|���B�z��statement�r��
		str=s;
		//�YRE�ֹ��L�~�A�h�Φ�token�æ^��token type�Mlexemes (P4��output)
		//if(t_type!=0){return "<"+type_list[t_type]+","+peek+">";}
		if(t_type!=0){return type_list[t_type];}
		else {return "EOF.(2)";}
	}
}
//�Ѧҽҵ{PPT(02)page.68-70�Τ���ҥ�Ch2.4.2
int match(string t)
{
	if(lookahead==t)
	{
		//��s���槹��statement�r��]�Ω�error�ɡA�i�L�X��e���涥�q�^
		if(t=="num" || t=="id")
		{
			error_str+=peek;
			cout<<"\t"<<peek<<" Matched"<<endl;
		}
		else{
			error_str+=t;
			cout<<"\t"<<t<<" Matched"<<endl;
		}
		lookahead=next_Token();	//�����U�@��token�A��slookahead(���ЦV�Ჾ������)
		return 1;
	}
	else{return 0;}
}
/* 
 * [����left recursion�᪺G1��k]
 *
 * expr -> rel = expr
 * expr -> rel
 * rel -> add rel_tail
 * rel_tail -> < add rel_tail
 * rel_tail -> <= add rel_tail
 * rel_tail -> null
 * add -> term add_tail
 * add_tail -> + term add_tail
 * add_tail -> null
 * term -> factor term_tail
 * term_tail -> * factor term_tail
 * term_tail -> null
 * factor -> ( expr )
 * factor -> num
 * factor -> id
 */
void expr()
{
	//expr -> rel = expr
	//expr -> rel
	cout<<"[Matching]expr()"<<endl;
	rel();
	if(lookahead=="=")
	{
		match("=");
		cout<<"   expr -> rel = expr"<<endl;
		expr();
	}
	else{cout<<"   expr -> rel"<<endl;}
}
void rel()
{
	//rel -> add rel_tail
	cout<<"[Matching]rel()"<<endl;
	add();
	rel_tail();
}
void rel_tail()
{
	//rel_tail -> '<' add rel_tail
	//rel_tail -> '<=' add rel_tail
	//rel_tail -> null
	cout<<"[Matching]rel_tail()"<<endl;
	if(lookahead=="<")
	{
		match("<");
		cout<<"   rel_tail -> < add rel_tail"<<endl;
		add();
		rel_tail();
	}
	else if(lookahead=="<=")
	{
		match("<=");
		cout<<"   rel_tail -> <= add rel_tail"<<endl;
		add();
		rel_tail();
	}
	else{cout<<"   rel_tail -> null"<<endl;}
}
void add()
{
	//add -> term add_tail
	cout<<"[Matching]add()"<<endl;
	term();
	add_tail();
}
void add_tail()
{
	//add_tail -> + term add_tail
	//add_tail -> null
	cout<<"[Matching]add_tail()"<<endl;
	if(lookahead=="+")
	{
		match("+");
		cout<<"   add_tail -> + term add_tail"<<endl;
		term();
		add_tail();
	}
	else{cout<<"   add_tail -> null"<<endl;}	
}
void term()
{
	//term -> factor term_tail
	cout<<"[Matching]term()"<<endl;
	factor();
	term_tail();
}
void term_tail()
{
	//term_tail -> * factor term_tail
	//term_tail -> null
	cout<<"[Matching]term_tail()"<<endl;
	if(lookahead=="*")
	{
		match("*");
		cout<<"   term_tail -> * factor term_tail"<<endl;
		factor();
		term_tail();
	}
	else{cout<<"   term_tail -> null"<<endl;}
}
void factor()
{
	//factor -> ( expr )
	//factor -> num
	//factor -> id
	cout<<"[Matching]factor()"<<endl;
	if(lookahead=="(")
	{
		match("(");
		cout<<"   factor -> ( expr )"<<endl;
		expr();
		match(")");
	}
	else if(lookahead=="num")
	{
		match("num");
		cout<<"   factor -> num"<<endl;
	}
	else if(lookahead=="id")
	{
		match("id");
		cout<<"   factor -> id"<<endl;
	}
	else
	{
		error_str+=lookahead;
		lookahead="EOF.(3)";	//�䤣��۲Ū�terminal�A�����Nlookahead���������Ϊ�token�A�H����parsing
		cout<<"\t[ERROR]���O�ŦX��terminal"<<endl;
		cout<<"\t[parsing stops at: "<<error_str<<"]"<<endl;
	}
}
int main()
{
	while(1)
	{
		init();					//Reset�����ܼ�
		cout<<"Input the statement:"<<endl;
		getline(cin,str);		//Ū�Jstatement
		if(str=="0"){break;}	//����K��X�ɮסA�]�m��J0�ɡA�����{��
		del_comment();			//�簣����
		del_space();			//�簣�ť�
		lookahead=next_Token();

		expr();					//�}�l����Parsing
		if(lookahead=="EOF.(1)")	//��ܥ��`�����]��ystatementŪ���^
		{
			cout<<"��statement�X�k"<<endl;
			cout<<"Success."<<endl;
		}
		else if(lookahead=="EOF.(2)")	//���X�k�]���J���ŦXRegular Expression�P�w�^
		{
			cout<<endl;
			cout<<"��statement���X�k"<<endl;
			cout<<"[Syntax Error(1)���J] Failed."<<endl;
		}
		else	//���X�k�]���c���ŦXG1��k�^�A�æL�X�B�z�פ�q
		{
			cout<<endl;
			cout<<"��statement���X�k"<<endl;
			cout<<"[Syntax Error(2)��k] Failed."<<endl;
			cout<<"Parsing stops at:"<<error_str<<endl;
		}
		cout<<endl;
		cout<<"=================================================="<<endl;
	}
	
    return 0;
}