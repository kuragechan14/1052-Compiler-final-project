/*
 * (P4) Lexical Analyzer_v1 (第五組) 2017.06.26
 * Function:
 *	　	-詞彙分析：讀取statements，判斷字元後逐一生成token，並輸出token type和lexeme
 *		-剔除空白、巢狀註解、雙斜線註解
 *		-簡易的syntax error回報
 * Input:
 *	　	-PPT內的test statements：
 *			- (s1) a = b + c * d
 *			- (s2) day = (1461*2) + 4.5 + (153*6+2) < 5 + 3
 *			- (s3) 123 =  (123 +* 4 ) 5 * 6
 *			- (s4) 含巢狀註解，故說明內不便貼上
 *		-自訂測資：
 *			- a = d * c //Hello 456
 *			- 3..1415=pi
 * Output:
 *		-依照程式執行順序輸出階段性的執行結果
 *			-第(1)階段：剔除註解後的字串
 *			-第(2)階段：剔除空白後的字串
 *			-第(3)階段：輸出statement裡依序找到/生成的token <token type,lexeme>
 *		-有另行版本用以輸出資料檔(.txt)
 * Return:
 *		-0:程式結束
 */
#include <iostream>
#include <string>
#include <regex>
using namespace std;
string type_list[10]={"","NUM","ID","=","<","<=","+","*","(",")"};
string str;
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
	cout<<"(1)刪除註解:"<<str<<endl;
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
	cout<<"(2)刪除空白:"<<str<<endl;
}
string next_Token()	//處理數字、字串、符號，提出token：參考PPT(02)page.101及課本page2-43
{
	
	if(str.length()==0){return "EOF.(1)";}	//整句Input已判斷結束
	else
	{
		int i(0),t_type(0);
		string peek="";	//初始化peek
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
		if(t_type!=0){return "<"+type_list[t_type]+","+peek+">";}
		else {return "EOF.(2)";}	
	}
}
int main()
{
	while(1)
	{
		cout<<"Input the statement:"<<endl;
		getline(cin,str);	//讀入statement
		if(str=="0"){break;}	//為方便輸出檔案，設置輸入0時，結束程式

		del_comment();	//剔除註解
		del_space();		//剔除空白
		cout<<"(3)依序處理數字、字串、符號:"<<endl;
		while(1)	//不斷呼叫next_Token提取新的Token直到EOF
		{
			string t=next_Token();
			if(t=="EOF.(1)")	//End of file.
			{
				cout<<"End of file."<<endl;
				break;
			}
			else if(t=="EOF.(2)")	//若遇到Error則停止並顯示
			{
				cout<<"[Syntax Error(1)] End of file."<<endl;
				break;
			}
			else{cout<<t<<endl;}	//P4 output，包含token type及lexeme
		}
		cout<<endl;
		str="";
	}
	
    return 0;
}