/*
 * (P5) Infix to Postfix Conversion_v2 (第五組) 2017.06.30
 *		- 本程式以P3程式為基底撰寫改版
 * Function:
 *		- [同於P3]Parsing、next_Token()：根據G1文法建構判斷式，分析輸入的statement是否合法
 *		- match()：若為token是num或id，則直接加入postfix內
  *		- Translation actions：G1的所有運算符號，都在執行完該rule的下一個non-terminal後，再將符號加入postfix
 *		- 若遇到不合法情況，會結束Parsing和Conversion，並印出當前執行階段之postfix
 * Input:
 *	　	-PPT內的test statements：
 *			- (s1) a = b + c * d
 *			- (s2) day = (1461*2) + 4.5 + (153*6+2) < 5 + 3
 *			- (s3) 123 =  (123 +* 4 ) 5 * 6
 *			- (s4) 含巢狀註解，故說明內不便貼上
 * Output:
 *		-依照程式執行輸出當前的執行進度
 *			-Success/ Failed
 *			-Postfix form of the input
 *		-若遇到不合法情況，會輸出Error訊息與當前已處理的結果
 *			-Parsing stops at...
 *			-Conversion stops at...
 *		-有另行版本用以輸出資料檔
 * Return:
 *		-0:程式結束
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
string str,peek,lookahead;
string error_str,postfix_str;
void init()	//初始化全域變數
{
	peek="";
	str="";
	lookahead="";
	error_str="";
	postfix_str="";
}
void del_comment()	//剔除註解
{
	string s="";
	for(int i=0;i<str.length();i++)
	{
		if(str[i]=='/' && str[i+1]=='/') {break;}	//若註解是"//"的格式，則整行都不看
		else if(str[i]=='/' && str[i+1]=='*')		//若註解是"/*...*/"的格式
		{
			for(int j=i+2;j<str.length();j++)
			{
				if(str[j-1]=='*' && str[j]=='/')	//找到註解的結尾
				{
					i=j;	//迴圈下一輪就從他的下一個開始繼續運作
					break;
				}
			}
		}
		else{s+=str[i];}	//其餘字元皆不動
	}
	str=s;
	//cout<<"(1)刪除註解:"<<str<<endl;
}
void del_space()	//剔除空白
{
	string s="";
	for(int i=0;i<str.length();i++)
	{
		if(str[i]==' '){continue;}	//遇到空白就跳過
		else{s+=str[i];}			//其餘字元皆不動
	}
	str=s;
	//cout<<"(2)刪除空白:"<<str<<endl;
}
string next_Token()	//處理數字、字串、符號，提出token：參考PPT(02)page.101及課本page2-43
{
	if(str.length()==0){return "EOF.(1)";}	//整句Input已判斷結束
	else
	{
		int i(0),t_type(0);
		peek="";	//清空peek
		string s="";
		if(isdigit(str[i]))	//處理數字：參考中文版課本第2.6.3章 (page.2-42)
		{	
			peek+=str[i];
			while(i+1<str.length())
			{
				//預讀下個字元，若為數字就加入peek
				if(isdigit(str[i+1]) || str[i+1]=='.')
				{
					i++;
					peek+=str[i];
				}
				else{break;}
			}
			//以Regular Expression判斷是否match NUM的格式
			regex pattern("[0-9]+(\.[0-9]+)?");
			bool match=regex_match(peek,pattern);
			if(match){
				//cout<<"NUM matched - "<<peek<<endl;
				t_type=1;
			}
			else{cout<<peek<<"\tError-NUM not matched."<<endl;}	//不match時，印出Error Msg
		}
		else if(isalpha(str[i]))	//處理ID：參考中文版課本第2.6.4章 (page.2-43)
		{
			peek+=str[i];
			while(i+1<str.length())
			{
				//預讀，若為字母/數字/底線就加入peek
				if(isalpha(str[i+1]) || isdigit(str[i+1]) || str[i+1]=='_')
				{
					i++;
					peek+=str[i];
				}
				else{break;}
			}
			//以RE判斷是否match ID的格式
			regex pattern("[a-zA-Z][a-zA-Z_0-9]*");
			bool match=regex_match(peek,pattern);
			if(match){
				//cout<<"ID matched - "<<peek<<endl;
				t_type=2;
			}
			else{cout<<peek<<"\tError-ID not matched."<<endl;}	//不match時，印出Error Msg
		}
		else if(str[i]=='='){t_type=3;}
		else if(str[i]=='<' && str[i+1]!='='){t_type=4;}
		else if(str[i]=='<' && str[i+1]=='='){t_type=5;i=i+1;}
		else if(str[i]=='+'){t_type=6;}
		else if(str[i]=='*'){t_type=7;}
		else if(str[i]=='('){t_type=8;}
		else if(str[i]==')'){t_type=9;}

		for(int j=i+1;j<str.length();j++) {s+=str[j];}	//更新尚未處理的statement字串
		str=s;
		//若RE核對後無誤，則形成token並回傳token type和lexemes (P4的output)
		//if(t_type!=0){return "<"+type_list[t_type]+","+peek+">";}
		if(t_type!=0){return type_list[t_type];}
		else {return "EOF.(2)";}
	}
}
//參考課程PPT(02.Hint)page.31-33及中文課本Ch2.3.5
int match(string t)
{
	if(lookahead==t)
	{
		//更新執行完的statement字串（用於error時，可印出當前執行階段）
		if(t=="num" || t=="id")
		{
			error_str+=peek;
			postfix_str+=peek+" ";	//非運算符號，直接丟進postfix
		}
		else {error_str+=t;}
		lookahead=next_Token();	//提取下一個token，更新lookahead(指標向後移的概念)
		return 1;
	}
	else{return 0;}
}
/* 
 * [消除left recursion後的G1文法]
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
	rel();
	if(lookahead=="=")
	{
		match("=");
		expr();
		postfix_str+="= ";	//Translation actions
	}
	else{}
}
void rel()
{
	//rel -> add rel_tail
	add();
	rel_tail();
}
void rel_tail()
{
	//rel_tail -> '<' add rel_tail
	//rel_tail -> '<=' add rel_tail
	//rel_tail -> null
	if(lookahead=="<")
	{
		match("<");
		add();
		postfix_str+="< ";	//Translation actions:插在add()和rel_tail()之間
		rel_tail();
	}
	else if(lookahead=="<=")
	{
		match("<=");
		add();
		postfix_str+="<= ";	//Translation actions:插在add()和rel_tail()之間
		rel_tail();
	}
	else{}
}
void add()
{
	//add -> term add_tail
	term();
	add_tail();
}
void add_tail()
{
	//add_tail -> + term add_tail
	//add_tail -> null
	if(lookahead=="+")
	{
		match("+");
		term();
		postfix_str+="+ ";	//Translation actions:插在term()和add_tail()之間
		add_tail();
	}
	else{}	
}
void term()
{
	//term -> factor term_tail
	factor();
	term_tail();
}
void term_tail()
{
	//term_tail -> * factor term_tail
	//term_tail -> null
	if(lookahead=="*")
	{
		match("*");
		factor();
		postfix_str+="* ";	//Translation actions:插在factor()和term_tail()之間
		term_tail();
	}
	else{}
}
void factor()
{
	//factor -> ( expr )
	//factor -> num
	//factor -> id
	if(lookahead=="(")
	{
		match("(");
		expr();
		match(")");
	}
	else if(lookahead=="num")
	{
		match("num");
	}
	else if(lookahead=="id")
	{
		match("id");
	}
	else
	{
		error_str+=lookahead;
		lookahead="EOF.(3)";	//找不到相符的terminal，直接將lookahead換成結束用的token，以結束parsing
		cout<<"\t[ERROR]不是符合的terminal"<<endl;
		cout<<"\tConversion stops at: "<<postfix_str<<endl;
	}
}
int main()
{
	while(1)
	{
		init();					//Reset全域變數
		cout<<"Input the statement:"<<endl;
		getline(cin,str);		//讀入statement
		if(str=="0"){break;}	//為方便輸出檔案，設置輸入0時，結束程式
		del_comment();			//剔除註解
		del_space();			//剔除空白
		lookahead=next_Token();

		expr();					//開始執行Parsing
		if(lookahead=="EOF.(1)")	//正常結束（整句statement讀完）
		{
			cout<<"此statement合法"<<endl;
			cout<<"Success."<<endl;
			cout<<"Postfix form of the input:"<<endl;	//P5指定output:Postfix form
			cout<<postfix_str<<endl;
		}
		else if(lookahead=="EOF.(2)")	//不合法（詞彙不符合Regular Expression判定）
		{
			cout<<endl;
			cout<<"此statement不合法"<<endl;
			cout<<"[Syntax Error(1)詞彙] Failed."<<endl;
		}
		else	//不合法（結構不符合G1文法），並印出處理終止階段
		{
			cout<<endl;
			cout<<"此statement不合法"<<endl;
			cout<<"[Syntax Error(2)文法] Failed."<<endl;
		}
		cout<<endl;
		cout<<"=================================================="<<endl;
	}
	
    return 0;
}